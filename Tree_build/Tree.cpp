#include <Tree.h>

Tree_Root* roots = NULL;
Data* datas = NULL;
Tree_Root* root_first = NULL;
int ID_num = 0;
int Total_data_num = 0;

extern bool Len_same;
extern int Window_size;
extern string File_path;

static void data_process(char* buff, int& len);//处理一行数据
static void ID_update(bool& ID_exis, int data_temp);//更新ID信息
static void DATA_update(bool& data_exis, int flag, Data* node_temp, int data_temp);//更新数据信息
static void tree_process();//更新树的数据
static void Window_exis(unsigned int num, int pos);

void tree_delete();//删除树

void tree_main()
{
	ifstream file;
	file.open(File_path, ios::in);
	if (!file)
	{
		cout << "打开文件\"" << File_path << "\"失败(＞﹏＜), 请检查文件是否存在" << endl;
		return;
	}
	char buff[SIZE];
	int len = -1;
	while (!file.eof())
	{
		file.getline(buff, SIZE, '\n');
		data_process(buff, len);
	}
	file.close();
	tree_process();
}
void data_process(char* buff, int& len)//处理一行数据
{
	int flag = 0;//用于记录处理该行第几个数据
	bool ID_exis = false;//用于判断当前的ID是否已经存在
	bool data_exis = true;//用于判断当前数据是否已经存在
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];
	memset(data, -1, DATA_SIZE + 1);
	format(data, buff, flag);
	if (flag == 0)return;
	//判断是否所有数据一样长
	if (Len_same)
		if (len == -1)
			len = flag - 1;
		else if (len != flag - 1)
			Len_same = false;
	//更新ID为（ID+数据长度）
	data[0] <<= 4;
	data[0] += flag - 1;
	node_temp->size = flag - 1;
	//更新头节点ID信息
	ID_update(ID_exis, data[0]);
	//判断该数据为数据部分
	for (flag = 1; flag < DATA_SIZE + 1; flag++)
			DATA_update(data_exis, flag, node_temp, data[flag]);
	if (data_exis)
		//判断为相同数据
		{
			datas->count++;
			delete node_temp;
		}
	else
		roots->total_type++;
	Total_data_num++;
}
void ID_update(bool& ID_exis, int data_temp)
{
	//判断ID是否已存在
	for (Tree_Root* temp = root_first; temp; temp = temp->next)
	{
		if (data_temp < temp->ID)
			break;
		if (data_temp == temp->ID)
		{
			ID_exis = true;
			roots = temp;//如果存在，则将roots指向当前存在ID的根节点
			break;
		}
	}
	//ID不存在，则创建新节点，按由小到大顺序插入
	if (!ID_exis)
	{
		roots = root_first;
		while (1)
		{
			//如果当前root头节点为空，则更新root头节点
			if (!roots)
			{
				roots = new Tree_Root();
				root_first = roots;
				break;
			}
			if (roots->ID < data_temp)
			{
				//如果当前节点是尾节点，则在后面补充新数据
				if (roots->next == NULL)
				{
					roots->next = new Tree_Root();
					roots->next->pre = roots;
					roots = roots->next;
					break;
				}
				roots = roots->next;
			}
			else
			{
				Tree_Root* temp = new Tree_Root();
				if (roots->pre)
				{
					temp->pre = roots->pre;
					temp->pre->next = temp;
				}
				else root_first = temp;//如果当前节点是头节点，则更新头节点
				temp->next = roots;
				roots->pre = temp;
				roots = temp;
				break;
			}
		}
		roots->ID = data_temp;
	}
	datas = roots->child;//更新datas指针
	roots->count++;
}
void DATA_update(bool& data_exis, int flag, Data* node_temp, int data_temp)
{
	node_temp->com_data[flag - 1] = data_temp;
	if (datas == NULL)//如果头指针为空，则插入在头节点
	{
		data_exis = false;
		roots->child = node_temp;
	}
	if (data_exis)
		while (1)
			//判断数据插入位置
			if (node_temp->com_data[flag - 1] < datas->com_data[flag - 1])
			{
				data_exis = false;
				//判断是否在数据头节点插入
				if (datas->pre)
				{
					//数据中间插入
					node_temp->pre = datas->pre;
					node_temp->pre->next = node_temp;
				}
				else
					roots->child = node_temp;//在头节点插入，更新root的数据指针
				node_temp->next = datas;
				datas->pre = node_temp;
				break;
			}
			else if (node_temp->com_data[flag - 1] > datas->com_data[flag - 1])
			{
				if (datas->next)
				{
					datas = datas->next;
					bool pre_same = true;
					//判断前面的数据是否相同
					for (int j = 0; j < flag - 1; j++)
						if (node_temp->com_data[j] != datas->com_data[j])
						{
							pre_same = false;
							break;
						}
					//如果前部的数据不同，则插入在此处
					if (!pre_same)
					{
						data_exis = false;
						node_temp->pre = datas->pre;
						node_temp->pre->next = node_temp;
						node_temp->next = datas;
						datas->pre = node_temp;
						break;
					}
				}
				else
				{
					//数据尾部插入
					data_exis = false;
					datas->next = node_temp;
					node_temp->pre = datas;
					break;
				}
			}
			else
				break;
}
//更新树的数据
void tree_process()
{
	roots = root_first;
	for (; roots; roots = roots->next)
	{
		//数据位数相同则去掉ID后的数据位
		if (Len_same)
			roots->ID >>= 4;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			roots->Window[i] = new unsigned int[roots->total_type];
			for (int j = 0; j < roots->total_type; j++)
				roots->Window[i][j] = 0xF0000000;
		}
		ID_num++;
		datas = roots->child;
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (; datas; datas = datas->next)
		{
			if (pre_same == false)
			{
				roots->pre_data_type++;
				for (int i = 0; i < PRE_SIZE; i++)
					pre_data[i] = datas->com_data[i];
				num = 1;
				pre_same = true;
			}
			else
				num++;
			datas->pos = num;//记录每个数据在链表中的位置
			//与下一节点比对数据
			if (datas->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (datas->next->com_data[i] != pre_data[i])
						pre_same = false;
			//判断同一位置数据的种类数，统计滑窗数据
			if (Window_size != -1)
				for (int i = 0; i <= datas->size - Window_size; i++)
				{
					unsigned int temp = 0;
					for (int j = 0; j < Window_size; j++)
					{
						temp <<= 8;
						temp += datas->com_data[i + j];
					}
					Window_exis(temp, i);
				}
			//计算当前数据与前一组数据的对应字节位的差值
			if (datas->pre)
				for (int i = 0; i < DATA_SIZE; i++)
					datas->_pre[i] = datas->com_data[i] - datas->pre->com_data[i];
		}
	}
}
//检查当前数据是否已经记录，没记录则更新数据
void Window_exis(unsigned int num,int pos)
{
	int i = 0;
	for (; i <= roots->total_type - Window_size && roots->Window[pos][i] != 0xF0000000; i++)
		if (num == roots->Window[pos][i])
			return;
	roots->Window[pos][i] = num;
	roots->Window_count[pos]++;
	return;
}
//删除树
void tree_delete()
{
	roots = root_first;
	while (roots)
	{
		datas = roots->child;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			delete[] roots->Window[i];
		}
		while (datas)
		{
			Data* datas_temp = datas;
			datas = datas->next;
			delete datas_temp;
		}
		Tree_Root* roots_temp = roots;
		roots = roots->next;
		delete roots_temp;
	}

	roots = NULL;
	root_first = NULL;
	datas = NULL;
	ID_num = 0;
	Total_data_num = 0;
}