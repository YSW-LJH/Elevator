#include <Tree.h>

Data* data_now = NULL;
Data_Root* root_now = NULL;
Data_Root* root_first = NULL;//根节点
File_Data* file_first = NULL;
File_Data* file_now = NULL;

static void name_process(string filename);//从文件名获取信息
static void data_process(char* buff, int& len);//处理一行数据
static void ID_update(bool& ID_exis, int data_temp);//更新ID信息
static void DATA_update(bool& data_exis, Data* node_temp);//更新数据信息
static void tree_update();//更新树的数据

void Data_Restore(string out_path);
void Data_Combine();
void tree_delete();    //删除树

void tree_main(string file_path)
{
	root_first = NULL;
	root_now = NULL;
	data_now = NULL;
	ifstream file;
	//建立文件原始数据树
	if (file_first == NULL)
	{
		file_first = new File_Data();
		file_now = file_first;
		file_now->file_path = file_path;
	}
	else
	{
		file_now->next = new File_Data();
		file_now->next->pre = file_now;
		file_now = file_now->next;
		file_now->file_path = file_path;
	}
	//处理文件名
	name_process(file_path);
	//打开文件
	file.open(file_path, ios::in);
	char buff[SIZE];
	int len = -1;
	while (!file.eof())
	{
		file.getline(buff, SIZE, '\n');
		data_process(buff, len);
	}
	file.close();
	tree_update();
	file_now->tree_root = root_first;
}
//文件名格式化处理下，从文件名中获取相关信息，命名格式：name_(floor_begin)_(floor_pass)_(floor_end).txt
void name_process(string filename)
{
	//获取文件名，删除路径信息
	filename = filename.substr(filename.rfind('\\') + 1, filename.length() - filename.rfind('\\') + 1);
	//获取楼层数据
	size_t pos = filename.find_first_of('_');
	while (1)
	{
		int num = 0;
		for (pos++; filename[pos] >= '0' && filename[pos] <= '9'; pos++)
		{
			num *= 10;
			num += filename[pos] - '0';
		}
		file_now->Floor_pass.push_back(num);
		//判断是否完成读取
		if (filename[pos] == '.')
			break;
	}
	int temp = file_now->Floor_pass.front() - file_now->Floor_pass.back();
	file_now->Floor_height = abs(temp) + 1;
	file_now->direction = temp < 0;
}
void data_process(char* buff, int& len)//处理一行数据
{
	int flag = 0;        //用于记录处理该行数据个数，包含ID
	bool ID_exis = false;//用于判断当前的ID是否已经存在
	bool data_exis = true;//用于判断当前数据是否已经存在
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];//记录该行数据，包含ID
	memset(data, -1, sizeof(data));
	format(data, buff, flag);
	//如果该行为空，则认为数据处理完成
	if (flag == 0)
		return;
	//判断是否所有数据一样长
	if (file_now->Len_same)
		if (len == -1)
			len = flag;
		else if (len != flag)
			file_now->Len_same = false;
	//更新ID为（ID+数据长度）
	data[0] <<= 4;
	data[0] += flag - 1;
	node_temp->size = flag - 1;
	//更新头节点ID信息
	ID_update(ID_exis, data[0]);
	//更新数据部分
	for (flag = 1; flag < DATA_SIZE + 1; flag++)
		node_temp->com_data[flag - 1] = data[flag];
	DATA_update(data_exis, node_temp);
	//判断数据是否已存在
	if (data_exis)
	{
		data_now->count++;
		delete node_temp;
	}
	else
	{
		root_now->total_type++;
	}
	root_now->count++;
	file_now->Total_data_num++;
	file_now->data.push_back(data_now);
}
void ID_update(bool& ID_exis, int data_temp)
{
	//判断ID是否已存在
	root_now = root_first;
	if (root_first)
		while (1)
		{
			if (data_temp < root_now->ID)
				break;
			if (data_temp == root_now->ID)
			{
				ID_exis = true;
				return;
			}
			if (root_now->next)
				root_now = root_now->next;
			else
				break;
		}
	//当前节点要么为空，要么为尾节点，要么值比新节点大
	//ID不存在，则创建新节点，按由小到大顺序插入
	file_now->ID_num++;
	//如果当前root头节点为空，则更新root头节点
	if (!root_first)
	{
		root_first = new Data_Root();
		root_now = root_first;
	}

	//如果当前节点是尾节点，则在后面补充新数据
	else if (root_now->next == NULL && data_temp > root_now->ID)
	{
		root_now->next = new Data_Root();
		root_now->next->pre = root_now;
		root_now = root_now->next;
	}
	//当前节点的值比新节点的值大，则在该节点前面插入
	else
	{
		Data_Root* temp = new Data_Root();
		if (root_now->pre)
		{
			temp->pre = root_now->pre;
			temp->pre->next = temp;
		}
		else root_first = temp;//如果当前节点是头节点，则更新头节点
		temp->next = root_now;
		root_now->pre = temp;
		root_now = temp;
	}
	root_now->ID = data_temp;
	root_now->file = file_now;
}
void DATA_update(bool& data_exis, Data* node_temp)
{
	data_now = root_now->child;//更新datas指针
	if (data_now == NULL)//如果头指针为空，则插入在头节点
	{
		data_exis = false;
		root_now->child = node_temp;
		data_now = node_temp;
		data_now->ID = root_now;
		return;
	}
	int flag = 0;
	//将新节点的位置限制在pos_1和pos_2之间，确保插入位置准确
	Data* pos_1 = data_now, * pos_2 = NULL;

	for (int i = 0; i < DATA_SIZE;)
	{
		//--新节点小于当前节点，在当前节点前插入
		if (node_temp->com_data[i] < pos_1->com_data[i])
		{
			data_exis = false;
			//判断是否在数据头节点插入
			if (pos_1->pre)
			{
				//数据中间插入
				node_temp->pre = pos_1->pre;
				pos_1->pre->next = node_temp;
			}
			else
				root_now->child = node_temp;//在头节点插入，更新root的数据指针
			node_temp->next = pos_1;
			pos_1->pre = node_temp;
			pos_1 = pos_1->pre;
			break;
		}
		//--新节点大于当前节点，
		if (node_temp->com_data[i] > pos_1->com_data[i])
		{
			//判断是否数据尾部插入
			if (pos_1->next == NULL)
			{
				data_exis = false;
				node_temp->pre = pos_1;
				pos_1->next = node_temp;
				pos_1 = pos_1->next;
				break;
			}
			if (pos_1 == pos_2)
			{
				data_exis = false;
				pos_1->next->pre = node_temp;
				node_temp->next = pos_1->next;
				node_temp->pre = pos_1;
				pos_1->next = node_temp;
				pos_1 = pos_1->next;
				break;
			}
			pos_1 = pos_1->next;
		}
		//--新节点等于当前节点
		if (node_temp->com_data[i] == pos_1->com_data[i])
		{
			for (i++; i < DATA_SIZE;)
				if (node_temp->com_data[i] == pos_1->com_data[i])
					i++;
				else
					break;
			for (pos_2 = pos_1; pos_2->next; pos_2 = pos_2->next)
			{
				int j = 0;
				for (; j < i; j++)
					if (pos_2->next->com_data[j] != pos_1->com_data[j])
						break;
				if (j != i)
					break;
			}
		}
	}
	data_now = pos_1;
	data_now->ID = root_now;
}
//更新树的数据
void tree_update()
{
	root_now = root_first;
	for (; root_now; root_now = root_now->next)
	{
		//数据位数相同则去掉ID后的数据位
		if (file_now->Len_same)
			root_now->ID >>= 4;
		//统计每位十六进制数的出现次数
		int count[DATA_SIZE * 2][16];
		memset(count, 0, sizeof(count));
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			//记录每个数据在链表中的位置,以前两个字节分开
			if (pre_same == false)
			{
				root_now->pre_data_type++;
				for (int i = 0; i < PRE_SIZE; i++)
					pre_data[i] = data_now->com_data[i];
				num = 1;
				pre_same = true;
			}
			else
				num++;
			data_now->pos = num;
			//与下一节点比对数据
			if (data_now->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (data_now->next->com_data[i] != pre_data[i])
						pre_same = false;
			//统计每半个字节相同数据的出现次数
			for (int i = 0; i < DATA_SIZE && data_now->com_data[i] != -1; i++)
			{
				count[i * 2][data_now->com_data[i] >> 4] += data_now->count;
				count[i * 2 + 1][data_now->com_data[i] % 16] += data_now->count;
			}
		}
		//计算概率值
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			for (int i = 0; i < DATA_SIZE && data_now->com_data[i] != -1; i++)
			{
				data_now->percent[i][0] = static_cast<double>(root_now->count) / count[i * 2][data_now->com_data[i] >> 4] - 1;
				data_now->percent[i][1] = static_cast<double>(root_now->count) / count[i * 2 + 1][data_now->com_data[i] % 16] - 1;
				data_now->percent_sum += data_now->percent[i][0] + data_now->percent[i][1];
			}
		}
	}
}
//树数据还原到原始文件的顺序
void Data_Restore(string out_path)
{
	ofstream out;
	out.open(out_path, ios::trunc);
	for (auto data : file_now->data)
	{
		//打印数据
		out << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << data->ID->ID << "  ";
		for (int i = 0; i < DATA_SIZE; i++)
			if (i < data->size)
				out << hex << uppercase << setw(2) << setfill('0') << data->com_data[i] << " ";
		//out << hex << uppercase << data->com_data[i] / 16 << " " << data->com_data[i] % 16 << " ";
			else
				out << "-1" << " ";

		//标签打印
		//if (data->tag_all.size() > 0)
		//	for (auto i : data->tag_all)
		//		if (i != 0)
		//			out << dec << i << " ";

		//概率打印
		//out << endl << " ";
		//for (int i = 0; i < data->size; i++)
		//	out << dec << data->percent[i][0] << " " << data->percent[i][1] << " ";

		//状态打印
		//if (data->status[0] != 0)
		out << dec << data->status[0] << ' ';
		out << dec << data->status[1] << ' ';

		//出现楼层打印
		for (int i = 0; i < file_now->Floor_height; i++)
			out << data->exist_floor[i];

		out << endl;
	}
	//树数据打印
	//for (root_now = root_first; root_now; root_now = root_now->next)
	//	for (data_now = root_now->child; data_now; data_now = data_now->next)
	//	{
	//		out << hex << uppercase << setw(3) << setfill('0') << data_now->ID->ID << " ";
	//		for (int i = 0; i < 8; i++)
	//			out << hex << uppercase << setw(2) << setfill('0') << data_now->com_data[i] << " ";
	//		out << endl;
	//	}
	out << endl;
	out.close();
}
//多文件数据合并
void Data_Combine()
{
	//在所有数据前插入三个新节点，用来统计上行、下行、静止数据信息，最终信号结果记录在 静止 数据集中。
	File_Data* total_up = new File_Data();
	File_Data* total_down = new File_Data();
	File_Data* total_sta = new File_Data();
	File_Data* total;
	Data* data_flag = NULL;
	Data_Root* root_flag = NULL;
	for (file_now = file_first; file_now; file_now = file_now->next)
	{
		//判断数据表示的电梯运行方向
		if (file_now->Floor_height == 1)
			total = total_sta;
		else if (file_now->direction)
			total = total_up;
		else
			total = total_down;
		//更新数据总条数
		total->Total_data_num += file_now->Total_data_num;
		//指针初始化
		Data_Root* cur_root = file_now->tree_root;
		root_now = total->tree_root;
		root_flag = NULL;
		//更新ID节点
		do
		{
			//当前节点为空，则复制被比较的节点
			if (root_now == NULL)
			{
				total->ID_num++;
				root_now = Data_Root::Add(cur_root);
				if (root_flag == NULL)
					total->tree_root = root_now;
				else
				{
					root_flag->next = root_now;
					root_now->pre = root_flag;
				}
			}
			//比较的节点ID相同，更新当前节点内容
			else if (*root_now == *cur_root)
			{
				root_now->count += cur_root->count;
			}
			//当前节点大于被比较的节点，则将被比较节点的副本插入当前节点前
			else if (*root_now > *cur_root)
			{
				total->ID_num++;
				Data_Root* temp = root_now->pre;
				root_now->pre = Data_Root::Add(cur_root);
				root_now->pre->next = root_now;
				if (temp)
					temp->next = root_now->pre;
				root_now = root_now->pre;
			}
			//当前节点小于被比较的节点，则进入下一节点
			else
			{
				root_flag = root_now;
				root_now = root_now->next;
				continue;
			}
			//指针初始化
			Data* cur_data = cur_root->child;
			data_now = root_now->child;
			data_flag = NULL;
			//更新Data节点
			do
			{
				//当前节点为空，则复制被比较的节点
				if (data_now == NULL)
				{
					root_now->total_type++;
					data_now = Data::Add(cur_data);
					if (data_flag == NULL)
						root_now->child = data_now;
					else
					{
						data_flag->next = data_now;
						data_now = data_flag;
					}
				}
				//比较的节点Data相同，更新当前节点内容
				else if (*data_now == *cur_data)
				{
					data_now->count += cur_data->count;
				}
				//当前节点大于被比较的节点，则将被比较节点的副本插入当前节点前
				else if (*data_now > *cur_data)
				{
					root_now->total_type++;
					Data* temp = data_now->pre;
					data_now->pre = Data::Add(cur_data);
					data_now->pre->next = data_now;
					if (temp)
						temp->next = data_now->pre;
				}
				//当前节点小于被比较的节点，则进入下一节点
				else
				{
					data_flag = data_now;
					data_now = data_now->next;
					continue;
				}
				//
				cur_data = cur_data->next;
			} while (cur_data);
			//
			cur_root = cur_root->next;
		} while (cur_root);
		//信号结果合并（门信号目前没有遇到有多种的情况）
		if (total == total_sta)
			continue;
		if (total->Signal["Floor"]["ID&Bit"] == "")
			total->Signal = file_now->Signal;
		else
		{
			vector<string>IB = string_splite(total->Signal["Floor"]["ID&Bit"], "\n");
			vector<string>Pos = string_splite(total->Signal["Floor"]["Pos"], "\n");
			vector<string>Inc = string_splite(total->Signal["Floor"]["Increment"], "\n");
			vector<string>Pre = string_splite(total->Signal["Floor"]["Pre_size"], "\n");

			vector<string>IB_t = string_splite(file_now->Signal["Floor"]["ID&Bit"], "\n");
			vector<string>Pos_t = string_splite(file_now->Signal["Floor"]["Pos"], "\n");
			vector<string>Inc_t = string_splite(file_now->Signal["Floor"]["Increment"], "\n");
			vector<string>Pre_t = string_splite(file_now->Signal["Floor"]["Pre_size"], "\n");

			string ib = "";
			string pos = "";
			string inc = "";
			string pre = "";
			for (int i = 0; i < IB.size(); i++)
			{
				//判断该楼层信号是否在当前楼层数据中存在,存在则认为楼层信号准确，否则认为不准确，删除。
				vector<string>data = string_splite(IB[i], " ");
				bool flag = false;
				for (int j = 0; j < IB_t.size(); j++)
				{
					vector<string>data_t = string_splite(IB_t[j], " ");
					if (Pos[i] == Pos_t[j])
						if (Inc[i] == Inc_t[j])
							if (Pre[i] == Pre_t[j])
							{
								for (int k = 0; k <= HString2int(Pre[i]); k++)

									if (data[k] == data_t[k])
										flag = true;
									else
									{
										flag = false;
										break;
									}
								if (flag)
									break;
							}
				}
				if (flag)
				{
					ib += IB[i] + "\n";
					pos += Pos[i] + "\n";
					inc += Inc[i] + "\n";
					pre += Pre[i] + "\n";
				}
			}
			total->Signal["Floor"]["ID&Bit"] = ib;
			total->Signal["Floor"]["Pos"] = pos;
			total->Signal["Floor"]["Increment"] = inc;
			total->Signal["Floor"]["Pre_size"] = pre;
		}
	}
	//判断输入的文件是否包含上行和下行数据
	if (total_up->Total_data_num == 0)
		total_sta->Signal = total_down->Signal;
	else if (total_down->Total_data_num == 0)
		total_sta->Signal = total_up->Signal;
	else
	{
		//更新门信号
		if (total_up->Signal["Door"]["Open"] != "")
			total_sta->Signal["Door"] = total_up->Signal["Door"];
		else
			total_sta->Signal["Door"] = total_down->Signal["Door"];

		//根据上下行数据，判断运行信号
		vector<string>IB_u = string_splite(total_up->Signal["Floor"]["ID&Bit"], "\n");
		vector<string>Pos_u = string_splite(total_up->Signal["Floor"]["Pos"], "\n");
		vector<string>Inc_u = string_splite(total_up->Signal["Floor"]["Increment"], "\n");
		vector<string>Pre_u = string_splite(total_up->Signal["Floor"]["Pre_size"], "\n");

		vector<string>IB_d = string_splite(total_down->Signal["Floor"]["ID&Bit"], "\n");
		vector<string>Pos_d = string_splite(total_down->Signal["Floor"]["Pos"], "\n");
		vector<string>Inc_d = string_splite(total_down->Signal["Floor"]["Increment"], "\n");
		vector<string>Pre_d = string_splite(total_up->Signal["Floor"]["Pre_size"], "\n");

		for (int i = 0; i < IB_u.size(); i++)
			for (int j = 0; j < IB_d.size(); j++)
				//先判断楼层信号位置和增量是否一致
				if (Pos_u[i] == Pos_d[j] && Inc_u[i] == Inc_d[j])
				{
					vector<string>data_u = string_splite(IB_u[i], " ");
					vector<string>data_d = string_splite(IB_d[j], " ");
					//数据长度是否相同
					if (data_u.size() <= 1 || data_u.size() != data_d.size())
						continue;
					//判断ID和前置字节是否相同
					bool pre_same = true;
					for (int k = 0; k <= HString2int(Pre_u[i]); k++)
					{
						if (data_u[k] != data_d[k])
						{
							pre_same = false;
							break;
						}
					}
					if (!pre_same)
						continue;
					//记录不同数据位数
					bool flag = false;
					int pos = -1;
					//记录第一次找到不相同数据的位置，///////(暂时不使用)->如果找到两个及以上不相同数据位，则跳过
					for (int k = 1; k < data_u.size(); k++)
						if (data_u[k] != data_d[k])
							if (!flag)
							{
								pos = k;
								flag = true;
								break;
							}
							//else
							//{
							//	flag = false;
							//	break;
							//}
					//找到运行信号或两个信号相同，记录到 静止 数据节点下
					if (flag || pos == -1)
					{
						//更新Floor数据
						//更新ID&Bit
						for (int k = 0; k <= HString2int(Pre_u[i]); k++)
							total_sta->Signal["Floor"]["ID&Bit"] += data_u[k] + " ";
						total_sta->Signal["Floor"]["ID&Bit"] += "\n";
						//更新Pos
						total_sta->Signal["Floor"]["Pos"] += Pos_u[i] + "\n";
						//更新Range
						vector<string>range_u = string_splite(total_up->Signal["Floor"]["Range"], " ");
						vector<string>range_d = string_splite(total_down->Signal["Floor"]["Range"], " ");
						int min = 0;
						int max = 0;
						if (HString2int(range_u[0]) <= HString2int(range_d[0]))
							min = HString2int(range_u[0]);
						if (HString2int(range_u[1]) >= HString2int(range_d[1]))
							max = HString2int(range_u[1]);
						string min_s = int2HString(min, 2);
						string max_s = int2HString(max, 2);
						total_sta->Signal["Floor"]["Range"] += min_s + " " + max_s + "\n";
						//更新增量
						total_sta->Signal["Floor"]["Increment"] += Inc_u[i] + "\n";
						//更新前置字节位
						total_sta->Signal["Floor"]["Pre_size"] += Pre_u[i] + "\n";
						//更新Run数据
						if (flag)
						{
							total_sta->Signal["Run"]["Up"] += IB_u[i] + " Pos:" + to_string(pos) + "\n";
							total_sta->Signal["Run"]["Down"] += IB_d[j] + " Pos:" + to_string(pos) + "\n";
						}
					}
				}
	}
	//更新节点顺序 静止->上行->下行->文件数据
	total_sta->next = total_up;
	total_up->next = total_down;
	total_down->next = file_first;

	file_first->pre = total_down;
	total_down->pre = total_up;
	total_up->pre = total_sta;

	file_first = total_sta;
}
//删除树
void tree_delete()
{
	file_now = file_first;
	while (file_now)
	{
		//删除树数据
		root_now = file_now->tree_root;
		while (root_now)
		{
			data_now = root_now->child;
			while (data_now!=NULL)
			{
				Data* datas_temp = data_now;
				data_now = data_now->next;
				delete datas_temp;
			}
			Data_Root* roots_temp = root_now;
			root_now = root_now->next;
			delete roots_temp;
		}
		//删除楼层数据
		floor_compare_delete(file_now->floor_data);
		//删除当前接节点
		File_Data* file_temp = file_now;
		file_now = file_now->next;
		delete file_temp;
	}
	root_now = NULL;
	root_first = NULL;
	data_now = NULL;
	file_first = NULL;
	file_now = NULL;
}