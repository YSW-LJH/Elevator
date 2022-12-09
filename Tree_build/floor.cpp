#include <floor.h>

Floor* floors = NULL;
Floor* floor_first = NULL;

extern Tree_Root* roots;
extern Data* datas;
extern Tree_Root* root_first;
extern int ID_num;

extern int Mode;
extern int Floor_height;
extern string File_name;
extern string File_path;

static void pre_process();
static void pre_process_2();
static void pre_process_2_help(vector<vector<int>>& data, int ID, int pre_size);
static void new_node(Tree_Root* root, Data* data, int count, int pos, int pre_num, int sub_num);//pre_process_help
static void floor_merge();
static bool node_compare(Floor* A, Floor* B);

void floor_delete();

void floor_main()
{
	pre_process_2();
	if (floor_first == NULL)
	{
		cout << "未找到可能的楼层数据！！！\n";
		return;
	}
	//合并相同数据
	//floor_merge();
}
//第一种处理方式，直接查找递增数列--竹韵数据遇到了问题.
static void pre_process()
{
	for (roots = root_first; roots; roots = roots->next)
		for (int i = 0; i <= 4; i++)//目前按照定义数据含义的位数小于等于4位来算
		{
			int temp_data[DATA_SIZE] = { 0 };//记录临时数据
			bool pre_same = false;//记录前部数据是否相同
			int count[DATA_SIZE] = { 0 };//计数
			Data* data_temp[DATA_SIZE] = { NULL };//保留指针
			datas = roots->child;
			for (; datas; )
			{
				//更新需要比对的数据
				if (pre_same == false)
				{
					//初始化数据
					for (int j = 0; j < i; j++)
						temp_data[j] = datas->com_data[j];
					for (int j = 0; j < DATA_SIZE; j++)
						data_temp[j] = datas;
					memset(count, 0, sizeof(count));
					pre_same = true;
					datas = datas->next;
					//下一节点不存在则退出
					if (datas == NULL)
						break;
					for (int j = i; j < DATA_SIZE; j++)
						temp_data[j] = datas->_pre[j];
				}
				//比对前部数据是否相同, 不同则更新数据 ,
				for (int j = 0; j < i; j++)
					if (temp_data[j] != datas->com_data[j])
					{
						for (int j = i; j < DATA_SIZE; j++)
							if (count[j] + 1 == Floor_height)
								new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
						pre_same = false;
						break;
					}
				if (pre_same == false)
					continue;
				//判断前部数据相同，开始进行统计
				if (i == 3 && datas->com_data[0] == 0x1D && datas->com_data[1] == 0x00 && datas->com_data[2] == 0x47)
					datas = datas;
				for (int j = i; j < DATA_SIZE; j++)
				{
					if (datas->_pre[j] == 0)
						continue;
					if (temp_data[j] == datas->_pre[j])
						count[j]++;
					else
					{
						if (count[j] + 1 == Floor_height)
							new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
						count[j] = 1;
						data_temp[j] = datas->pre;
						temp_data[j] = datas->_pre[j];
					}
				}
				datas = datas->next;
			}
			for (int j = i; j < DATA_SIZE; j++)
				if (count[j] + 1 == Floor_height)
					new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
		}
}
//第二种处理方式，统计一整列的数据，然后再找递增
static void pre_process_2()
{
	vector<vector<int>>data(8, vector<int>(0, 0));
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		for (roots = root_first; roots; roots = roots->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (datas = roots->child; datas; datas = datas->next)
			{
				//判断前部数据是否相同
				for (int i = 0; i < pre_size; i++)
					if (pre_data[i] != datas->com_data[i])
					{
						pre_same = false;
						break;
					}
				//前置数据不同，更新前置数据并处理现在已采集的数据
				if (!pre_same)
				{
					pre_process_2_help(data, roots->ID,pre_size);
					for (int i = 0; i < pre_size; i++)
						pre_data[i] = datas->com_data[i];
					pre_same = true;
				}
				//新数据放入向量
				for (int i = 0; i < DATA_SIZE; i++)
					data[i].push_back(datas->com_data[i]);
			}
			pre_process_2_help(data, roots->ID,pre_size);
		}
		delete[]pre_data;
	}
}
//判断数据是否符合楼层
static void pre_process_2_help(vector<vector<int>>& data, int ID, int pre_size)
{
	vector<int>temp;
	for (int pos = pre_size; pos < DATA_SIZE; pos++)
	{
		temp = data[pos];
		if (temp.size() <= 1)
			continue;
		//排序，合并相同的数据
		sort(temp.begin(), temp.end());
		vector<int>::iterator a = temp.begin();
		while (a != temp.end())
		{
			if (*a == *(a + 1))
				a = temp.erase(a);
			else a++;
		}
		if (temp.size() != Floor_height)
			continue;
		//判断数据是否为等差数列
		int flag = temp[1] - temp[0];
		bool same = true;
		a = temp.begin();
		a += 2;
		for (; a != temp.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = false;
				break;
			}
		if (!same)
			continue;
		//判断数据代表楼层，加入节点
		Floor* temp_floor = new Floor();
		temp_floor->pre_size = pre_size;
		temp_floor->byte_pos = pos + 1;
		temp_floor->height = Floor_height;
		temp_floor->count = Floor_height;
		temp_floor->ID = ID;
		temp_floor->sub_num = flag;
		for (int i = 0; i < Floor_height; i++)
		{
			vector<int>temp_0;
			for (int j = 0; j < DATA_SIZE; j++)
				temp_0.push_back(data[j][i]);
			temp_floor->data.push_back(temp_0);
		}
		if (floor_first == NULL)
		{
			floor_first = temp_floor;
			floors = temp_floor;
		}
		else
		{
			floors->next = temp_floor;
			temp_floor->pre = floors;
			floors = floors->next;
		}
	}
	data.clear();
	data.insert(data.begin(), 8, vector<int>(0, 0));
}
//创建新节点，仅第一种处理方式使用
static void new_node(Tree_Root* root, Data* data, int count, int pos, int pre_num, int sub_num)
{
	Floor* temp = new Floor();
	temp->file_path = File_path;
	temp->ID = root->ID;
	temp->count = count + 1;
	temp->byte_pos = pos + 1;
	temp->pre_size = pre_num;
	temp->sub_num = sub_num;
	temp->height = Floor_height;
	temp->data.push_back(vector<int>(0, 0));
	for (int i = 0; i < DATA_SIZE; i++)
		temp->data[0].push_back(data->com_data[i]);
	data = data->next;
	for (int i = 1; i <= count; data = data->next)
		if (data->_pre[pos] != 0)
		{
			vector<int>temp_data;
			for (int j = 0; j < DATA_SIZE; j++)
				temp_data.push_back(data->com_data[j]);
			temp->data.push_back(temp_data);
			i++;
		}
	//如果头节点为空，更新头节点
	if (floor_first == NULL)
	{
		floor_first = temp;
		floors = temp;
		return;
	}
	floors->next = temp;
	temp->pre = floors;
	floors = floors->next;
}
//节点合并--单文件处理时不使用，多文件处理的最后一步使用，取短原则
static void floor_merge()
{
	for (floors = floor_first; floors && floors->next; floors = floors->next)
		for (Floor* temp = floors->next; temp;)
			if (node_compare(floors, temp))
			{
				Floor* temp_del = temp;
				temp = temp->next;
				if (floors->pre_size < temp_del->pre_size)
				{
					floors->pre_size = temp_del->pre_size;
					floors->file_path = temp_del->file_path;
				}

				temp_del->pre->next = temp_del->next;
				if (temp_del->next)
					temp_del->next->pre = temp_del->pre;
				delete temp_del;
			}
			else
				temp = temp->next;
}
//比较两个节点之间的内容
static bool node_compare(Floor* A, Floor* B)
{
	if (A->ID != B->ID)
		return false;
	for (int i = 0; i < DATA_SIZE; i++)
		if (A->data[0][i] != B->data[0][i])
			return false;
	if (A->byte_pos != B->byte_pos)
		return false;
	if (A->count != B->count)
		return false;
	if (A->sub_num != B->sub_num)
		return false;
	return true;
}
//删除数据
void floor_delete()
{
	floors = floor_first;
	while (floors)
	{
		Floor* temp = floors;
		floors = floors->next;
		delete temp;
	}
	floors = NULL;
	floor_first = NULL;
}
