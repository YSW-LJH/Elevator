#include <floor.h>

extern Data_Root* root_now;
extern Data* data_now;
extern Data_Root* root_first;
extern File_Data* file_now;
extern File_Data* file_first;


static void pre_process();
static void pre_process_help(vector<Data*>& data, int ID, int pre_size);
static void floor_verify();

void floor_main()
{
	pre_process();
	if (file_now == NULL)
	{
		cout << "未找到可能的楼层数据！！！\n";
		return;
	}
	floor_verify();
}
//统计一整列的数据，然后再找递增
static void pre_process()
{
	vector<Data*>data;
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		root_first = file_now->tree_root;
		for (root_now = root_first; root_now; root_now = root_now->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (data_now = root_now->child; data_now; data_now = data_now->next)
			{
				//打过楼层标签的数据跳过
				if (data_now->tag_all.size() > 0)
					if (data_now->tag_all[0] % 16 == 1)
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
			if (data[0]->tag_all[0] % 16 == 1)
				if ((data[0]->tag_all[0] / 16) % 16 == pos + 1)
					continue;
		//采集pos位的数据
		vector<int>pos_data;
		for (auto i : data)
			pos_data.push_back(i->com_data[pos]);
		if (pos_data.size() <= 1)
			continue;
		//排序，合并相同的数据
		sort(pos_data.begin(), pos_data.end());
		vector<int>::iterator a = pos_data.begin();
		while ((a + 1) != pos_data.end())
		{
			if (*a == *(a + 1))
				a = pos_data.erase(a);
			else a++;
		}
		//判断数据是否为等差数列且与楼层高度相同
		if (pos_data.size() != file_now->Floor_height)
			continue;
		//if (pos_data.size() < 3)
		//	continue;
		int flag = pos_data[1] - pos_data[0];
		bool same = true;
		a = pos_data.begin();
		a += 2;
		for (; a != pos_data.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = false;
				break;
			}
		if (!same)
			continue;
		//判断数据代表楼层，打标签
		file_now->Floor_count++;
		//计算相同字节位，跳过楼层信号位
		int same_pos = 0;
		for (int temp_pos = 0; temp_pos < 8; temp_pos++)
		{
			if (temp_pos == pos || temp_pos >= data[0]->size)
			{
				same_pos *= 2;
				continue;
			}
			//统计不同楼层下某一位数据，判断是否每层都出现相同的值（可能是运行信号）
			map<int, vector<int>>pos_sig;
			pos_sig[data[0]->com_data[pos]].push_back(data[0]->com_data[temp_pos]);
			int run_pos = 0;
			for (int i = 1, floor_temp = data[0]->com_data[pos]; i < data.size(); i++)
			{
				if (data[i - 1]->com_data[temp_pos] != data[i]->com_data[temp_pos])
					flag = false;
				//统计每种楼层信号下的数据
				pos_sig[data[i]->com_data[pos]].push_back(data[i]->com_data[temp_pos]);
			}
			if (flag)
				same_pos = same_pos * 2 + 1;
			else
			{
				flag = false;
				//统计每种数据出现的楼层次数
				map<int, int>pos_count;
				for (auto temp_sig : pos_sig)
				{
					vector<int> temp_data = temp_sig.second;
					sort(temp_data.begin(), temp_data.end());
					for (int i = 0, num = -1; i < temp_data.size(); i++)
						if (num != temp_data[i])
						{
							num = temp_data[i];
							pos_count[num]++;
						}
				}
				//判断出现次数是否与楼层高度相同
				for(auto count:pos_count)
					if (count.second == file_now->Floor_height)
					{
						flag = true;
						break;
					}
				if (flag)
					same_pos = same_pos * 2 + 1;
				else
					same_pos *= 2;
			}
		}
		//添加楼层标签
		for (auto i : data)
			//---------------------------------第几个标签(F00000)---相同字节位统计(FF000)---最短前置字节(F00)-----楼层信号位(F0)--定义楼层信号(F)
			i->tag_all.push_back(file_now->Floor_count * pow(16, 5) + same_pos * pow(16, 3) + pre_size * pow(16, 2) + (pos + 1) * 16 + 1);
	}
	//向量初始化
	data.clear();
}
//数据验证
void floor_verify()
{
	if (file_now->Floor_count == 0)
		return;
	int* data_temp = new int[file_now->Floor_count] {0};//用于存放相同楼层标签的前一个数据,-1则认为楼层数据无效
	int* D_value = new int[file_now->Floor_count] {0};//用于存放每个楼层标签相邻数据间差值
	int* floor = new int[file_now->Floor_count] {0};//用于判断每个楼层标签当前数据表示的楼层
	int** floor_signal = new int*[file_now->Floor_count];//用于记录楼层信号详细信息
	for (int i = 0; i < file_now->Floor_count; i++)//ID 字节*8 POS MIN MAX Increment Pre_size
	{
		floor_signal[i] = new int[14];
		memset(floor_signal[i], -1, sizeof(int)*14);
	}
	for (auto data : file_now->data)
		if (data->tag_all.size() > 0)
			for (auto tag : data->tag_all)
				if (tag % 16 == 1)//判断为楼层标签
				{
					const int a = tag / pow(16, 5) - 1;//表示第几次打的标签
					const int b = (tag % (16 * 16)) / 16 - 1;//表示数据的第几位表示楼层
					const int c = data->com_data[b] - data_temp[a];//表示增量
					if (data_temp[a] == -1)//如果已经判断该数据无效，则继续比较下一个
						continue;
					//更新该标签记录的楼层信号
					//记录ID
					if (floor_signal[a][0] == -1)
						floor_signal[a][0] = data->ID->ID;
					//0x100：前置字节位， 0x200：楼层信号位， 0x300：不相同字节位， 0x400：无效字节位， 0x000相同字节位
					//记录不变字节及最短前置字节,在楼层信号第一次发生改变时
					if (data_temp[a] != 0 && c != 0)
						for (int i = 0, flag_data = (tag / (16 * 16)) % (16 * 16 * 16); i < 8; i++)
							if (floor_signal[a][i + 1] == -1)
							{
								if (i < flag_data % 16)
									//记录前置字节
									floor_signal[a][i + 1] = data->com_data[i] + 0x100;
								else if (i == b)
									//记录楼层信号
									floor_signal[a][i + 1] = 0x200;
								else if (i < data->size)
								{
									//记录其他相同数据位,不相同记录为0x300
									if ((flag_data / (int)pow(2, (11 - i)) % 2) == 1)
										floor_signal[a][i + 1] = data->com_data[i];
									else
										floor_signal[a][i + 1] = 0x300;
								}
								else
									//无效字节位
									floor_signal[a][i + 1] = 0x400;
							}
					//记录楼层信号位
					if (floor_signal[a][9] == -1)
						floor_signal[a][9] = b + 1;
					//记录信号最小值
					if (floor_signal[a][10] == -1 || floor_signal[a][10] > data->com_data[b])
						floor_signal[a][10] = data->com_data[b];
					//记录信号最大值
					if (floor_signal[a][11] == -1 || floor_signal[a][11] < data->com_data[b])
						floor_signal[a][11] = data->com_data[b];
					//记录信号增量
					if (c != 0)
						floor_signal[a][12] = file_now->direction ? c : -c;
					//记录前置字节
					if (c != 0)
						floor_signal[a][13] = (tag / (16 * 16)) % 16;
					//------------------------------------------------------------
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
					//只更新一次
					if (D_value[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						D_value[a] = c;
						continue;
					}
					//
					data_temp[a] = data->com_data[b];
					//判断该数据与前一个数据是否等差或者相同
					if (c != 0 && c != D_value[a])// && c != (-D_value[a]))
					{
						data_temp[a] = -1;
						file_now->Floor_count--;
						continue;
					}
				}
	//将楼层信号加入File_Data结构体
	map<string, string> signal = { {"ID&Bit",""},{"Pos",""},{"Range",""},{"Increment",""} };
	for (int i = 0; i < file_now->Floor_count; i++)
		//判断是否是有效的楼层信号
		if (data_temp[i] != -1)
		{
			int j = 0;
			signal["ID&Bit"] += int2HString(floor_signal[i][j], 3) + " ";
			//记录楼层信号部分
			while (++j < 9)
			{
				//前置字节
				if (floor_signal[i][j]/(16*16) == 1)
				{
					signal["ID&Bit"] += int2HString(floor_signal[i][j] - 0x100, 2) + " ";
					//signal["ID&Bit"] += int2HString(floor_signal[i][j], 2) + " ";
					continue;
				}
				//楼层信号位
				if (floor_signal[i][j] == 0x200)
				{
					signal["ID&Bit"] += "FL ";
					continue;
				}
				//不相同信号位
				if (floor_signal[i][j] == 0x300)
				{
					signal["ID&Bit"] += "XX ";
					continue;
				}
				//是否处理完成
				if (floor_signal[i][j] == 0x400)
				{
					j = 9;
					break;
				}
				//相同数据位
				signal["ID&Bit"] += int2HString(floor_signal[i][j], 2) + " ";
			}
			signal["ID&Bit"] += "\n";
			//记录楼层信号位置
			signal["Pos"] += to_string(floor_signal[i][j++]);
			signal["Pos"] += "\n";
			//记录楼层信号范围
			signal["Range"] += int2HString(floor_signal[i][j++], 2) + " ";
			signal["Range"] += int2HString(floor_signal[i][j++], 2);
			signal["Range"] += "\n";
			//记录楼层信号增量
			signal["Increment"] += to_string(floor_signal[i][j++]);
			signal["Increment"] += "\n";
			//记录楼层信号增量
			signal["Pre_size"] += to_string(floor_signal[i][j]);
			signal["Pre_size"] += "\n";
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
	for (int i = 0; i < file_now->Floor_count; i++)
		delete[]floor_signal[i];
	delete[]floor_signal;
}


