#include <Tree.h>

Data* data_now = NULL;
Tree_Root* root_now = NULL;
Tree_Root* root_first = NULL;//根节点
File_Data* file_first = NULL;
File_Data* file_now = NULL;

extern bool Len_same;
extern string File_path;
extern int Floor_height;
extern int Floor_begin;
extern int Floor_end;

static void data_process(char* buff, int& len);//处理一行数据
static void ID_update(bool& ID_exis, int data_temp);//更新ID信息
static void DATA_update(bool& data_exis, Data* node_temp);//更新数据信息
void Data_Restore(string out_path);
static void tree_update();//更新树的数据

void tree_delete();    //删除树

void tree_main()
{
	ifstream file;
	//建立文件原始数据树
	if (file_first == NULL)
	{
		file_first = new File_Data();
		file_now = file_first;
		file_now->file_path = File_path;
	}
	else
	{
		file_now->next = new File_Data();
		file_now->next->pre = file_now;
		file_now = file_now->next;
		file_now->file_path = File_path;
	}
	//打开文件
	file.open(File_path, ios::in);
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
	file_now->Floor_height = Floor_height;
	file_now->Floor_begin = Floor_begin;
	file_now->Floor_end = Floor_end;

}
void data_process(char* buff, int& len)//处理一行数据
{
	int flag = 0;        //用于记录处理该行数据个数，包含ID
	bool ID_exis = false;//用于判断当前的ID是否已经存在
	bool data_exis = true;//用于判断当前数据是否已经存在
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];//记录该行数据，包含ID
	memset(data, -1, DATA_SIZE + 1);
	format(data, buff, flag);
	//如果该行为空，则认为数据处理完成
	if (flag == 0)
		return;
	//判断是否所有数据一样长
	if (Len_same)
		if (len == -1)
			len = flag;
		else if (len != flag)
			Len_same = false;
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
		root_first = new Tree_Root();
		root_now = root_first;
	}

	//如果当前节点是尾节点，则在后面补充新数据
	else if (root_now->next == NULL && data_temp > root_now->ID)
	{
		root_now->next = new Tree_Root();
		root_now->next->pre = root_now;
		root_now = root_now->next;
	}
	//当前节点的值比新节点的值大，则在该节点前面插入
	else
	{
		Tree_Root* temp = new Tree_Root();
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
		if (Len_same)
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
	for(file_now=file_first;file_now;file_now=file_now->next)
		for (auto data : file_now->data)
		{
			out << hex << uppercase << setw(3 + !Len_same) << setfill('0') << data->ID->ID << "  ";
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
			//for (int i = 0; i < DATA_SIZE; i++)
			//	out << dec << data->percent[i][0] << " " << data->percent[i][1] << " ";
			if(data->status[0]!=0)
			out << dec << data->status[0];
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
//多文件数据合并------待完善
void Combine()
{
	for (file_now = file_first; file_now; file_now = file_now->next)
		for (root_now = file_now->tree_root; root_now; root_now = root_now->next)
			for (data_now = root_now->child; data_now; root_now = root_now->next)
			{

			}
}
//删除树
void tree_delete()
{
	file_now = file_first;
	while (file_now)
	{
		root_now = file_now->tree_root;
		while (root_now)
		{
			data_now = root_now->child;
			while (data_now)
			{
				Data* datas_temp = data_now;
				data_now = data_now->next;
				delete datas_temp;
			}
			Tree_Root* roots_temp = root_now;
			root_now = root_now->next;
			delete roots_temp;
		}
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