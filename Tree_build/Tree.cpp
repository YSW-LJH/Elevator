#include <Tree.h>

Data* data_now = NULL;
Data_Root* root_now = NULL;
Data_Root* root_first = NULL;//���ڵ�
File_Data* file_first = NULL;
File_Data* file_now = NULL;

static void name_process(string filename);//���ļ�����ȡ��Ϣ
static void data_process(char* buff, int& len);//����һ������
static void ID_update(bool& ID_exis, int data_temp);//����ID��Ϣ
static void DATA_update(bool& data_exis, Data* node_temp);//����������Ϣ
static void tree_update();//������������

void Data_Restore(string out_path);
void Data_Combine();
void tree_delete();    //ɾ����

void tree_main(string file_path)
{
	root_first = NULL;
	root_now = NULL;
	data_now = NULL;
	ifstream file;
	//�����ļ�ԭʼ������
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
	//�����ļ���
	name_process(file_path);
	//���ļ�
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
//�ļ�����ʽ�������£����ļ����л�ȡ�����Ϣ��������ʽ��name_(floor_begin)_(floor_pass)_(floor_end).txt
void name_process(string filename)
{
	//��ȡ�ļ�����ɾ��·����Ϣ
	filename = filename.substr(filename.rfind('\\') + 1, filename.length() - filename.rfind('\\') + 1);
	//��ȡ¥������
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
		//�ж��Ƿ���ɶ�ȡ
		if (filename[pos] == '.')
			break;
	}
	int temp = file_now->Floor_pass.front() - file_now->Floor_pass.back();
	file_now->Floor_height = abs(temp) + 1;
	file_now->direction = temp < 0;
}
void data_process(char* buff, int& len)//����һ������
{
	int flag = 0;        //���ڼ�¼����������ݸ���������ID
	bool ID_exis = false;//�����жϵ�ǰ��ID�Ƿ��Ѿ�����
	bool data_exis = true;//�����жϵ�ǰ�����Ƿ��Ѿ�����
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];//��¼�������ݣ�����ID
	memset(data, -1, sizeof(data));
	format(data, buff, flag);
	//�������Ϊ�գ�����Ϊ���ݴ������
	if (flag == 0)
		return;
	//�ж��Ƿ���������һ����
	if (file_now->Len_same)
		if (len == -1)
			len = flag;
		else if (len != flag)
			file_now->Len_same = false;
	//����IDΪ��ID+���ݳ��ȣ�
	data[0] <<= 4;
	data[0] += flag - 1;
	node_temp->size = flag - 1;
	//����ͷ�ڵ�ID��Ϣ
	ID_update(ID_exis, data[0]);
	//�������ݲ���
	for (flag = 1; flag < DATA_SIZE + 1; flag++)
		node_temp->com_data[flag - 1] = data[flag];
	DATA_update(data_exis, node_temp);
	//�ж������Ƿ��Ѵ���
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
	//�ж�ID�Ƿ��Ѵ���
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
	//��ǰ�ڵ�ҪôΪ�գ�ҪôΪβ�ڵ㣬Ҫôֵ���½ڵ��
	//ID�����ڣ��򴴽��½ڵ㣬����С����˳�����
	file_now->ID_num++;
	//�����ǰrootͷ�ڵ�Ϊ�գ������rootͷ�ڵ�
	if (!root_first)
	{
		root_first = new Data_Root();
		root_now = root_first;
	}

	//�����ǰ�ڵ���β�ڵ㣬���ں��油��������
	else if (root_now->next == NULL && data_temp > root_now->ID)
	{
		root_now->next = new Data_Root();
		root_now->next->pre = root_now;
		root_now = root_now->next;
	}
	//��ǰ�ڵ��ֵ���½ڵ��ֵ�����ڸýڵ�ǰ�����
	else
	{
		Data_Root* temp = new Data_Root();
		if (root_now->pre)
		{
			temp->pre = root_now->pre;
			temp->pre->next = temp;
		}
		else root_first = temp;//�����ǰ�ڵ���ͷ�ڵ㣬�����ͷ�ڵ�
		temp->next = root_now;
		root_now->pre = temp;
		root_now = temp;
	}
	root_now->ID = data_temp;
	root_now->file = file_now;
}
void DATA_update(bool& data_exis, Data* node_temp)
{
	data_now = root_now->child;//����datasָ��
	if (data_now == NULL)//���ͷָ��Ϊ�գ��������ͷ�ڵ�
	{
		data_exis = false;
		root_now->child = node_temp;
		data_now = node_temp;
		data_now->ID = root_now;
		return;
	}
	int flag = 0;
	//���½ڵ��λ��������pos_1��pos_2֮�䣬ȷ������λ��׼ȷ
	Data* pos_1 = data_now, * pos_2 = NULL;

	for (int i = 0; i < DATA_SIZE;)
	{
		//--�½ڵ�С�ڵ�ǰ�ڵ㣬�ڵ�ǰ�ڵ�ǰ����
		if (node_temp->com_data[i] < pos_1->com_data[i])
		{
			data_exis = false;
			//�ж��Ƿ�������ͷ�ڵ����
			if (pos_1->pre)
			{
				//�����м����
				node_temp->pre = pos_1->pre;
				pos_1->pre->next = node_temp;
			}
			else
				root_now->child = node_temp;//��ͷ�ڵ���룬����root������ָ��
			node_temp->next = pos_1;
			pos_1->pre = node_temp;
			pos_1 = pos_1->pre;
			break;
		}
		//--�½ڵ���ڵ�ǰ�ڵ㣬
		if (node_temp->com_data[i] > pos_1->com_data[i])
		{
			//�ж��Ƿ�����β������
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
		//--�½ڵ���ڵ�ǰ�ڵ�
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
//������������
void tree_update()
{
	root_now = root_first;
	for (; root_now; root_now = root_now->next)
	{
		//����λ����ͬ��ȥ��ID�������λ
		if (file_now->Len_same)
			root_now->ID >>= 4;
		//ͳ��ÿλʮ���������ĳ��ִ���
		int count[DATA_SIZE * 2][16];
		memset(count, 0, sizeof(count));
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			//��¼ÿ�������������е�λ��,��ǰ�����ֽڷֿ�
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
			//����һ�ڵ�ȶ�����
			if (data_now->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (data_now->next->com_data[i] != pre_data[i])
						pre_same = false;
			//ͳ��ÿ����ֽ���ͬ���ݵĳ��ִ���
			for (int i = 0; i < DATA_SIZE && data_now->com_data[i] != -1; i++)
			{
				count[i * 2][data_now->com_data[i] >> 4] += data_now->count;
				count[i * 2 + 1][data_now->com_data[i] % 16] += data_now->count;
			}
		}
		//�������ֵ
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
//�����ݻ�ԭ��ԭʼ�ļ���˳��
void Data_Restore(string out_path)
{
	ofstream out;
	out.open(out_path, ios::trunc);
	for (auto data : file_now->data)
	{
		//��ӡ����
		out << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << data->ID->ID << "  ";
		for (int i = 0; i < DATA_SIZE; i++)
			if (i < data->size)
				out << hex << uppercase << setw(2) << setfill('0') << data->com_data[i] << " ";
		//out << hex << uppercase << data->com_data[i] / 16 << " " << data->com_data[i] % 16 << " ";
			else
				out << "-1" << " ";

		//��ǩ��ӡ
		//if (data->tag_all.size() > 0)
		//	for (auto i : data->tag_all)
		//		if (i != 0)
		//			out << dec << i << " ";

		//���ʴ�ӡ
		//out << endl << " ";
		//for (int i = 0; i < data->size; i++)
		//	out << dec << data->percent[i][0] << " " << data->percent[i][1] << " ";

		//״̬��ӡ
		//if (data->status[0] != 0)
		out << dec << data->status[0] << ' ';
		out << dec << data->status[1] << ' ';

		//����¥���ӡ
		for (int i = 0; i < file_now->Floor_height; i++)
			out << data->exist_floor[i];

		out << endl;
	}
	//�����ݴ�ӡ
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
//���ļ����ݺϲ�
void Data_Combine()
{
	//����������ǰ���������½ڵ㣬����ͳ�����С����С���ֹ������Ϣ�������źŽ����¼�� ��ֹ ���ݼ��С�
	File_Data* total_up = new File_Data();
	File_Data* total_down = new File_Data();
	File_Data* total_sta = new File_Data();
	File_Data* total;
	Data* data_flag = NULL;
	Data_Root* root_flag = NULL;
	for (file_now = file_first; file_now; file_now = file_now->next)
	{
		//�ж����ݱ�ʾ�ĵ������з���
		if (file_now->Floor_height == 1)
			total = total_sta;
		else if (file_now->direction)
			total = total_up;
		else
			total = total_down;
		//��������������
		total->Total_data_num += file_now->Total_data_num;
		//ָ���ʼ��
		Data_Root* cur_root = file_now->tree_root;
		root_now = total->tree_root;
		root_flag = NULL;
		//����ID�ڵ�
		do
		{
			//��ǰ�ڵ�Ϊ�գ����Ʊ��ȽϵĽڵ�
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
			//�ȽϵĽڵ�ID��ͬ�����µ�ǰ�ڵ�����
			else if (*root_now == *cur_root)
			{
				root_now->count += cur_root->count;
			}
			//��ǰ�ڵ���ڱ��ȽϵĽڵ㣬�򽫱��ȽϽڵ�ĸ������뵱ǰ�ڵ�ǰ
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
			//��ǰ�ڵ�С�ڱ��ȽϵĽڵ㣬�������һ�ڵ�
			else
			{
				root_flag = root_now;
				root_now = root_now->next;
				continue;
			}
			//ָ���ʼ��
			Data* cur_data = cur_root->child;
			data_now = root_now->child;
			data_flag = NULL;
			//����Data�ڵ�
			do
			{
				//��ǰ�ڵ�Ϊ�գ����Ʊ��ȽϵĽڵ�
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
				//�ȽϵĽڵ�Data��ͬ�����µ�ǰ�ڵ�����
				else if (*data_now == *cur_data)
				{
					data_now->count += cur_data->count;
				}
				//��ǰ�ڵ���ڱ��ȽϵĽڵ㣬�򽫱��ȽϽڵ�ĸ������뵱ǰ�ڵ�ǰ
				else if (*data_now > *cur_data)
				{
					root_now->total_type++;
					Data* temp = data_now->pre;
					data_now->pre = Data::Add(cur_data);
					data_now->pre->next = data_now;
					if (temp)
						temp->next = data_now->pre;
				}
				//��ǰ�ڵ�С�ڱ��ȽϵĽڵ㣬�������һ�ڵ�
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
		//�źŽ���ϲ������ź�Ŀǰû�������ж��ֵ������
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
				//�жϸ�¥���ź��Ƿ��ڵ�ǰ¥�������д���,��������Ϊ¥���ź�׼ȷ��������Ϊ��׼ȷ��ɾ����
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
	//�ж�������ļ��Ƿ�������к���������
	if (total_up->Total_data_num == 0)
		total_sta->Signal = total_down->Signal;
	else if (total_down->Total_data_num == 0)
		total_sta->Signal = total_up->Signal;
	else
	{
		//�������ź�
		if (total_up->Signal["Door"]["Open"] != "")
			total_sta->Signal["Door"] = total_up->Signal["Door"];
		else
			total_sta->Signal["Door"] = total_down->Signal["Door"];

		//�������������ݣ��ж������ź�
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
				//���ж�¥���ź�λ�ú������Ƿ�һ��
				if (Pos_u[i] == Pos_d[j] && Inc_u[i] == Inc_d[j])
				{
					vector<string>data_u = string_splite(IB_u[i], " ");
					vector<string>data_d = string_splite(IB_d[j], " ");
					//���ݳ����Ƿ���ͬ
					if (data_u.size() <= 1 || data_u.size() != data_d.size())
						continue;
					//�ж�ID��ǰ���ֽ��Ƿ���ͬ
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
					//��¼��ͬ����λ��
					bool flag = false;
					int pos = -1;
					//��¼��һ���ҵ�����ͬ���ݵ�λ�ã�///////(��ʱ��ʹ��)->����ҵ����������ϲ���ͬ����λ��������
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
					//�ҵ������źŻ������ź���ͬ����¼�� ��ֹ ���ݽڵ���
					if (flag || pos == -1)
					{
						//����Floor����
						//����ID&Bit
						for (int k = 0; k <= HString2int(Pre_u[i]); k++)
							total_sta->Signal["Floor"]["ID&Bit"] += data_u[k] + " ";
						total_sta->Signal["Floor"]["ID&Bit"] += "\n";
						//����Pos
						total_sta->Signal["Floor"]["Pos"] += Pos_u[i] + "\n";
						//����Range
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
						//��������
						total_sta->Signal["Floor"]["Increment"] += Inc_u[i] + "\n";
						//����ǰ���ֽ�λ
						total_sta->Signal["Floor"]["Pre_size"] += Pre_u[i] + "\n";
						//����Run����
						if (flag)
						{
							total_sta->Signal["Run"]["Up"] += IB_u[i] + " Pos:" + to_string(pos) + "\n";
							total_sta->Signal["Run"]["Down"] += IB_d[j] + " Pos:" + to_string(pos) + "\n";
						}
					}
				}
	}
	//���½ڵ�˳�� ��ֹ->����->����->�ļ�����
	total_sta->next = total_up;
	total_up->next = total_down;
	total_down->next = file_first;

	file_first->pre = total_down;
	total_down->pre = total_up;
	total_up->pre = total_sta;

	file_first = total_sta;
}
//ɾ����
void tree_delete()
{
	file_now = file_first;
	while (file_now)
	{
		//ɾ��������
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
		//ɾ��¥������
		floor_compare_delete(file_now->floor_data);
		//ɾ����ǰ�ӽڵ�
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