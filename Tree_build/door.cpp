#include<door.h>

extern File_Data* file_now;

Data* rule_1[3] = { NULL };//顺序：A-B-C-B-A
Data* rule_2[4] = { NULL };//顺序：A-B-C-D-A
Floor_Data* floor_data;

static bool find_1();
static bool find_2();
string data_to_string(Data* data);

void door_main()
{
	floor_data = file_now->floor_data;
	map<string, string> signal = { {"Open",""},{"Run_O",""},{"Run_C",""},{"Close",""} };
	bool flag;
	flag = find_1();
	if (!flag)
	{
		flag = find_2();
		if (flag)
		{
			signal["Open"] = data_to_string(rule_2[0]);
			signal["Run_O"] = data_to_string(rule_2[1]);
			signal["Run_C"] = data_to_string(rule_2[2]);
			signal["Close"] = data_to_string(rule_2[3]);
		}
	}
	else
	{
		signal["Open"] = data_to_string(rule_1[0]);
		signal["Run_O"] = data_to_string(rule_1[1]);
		signal["Run_C"] = data_to_string(rule_1[1]);
		signal["Close"] = data_to_string(rule_1[2]);
	}
	file_now->Signal["Door"] = signal;
	return;
}
//根据规则一查找
bool find_1()
{
	//记录楼层高度
	int height = file_now->Floor_height;
	//记录第一个经停（或终点）楼层的数据，并加入前一层楼的数据（经过或起始楼层）
	Floor_Data data_temp;
	for (int i = 2; i >= 1; i--)
		for (int j = 0; j < floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data.size(); j++)
			data_temp.data.push_back(floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data[j]);
	Floor_Data* data = &data_temp;
	//记录经停及终点楼层，初始化为 false
	bool* flag = new bool[height];
	for (int i = 0; i < height; i++)
		flag[i] = false;
	//更新经停及终点楼层位置
	for (int i = 1; i < file_now->Floor_pass.size(); i++)
		flag[file_now->Floor_pass[i] - 1] = true;
	//获取关门到位信号，筛选条件：起始、经停、终点楼层都出现,其余楼层不定
	for (vector<Data*>::iterator pos_1 = data->data.begin(); pos_1 != data->data.end(); pos_1++)
	{
		bool flag_1 = true;
		for (int i = 0; i < file_now->Floor_pass.size(); i++)
			if (!(*pos_1)->exist_floor[file_now->Floor_pass[i]-1])
			{
				flag_1 = false;
				break;
			}
		if (!flag_1)
			continue;
		rule_1[0] = *pos_1;
		(*pos_1)->status[1] = 1;
		//获取门运行信号，筛选条件：ID相同、未被选取，且仅在经停和终点楼层出现
		for (vector<Data*>::iterator pos_2 = pos_1 + 1; pos_2 != data->data.end(); pos_2++)
		{
			if ((*pos_2)->status[1] != 0)
				continue;
			if ((*pos_2)->ID->ID != rule_1[0]->ID->ID)
				continue;
			bool flag_2 = true;
			for (int i = 0; i < height; i++)
				if ((*pos_2)->exist_floor[i] != flag[i])
				{
					flag_2 = false;
					break;
				}
			if (!flag_2)
				continue;
			rule_1[1] = *pos_2;
			(*pos_2)->status[1] = 2;
			//获取开门到位信号，筛选条件：ID相同、未被选取，且仅在经停和终点楼层出现
			for (vector<Data*>::iterator pos_3 = pos_2 + 1; pos_3 != data->data.end(); pos_3++)
			{
				if ((*pos_3)->status[1] != 0)
					continue;
				if ((*pos_3)->ID->ID != rule_1[0]->ID->ID)
					continue;
				bool flag_3 = true;
				for (int i = 0; i < height; i++)
					if ((*pos_3)->exist_floor[i] != flag[i])
					{
						flag_3 = false;
						break;
					}
				if (!flag_3)
					continue;
				rule_1[2] = *pos_3;
				(*pos_3)->status[1] = 3;
				//验证门信号
				vector<Data*>::iterator temp = data->data.begin();
				//当前门状态 1 关门到位；2 门运动；3 开门到位
				int door_stat = 1;
				//当前门是在开门还是关门
				bool door_flag = true;
				//验证结果
				bool verify = true;
				for (; temp != data->data.end(); temp++)
					if ((*temp)->status[1] != 0 && (*temp)->status[1] != door_stat)
					{
						switch (door_stat)
						{
							//关门，且将要开门，则下一状态为门运动
						case 1:
							if (door_flag)
								door_stat = 2;
							else
								verify = false;
							break;
							//门运动，开门则下一状态为开门到位，关门则下一状态为关门到位
						case 2:
							if (door_flag)
								door_stat = 3;
							else
								door_stat = 1;
							break;
							//开门到位，则接下来进行关门，状态为门运动
						case 3:
							if (door_flag)
								door_flag = false;
							else
								verify = false;
							door_stat = 2;
							break;
						}
						//判断是否和预测值一致
						if ((*temp)->status[1] != door_stat)
							verify = false;
						if (!verify)
							break;
					}
				if (verify)
				{
					delete[]flag;
					return true;
				}
				(*pos_3)->status[1] = 0;
			}
			(*pos_2)->status[1] = 0;
		}
		(*pos_1)->status[1] = 0;
	}
	delete[]flag;
	return false;
}
//根据规则二查找
bool find_2()
{
	//记录楼层高度
	int height = file_now->Floor_height;
	//记录第一个经停（或终点）楼层的数据，并加入前一层楼的数据（经过或起始楼层）
	Floor_Data data_temp;
	for (int i = 2; i >= 1; i--)
		for (int j = 0; j < floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data.size(); j++)
			data_temp.data.push_back(floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data[j]);
	Floor_Data* data = &data_temp;
	//记录经停及终点楼层，初始化为 false
	bool* flag = new bool[height];
	for (int i = 0; i < height; i++)
		flag[i] = false;
	//更新经停及终点楼层位置
	for (int i = 1; i < file_now->Floor_pass.size(); i++)
		flag[file_now->Floor_pass[i] - 1] = true;
	//获取关门到位信号，筛选条件：起始、经停、终点楼层都出现,其余楼层不定
	for (vector<Data*>::iterator pos_1 = data->data.begin(); pos_1 != data->data.end(); pos_1++)
	{
		bool flag_1 = true;
		for (int i = 0; i < file_now->Floor_pass.size(); i++)
			if (!(*pos_1)->exist_floor[file_now->Floor_pass[i]-1])
			{
				flag_1 = false;
				break;
			}
		if (!flag_1)
			continue;
		rule_2[0] = *pos_1;
		(*pos_1)->status[1] = 1;
		//获取开门运行信号，筛选条件：ID相同、未被选取，且仅在经停和终点楼层出现
		for (vector<Data*>::iterator pos_2 = pos_1 + 1; pos_2 != data->data.end(); pos_2++)
		{
			if ((*pos_2)->status[1] != 0)
				continue;
			if ((*pos_2)->ID->ID != rule_2[0]->ID->ID)
				continue;
			bool flag_2 = true;
			for (int i = 0; i < height; i++)
				if ((*pos_2)->exist_floor[i] != flag[i])
				{
					flag_2 = false;
					break;
				}
			if (!flag_2)
				continue;
			rule_2[1] = *pos_2;
			(*pos_2)->status[1] = 2;
			//获取开门到位信号，筛选条件：ID相同、未被选取，且仅在经停和终点楼层出现
			for (vector<Data*>::iterator pos_3 = pos_2 + 1; pos_3 != data->data.end(); pos_3++)
			{
				if ((*pos_3)->status[1] != 0)
					continue;
				if ((*pos_3)->ID->ID != rule_2[0]->ID->ID)
					continue;
				bool flag_3 = true;
				for (int i = 0; i < height; i++)
					if ((*pos_3)->exist_floor[i] != flag[i])
					{
						flag_3 = false;
						break;
					}
				if (!flag_3)
					continue;
				rule_2[2] = *pos_3;
				(*pos_3)->status[1] = 3;
				//获取关门运行信号，筛选条件：ID相同、未被选取，且仅在经停和终点楼层出现
				for (vector<Data*>::iterator pos_4 = pos_3 + 1; pos_4 != data->data.end(); pos_4++)
				{
					if ((*pos_4)->status[1] != 0)
						continue;
					if ((*pos_4)->ID->ID != rule_2[0]->ID->ID)
						continue;
					bool flag_4 = true;
					for (int i = 0; i < height; i++)
						if ((*pos_4)->exist_floor[i] != flag[i])
						{
							flag_4 = false;
							break;
						}
					if (!flag_4)
						continue;
					rule_2[3] = *pos_4;
					(*pos_4)->status[1] = 4;
					//验证门信号
					vector<Data*>::iterator temp = data->data.begin();
					//当前门状态 1 关门到位；2 开门运动；3 开门到位；4 关门运行
					int door_stat = 1;
					//当前门是在开门还是关门
					bool door_flag = true;
					//验证结果
					bool verify = true;
					for (; temp != data->data.end(); temp++)
						if ((*temp)->status[1] != 0 && (*temp)->status[1] != door_stat)
						{
							switch (door_stat)
							{
								//关门，且将要开门，则下一状态为门运动
							case 1:
								if (door_flag)
									door_stat = 2;
								else
									verify = false;
								break;
								//开门运动，开门则下一状态为开门到位，关门则下一状态为关门到位
							case 2:
								if (door_flag)
									door_stat = 3;
								else
									verify = false;
								break;
								//开门到位，则接下来进行关门，状态为门运动
							case 3:
								if (door_flag)
									door_flag = false;
								else
									verify = false;
								door_stat = 4;
								break;
								//关门运动，接下来为关门保持
							case 4:
								if (door_flag)
									verify = false;
								door_stat = 1;
								break;
							}
							//判断是否和预测值一致
							if ((*temp)->status[1] != door_stat)
								verify = false;
							if (!verify)
								break;
						}
					if (verify)
					{

						delete[]flag;
						return true;
					}
					(*pos_4)->status[1] = 0;
				}
				(*pos_3)->status[1] = 0;
			}
			(*pos_2)->status[1] = 0;
		}
		(*pos_1)->status[1] = 0;
	}
	delete[]flag;
	return false;
}
//把Data->com_data数据转换为字符串
string data_to_string(Data* data)
{
	string temp = int2HString(data->ID->ID);
	if (temp.size() < 3)
		temp.insert(0, 3 - temp.size(), '0');		
	temp += " ";
	for (int i = 0; i < data->size; i++)
	{
		string hexx = int2HString(data->com_data[i]);
		if (hexx.size() < 2)
			hexx.insert(0, 2 - hexx.size(), '0');
		temp += hexx;
		temp += " ";
	}
	return temp;
}