#include <Tree.h>

Data* data_now = NULL;
Tree_Root* root_now = NULL;
Tree_Root* root_first = NULL;//根节点
File_Data* file_first = NULL;
File_Data* file_now = NULL;
extern bool Len_same;
extern int Window_size;
extern string File_path;

static void data_process(char* buff, int& len);//处理一行数据
static void ID_update(bool& ID_exis, int data_temp);//更新ID信息
static void DATA_update(bool& data_exis, Data* node_temp);//更新数据信息
static void tree_process();//更新树的数据
static void Window_exis(unsigned int num, int pos);

void tree_delete();//删除树

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
	tree_process();
	file_now->tree_root = root_first;
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
	if (flag == 0)return;
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
		root_now->total_type++;
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
	if (!ID_exis)
	{
		file_now->ID_num++;
		//如果当前root头节点为空，则更新root头节点
		if (!root_first)
		{
			root_first = new Tree_Root();
			root_now = root_first;
		}

		//如果当前节点是尾节点，则在后面补充新数据
		else if (root_now->next == NULL)
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
	Data* pos_1 = data_now, * pos_2 = NULL;
	for (int i = 0; i < DATA_SIZE;)
	{
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
		if (node_temp->com_data[i] == pos_1->com_data[i])
		{
			for (; i < DATA_SIZE;)
				if (node_temp->com_data[i + 1] == pos_1->com_data[i + 1])
					i++;
				else 
					break;

			for (pos_2 = pos_1; pos_2->next; pos_2 = pos_2->next)
				if (pos_2->next->com_data[i] != pos_1->com_data[i])
					break;
			i++;
		}
	}
	data_now = pos_1;
	data_now->ID = root_now;
}
//更新树的数据
void tree_process()
{
	root_now = root_first;
	for (; root_now; root_now = root_now->next)
	{
		//数据位数相同则去掉ID后的数据位
		if (Len_same)
			root_now->ID >>= 4;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			root_now->Window[i] = new unsigned int[root_now->total_type];
			for (int j = 0; j < root_now->total_type; j++)
				root_now->Window[i][j] = 0xF0000000;
		}
		data_now = root_now->child;
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (; data_now; data_now = data_now->next)
		{
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
			data_now->pos = num;//记录每个数据在链表中的位置,以前两个字节分开
			//与下一节点比对数据
			if (data_now->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (data_now->next->com_data[i] != pre_data[i])
						pre_same = false;
			//判断同一位置数据的种类数，统计滑窗数据
			//temp存储对应窗口字节的合并起来的的数，然后统计temp的种类数
			if (Window_size != -1)
				for (int i = 0; i <= data_now->size - Window_size; i++)
				{
					unsigned int temp = 0;
					for (int j = 0; j < Window_size; j++)
					{
						temp <<= 8;
						temp += data_now->com_data[i + j];
					}
					Window_exis(temp, i);
				}
			//计算当前数据与前一组数据的对应字节位的差值
			if (data_now->pre)
				for (int i = 0; i < DATA_SIZE; i++)
					data_now->_pre[i] = data_now->com_data[i] - data_now->pre->com_data[i];
		}
	}
}
//检查当前数据是否已经记录，没记录则更新数据
void Window_exis(unsigned int num, int pos)
{
	int i = 0;
	for (; i <= root_now->total_type - Window_size && root_now->Window[pos][i] != 0xF0000000; i++)
		if (num == root_now->Window[pos][i])
			return;
	root_now->Window[pos][i] = num;
	root_now->Window_count[pos]++;
	return;
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
			for (int i = 0; i < DATA_SIZE; i++)
			{
				delete[] root_now->Window[i];
			}
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