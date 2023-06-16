#include <Data_Sift.h>

extern _File_Data* file_first;
extern _File_Data* file_now;


static void process(const int pre);
//采集一组数据
static vector<vector<int>> Data_select(Data*& cur_data, int pre);
//获取某一电梯状态下的数据
static map<long long, vector<vector<int>>> Data_get(File_Data* file_data, int pre);

//记录结果
map<long long, vector<int>> result;

void Data_Sift_main(string path, const int pre, const int sel)
{
	result.clear();
	string out_path = path + "\\out";
	string cmd = "mkdir " + out_path;
	system(cmd.c_str());
	vector<string>files;
	getFiles(path, files);
	for (auto& file : files)
	{
		tree_main(file);
		//只有在楼层大于2时才分析特征信号
		if (file_now->Floor_height > 2)
		{
			floor_main();
			floor_compare_main();
			door_main();
		}
		Data_Restore(out_path + file.substr(file.rfind('\\'), file.length() - file.rfind('\\')));
	}
	Data_Combine();
	process(pre);
	_delete();
}

//参数：前置字节数
void process(const int pre)
{
	//
	bool sta_flag = false;
	bool up_flag = false;
	bool down_flag = false;
	SData sta_data;
	SData up_data;
	SData down_data;
	//静止数据
	if (file_first->Total_data_num != 0)
	{
		sta_data.data = Data_get(file_first, pre);
		sta_data.it = sta_data.data.begin();
		sta_flag = true;
	}
	//上行数据
	if (file_first->next->Total_data_num != 0)
	{
		up_data.data = Data_get(file_first->next, pre);
		up_data.it = up_data.data.begin();
		up_flag = true;
	}
	//下行数据
	if (file_first->next->next->Total_data_num != 0)
	{
		down_data.data = Data_get(file_first->next->next, pre);
		down_data.it = down_data.data.begin();
		down_flag = true;
	}
	//计算结果
	while (1)
	{
		//找到最小值，从小到大依次处理数据
		long long minn = (~((long long)1 << 63));
		if (sta_flag && sta_data.it != sta_data.data.end())
			if (minn > sta_data.it->first)
				minn = sta_data.it->first;
		if (up_flag && sta_data.it != up_data.data.end())
			if (minn > up_data.it->first)
				minn = up_data.it->first;
		if (down_flag && sta_data.it != down_data.data.end())
			if (minn > down_data.it->first)
				minn = down_data.it->first;
		//判断数据的有效性，静止时不发生改变，运行时发生改变
		bool* bit_flag = new bool[minn % 16];
		memset(bit_flag, true, sizeof(bit_flag));
		for (int i = 0; i <= minn % 16; i++)
			if (bit_flag[i])
			{
				if (sta_flag && sta_data.it->first == minn)
					if (sta_data.it->second.back()[i] == 1)
						bit_flag[i] = false;
				if (up_flag && up_data.it->first == minn)
					if (up_data.it->second.back()[i] == 0)
						bit_flag[i] = false;
				if (down_flag && down_data.it->first == minn)
					if (down_data.it->second.back()[i] == 0)
						bit_flag[i] = false;
			}
		//记录结果，key采用原key最后一位由记录原始数据长度变为记录有效数据长度
		//记录有效数据的具体位置
		vector<int> bit_pos;
		int bit_count = 0;
		for (int i = 0; i <= minn % 16; i++)
			if (bit_flag[i])
			{
				bit_pos.push_back(i);
				bit_count++;
			}
		int key = minn >> 4;
		key <<= 4;
		key += bit_count;
		result[key] = bit_pos;
		//判断下一条数据
		if (sta_flag)
			if (sta_data.it != sta_data.data.end())
			{
				if (minn == sta_data.it->first)
					sta_data.it++;
			}
			else
				sta_flag = false;
		if (up_flag)
			if (sta_data.it != up_data.data.end())
			{
				if (minn == up_data.it->first)
					up_data.it++;
			}
			else
				up_flag = false;
		if (down_flag)
			if (sta_data.it != down_data.data.end())
			{
				if (minn == down_data.it->first)
					down_data.it++;
			}
			else
				down_flag = false;
		//三种状态的标志全为false，退出循环
		if (!sta_flag && !up_flag && !down_flag)
			break;
		delete[]bit_flag;
	}

}

//获取某一电梯状态下的数据
map<long long, vector<vector<int>>> Data_get(File_Data* file_data, int pre)
{
	//
	map<long long, vector<vector<int>>> result;
	Data* temp_ptr = file_data->tree_root->child;
	while (1)
	{
		//-------------------------
		//字典KEY采用（ID+前置字节+数据长度(4bit为一位，统计的数据数减一)）
		long long key = temp_ptr->ID->ID;
		for (int i = 0; i < pre; i++)
		{
			key <<= 8;
			key += temp_ptr->com_data[i];
		}
		key <<= 4;
		key += (temp_ptr->size - pre) * 2 - 1;
		//采集一组数据
		result[key] = Data_select(temp_ptr, pre);
		//判断下一节点是否存在，入过不存在且下一ID也不存在，则退出循环
		if (temp_ptr->next == NULL)
			if (temp_ptr->ID->next != NULL)
				temp_ptr = temp_ptr->ID->next->child;
			else
				break;
	}
	return result;
}

//采集一组数据
vector<vector<int>> Data_select(Data*&cur_data,int pre)
{
	vector<vector<int>> result;
	int* temp_pre = new int[pre];
	for (int i = 0; i < pre; i++)
		temp_pre[i] = cur_data->com_data[i];
	//临时存一组数据
	vector<int> one_data;
	//在前置字节发生改变或到达链表末尾时结束
	while(1)
	{
		//判断前置数据是否相同
		bool pre_same = true;
		for (int j = 0; j < pre; j++)
			if (temp_pre[j] != cur_data->com_data[j])
			{
				pre_same = false;
				break;
			}
		if (!pre_same)
			break;
		//获取数据并放入向量，高低位分开
		for (int i = pre; i < cur_data->size; i++)
		{
			one_data.push_back(cur_data->com_data[i] / 16);
			one_data.push_back(cur_data->com_data[i] % 16);
		}
		result.push_back(one_data);
		one_data.clear();
		//前往下一节点
		if (cur_data->next == NULL)
			break;
		cur_data = cur_data->next;
	}
	//判断数据是否变化，结果放在向量最后，变化位记1，不变位记0
	one_data = vector<int>((cur_data->size - pre) * 2, 0);
	for (auto temp_data : result)
		for (int i = 0; i < (cur_data->size - pre) * 2; i++)
			if (one_data[i] == 0)
				if (temp_data[i] != result.front()[i])
					one_data[i] = 1;
	result.push_back(one_data);

	delete[]temp_pre;
	return result;
}