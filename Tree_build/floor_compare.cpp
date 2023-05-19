#include <floor_compare.h>

extern File_Data* file_first;
extern File_Data* file_now;
extern Data_Root* root_first;
extern Data_Root* root_now;
extern Data* data_now;

static void floor_splite();
static void floor_compare();
static void test();

void floor_compare_main()
{
	floor_splite();
	//test();
}
//按照楼层分割数据，连续两次出现相同楼层信号，才认为楼层改变
void floor_splite()
{
	Floor_Data* floor_data = NULL;
	//用数组储存不同的楼层数据
	floor_data = new Floor_Data[file_now->Floor_height];
	//获取原始数据
	vector<_Data*>file_data = file_now->data;
	//定义当前楼层
	int pos = file_now->Floor_pass.front();

	for (int i = 0; i < file_data.size(); i++)
	{
		//判断楼层标签,为0或与当前楼层相同，则统计
		if (file_data[i]->status[0] == 0 || file_data[i]->status[0] == pos)
		{
			floor_data[pos - 1].data.push_back(file_data[i]);
			file_data[i]->exist_floor[pos - 1] = 1;
			continue;
		}
		//楼层标签发生变化，判断下一个是否与pos相同，相同则认为楼层不变
		for (int j = i + 1; j < file_data.size(); j++)
		{
			if (file_data[j]->status[0] == 0)
				continue;
			if (file_data[j]->status[0] != pos)
				pos = file_data[i]->status[0];
			break;
		}
		floor_data[pos - 1].data.push_back(file_data[i]);
		file_data[i]->exist_floor[pos - 1] = 1;
	}
	file_now->floor_data = floor_data;
}
void floor_compare()
{
	vector<Data> all;

}
void test()
{
	int count[8] = { 0 };
	int type[8] = { 0 };
	int temp[8] = { 0 };
	memset(temp, -1, sizeof(temp));
	bool exist[8][256] = { false };
	memset(exist, false, sizeof(exist));
	for (auto data : file_now->data)
	{
		if (data->ID->ID == 8)
			for (int i = 0; i < 8; i++)
				if (data->com_data[i] != temp[i])
				{
					temp[i] = data->com_data[i];
					count[i]++;
					if (exist[i][temp[i]] == false)
					{
						exist[i][temp[i]] = true;
						type[i]++;
					}
				}
	}
}

void floor_compare_delete(Floor_Data*floor_data)
{
	if (floor_data != NULL)
		delete[]floor_data;
}
