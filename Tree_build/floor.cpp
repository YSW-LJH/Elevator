#include <floor.h>

extern Data_Root* root_now;
extern Data* data_now;
extern Data_Root* root_first;
extern File_Data* file_now;
extern File_Data* file_first;


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
		if (temp.size() < file_now->Floor_height)
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
		if (same != file_now->Floor_height)
			continue;
		//判断数据代表楼层，打标签
		file_now->Floor_count++;

		for (auto i : data)
			i->tag_all.push_back(file_now->Floor_count * 1000 + pre_size * 100 + (pos + 1) * 10 + 1);//第几个标签+前置字节位+楼层信号位+定义楼层信号
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
	int** floor_signal = new int*[file_now->Floor_count];//用于记录楼层信号详细信息
	for (int i = 0; i < file_now->Floor_count; i++)//ID 字节*4 POS MIN MAX Increment
	{
		floor_signal[i] = new int[9];
		memset(floor_signal[i], -1, sizeof(int)*9);
	}
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
					//更新该标签的记录的楼层信号
					//记录ID
					if (floor_signal[a][0] == -1)
						floor_signal[a][0] = data->ID->ID;
					//记录不变字节
					for (int i = 0; i < (num / 100) % 10; i++)
						if (floor_signal[a][i + 1] == -1)
							floor_signal[a][i + 1] = data->com_data[i];
					//记录楼层信号位
					if (floor_signal[a][5] == -1)
						floor_signal[a][5] = b + 1;
					//记录信号最小值
					if (floor_signal[a][6] == -1 || floor_signal[a][6] > data->com_data[b])
						floor_signal[a][6] = data->com_data[b];
					//记录信号最大值
					if (floor_signal[a][7] == -1 || floor_signal[a][7] < data->com_data[b])
						floor_signal[a][7] = data->com_data[b];
					//记录信号增量
					if (c != 0)
						floor_signal[a][8] = c;
					//判断当前数据表示的楼层
					if (floor[a] == 0)
					{
						floor[a] = file_now->Floor_pass.front();
						data->status[0] = floor[a];
					}
					else
						if (c != 0)
						{
							floor[a] += file_now->direction ? 1 : -1;
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
						continue;
					}
				}
	//将楼楼层信号加入File_Data结构体
	string signal = "";
	for (int i = 0; i < file_now->Floor_height; i++)
		if (data_temp[i] != -1)
		{
			for (int j = 0; j < 9; j++)
				if (floor_signal[i][j] != -1)
				{
					if (j == 0) signal += "ID&Bit: ";
					if (j == 5)signal += "Pos: ";
					if (j == 6)signal += "Range: ";
					if (j == 8)signal += "Increment: ";
					if (j < 5 || j == 6 || j == 7)
					{
						string hexx = "";
						char a = 0;
						int flag = 0;
						for (int k = floor_signal[i][j]; k > 0; k /= 16, flag++)
							if (k % 16 < 10)
							{
								a = '0' + k % 16;
								hexx.insert(0, 1, a);
							}
							else
							{
								a = 'A' + k % 16 - 10;
								hexx.insert(0, 1, a);
							}
						if (flag < 2 && j>0)
							while (++flag <= 2)
								hexx.insert(0, 1, '0');
						if (flag < 3 && j == 0)
							while (++flag <= 3)
								hexx.insert(0, 1, '0');
						signal += hexx + ' ';
					}
					else
						signal += to_string(floor_signal[i][j]) + ' ';
				}
			signal += '\n';
		}
	file_now->Signal["Floor"] = signal;
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
	for (int i = 0; i < file_now->Floor_height; i++)
		delete[]floor_signal[i];
	delete[]floor_signal;
}


