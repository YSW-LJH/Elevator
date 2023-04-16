#include <floor.h>

extern Tree_Root* root_now;
extern Data* data_now;
extern Tree_Root* root_first;
extern File_Data* file_now;
extern File_Data* file_first;

extern int Mode;
extern int Floor_height;
extern string File_name;
extern string File_path;

static void pre_process();
static void pre_process_help(vector<Data*>& data, int ID, int pre_size);


void floor_main()
{
	pre_process();
	
	if (file_now == NULL)
	{
		cout << "未找到可能的楼层数据！！！\n";
		return;
	}
}
//统计一整列的数据，然后再找递增
static void pre_process()
{
	vector<Data*>data;
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		for (root_now = root_first; root_now; root_now = root_now->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (data_now = root_now->child; data_now; data_now = data_now->next)
			{
				//打过楼层标签的数据跳过
				if (data_now->tag_all.size() > 0)
					if (data_now->tag_all[0] % 10 == 1)
						continue;
				//判断前部数据是否相同
				for (int i = 0; i < pre_size; i++)
					if (pre_data[i] != data_now->com_data[i])
					{
						pre_same = false;
						break;
					}
				//前置数据不同，更新前置数据并处理现在已采集的数据
				if (!pre_same)
				{
					pre_process_help(data, root_now->ID,pre_size);
					for (int i = 0; i < pre_size; i++)
						pre_data[i] = data_now->com_data[i];
					pre_same = true;
				}
				//新数据放入向量
				data.push_back(data_now);
			}
			pre_process_help(data, root_now->ID,pre_size);
		}
		delete[]pre_data;
	}
}
//判断数据是否符合楼层
static void pre_process_help(vector<Data*>& data, int ID, int pre_size)
{
	if (data.size() == 0)
		return;
	for (int pos = pre_size; pos < DATA_SIZE; pos++)
	{
		//判断该位是否打过楼层标签
		if (data[0]->tag_all.size() > 0)
			if (data[0]->tag_all[0] % 10 == 1)
				if ((data[0]->tag_all[0] / 10) % 10 == pos + 1)
					continue;
		//采集pos位的数据
		vector<int>temp;
		for (auto i : data)
			temp.push_back(i->com_data[pos]);
		if (temp.size() <= 1)
			continue;
		//排序，合并相同的数据
		sort(temp.begin(), temp.end());
		vector<int>::iterator a = temp.begin();
		while ((a + 1) != temp.end())
		{
			if (*a == *(a + 1))
				a = temp.erase(a);
			else a++;
		}
		//判断数据是否为等差数列且至少有三组数据
		if (temp.size() < Floor_height)
			continue;
		//if (temp.size() < 3)
		//	continue;
		int flag = temp[1] - temp[0];
		int same = 2;
		a = temp.begin();
		a += 2;
		for (; a != temp.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = 0;
				break;
			}
			else
				same++;
		if (same == 0)
			continue;
		//判断递增数列是否和楼层高度相同
		if (same != Floor_height)
			continue;
		//判断数据代表楼层，打标签
		file_now->Floor_count++;
		int floor_temp = file_now->Floor_begin;
		int floor_flag = data[0]->com_data[pos];
		for (auto i : data)
			i->tag_all.push_back(file_now->Floor_count * 1000 + pre_size * 100 + (pos + 1) * 10 + 1);
	}
	//向量初始化
	data.clear();
}
//数据验证
void floor_verify()
{
	if (file_now->Floor_count == 0)
		return;
	int* data_temp = new int[file_now->Floor_count] {0};//用于存放相同楼层标签的前一个数据
	int* D_value = new int[file_now->Floor_count] {0};//用于存放每个楼层标签相邻数据间差值
	int* floor = new int[file_now->Floor_count] {0};//用于判断每个楼层标签当前数据表示的楼层
	for (auto data : file_now->data)
		if (data->tag_all.size() > 0)
			for (auto num : data->tag_all)
				if (num % 10 == 1)//判断为楼层标签
				{
					int a = num / 1000 - 1;//表示第几次打的标签
					int b = (num % 100) / 10 - 1;//表示数据的第几位表示楼层
					int c = data->com_data[b] - data_temp[a];//表示增量
					if (data_temp[a] == -1)//如果已经判断该数据无效，则继续比较下一个
						continue;
					//判断当前数据表示的楼层
					if (floor[a] == 0)
					{
						floor[a] = file_now->Floor_begin;
						data->status[0] = floor[a];
					}
					else
						if (c != 0)
						{
							floor[a] += file_now->Floor_begin < file_now->Floor_end ? 1 : -1;
							data->status[0] = floor[a];
						}
						else
							data->status[0] = floor[a];
					//初始化数据
					if (data_temp[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						continue;
					}
					if (D_value[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						D_value[a] = c;
						continue;
					}
					//
					data_temp[a] = data->com_data[b];
					//判断该数据与前一个数据是否等差或者相同
					if (c != 0 && c != D_value[a] && c != (-D_value[a]))
					{
						data_temp[a] = -1;
						file_now->Floor_count--;
					}
				}
	//遍历树，删除无效楼层标签
	for (root_now = file_now->tree_root; root_now; root_now = root_now->next)
		for (data_now = root_now->child; data_now; data_now = data_now->next)
			for (vector<int>::iterator data = data_now->tag_all.begin(); data != data_now->tag_all.end(); )
				if (*data % 10 == 1)
					if (data_temp[*data / 1000 - 1] == -1)
					{
						data = data_now->tag_all.erase(data);
						data_now->status[0] = 0;
					}
					else
						data++;
				else
					break;
	delete[]data_temp;
	delete[]D_value;
	delete[]floor;
}


