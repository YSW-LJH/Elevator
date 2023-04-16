#include <floor_compare.h>

char rule[4] = { 'A','B','C','D' };

extern File_Data* file_first;
extern File_Data* file_now;
extern Tree_Root* root_first;
extern Tree_Root* root_now;
extern Data* data_now;

static void pre_process();
static void test();

void door_delete();

void door_main()
{
	pre_process();
	//test();
}
//按照楼层分割数据，连续两次出现相同楼层信号，才认为楼层改变
static void pre_process()
{
	Floor_Data* floor = new Floor_Data[file_now->Floor_height];
	vector<_Data*>file_data = file_now->data;
	int num = file_now->Floor_begin;

	for (int i = 0; i < file_data.size(); i++)
	{
		if (file_data[i]->status[0] == 0 || file_data[i]->status[0] == num)
		{
			floor[num - 1].data.push_back(file_now->data[i]);
			continue;
		}
		for (int j = i + 1; j < file_data.size(); j++)
			if (file_data[j]->status[0] != 0)
				if (file_data[j]->status[0] != num)
				{
					num = file_data[i]->status[0];
					break;
				}
				else
					break;
		floor[num - 1].data.push_back(file_data[i]);
	}

	delete[] floor;
	int a = 0;
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
void door_delete()
{

}
