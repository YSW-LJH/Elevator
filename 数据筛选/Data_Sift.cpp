#include <Data_Sift.h>

extern _File_Data* file_first;
extern _File_Data* file_now;


static void process(const int pre);
//采集一组数据
static vector<vector<int>> Data_select(Data*& cur_data, int pre);
//获取某一电梯状态下的数据
static map<long long, vector<vector<int>>> Data_get(File_Data* file_data, int pre);
//获取原始数据<文件名，<ID，原始数据>>
static map<string, map<long long, vector<int>>> Raw_data(int pre);
//打印原始数据中的有效数据
static void Raw_print(map<string, map<long long, vector<int>>> data, string path, int pre);

//记录结果,<ID+Bn+有效数据长度，有效数据位>
map<long long, vector<int>> valid_data;

//int main()
//{
//	string path;
//	int pre;
//	cout << "输入文件夹路径：\n";
//	cin >> path;
//	cout << "输入前置字节长度（0~2）\n";
//	cin >> pre;
//	string cmd;
//	string path_temp = path;
//	path_temp.append("\\out");
//	cmd = "mkdir " + path_temp;
//	system(cmd.c_str());
//	Data_Sift_main(path, pre, 0);
//	cout << "程序结束，输入任意字符结束\n";
//	cin >> path;
//	return 0;
//}

void Data_Sift_main(string path, const int pre, const int sel)
{
	valid_data.clear();
	vector<string>files;
	getFiles(path, files);
	for (auto& file : files)
	{
		tree_main(file);
	}
	Data_Combine();
	process(pre);
	//根据识别结果，获取原始数据
	map<string, map<long long, vector<int>>> raw_data = Raw_data(pre);
	Raw_print(raw_data, path, pre);
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
		if (up_flag && up_data.it != up_data.data.end())
			if (minn > up_data.it->first)
				minn = up_data.it->first;
		if (down_flag && down_data.it != down_data.data.end())
			if (minn > down_data.it->first)
				minn = down_data.it->first;
		//判断数据的有效性，静止时不发生改变，运行时发生改变
		bool* bit_flag = new bool[minn % 16 + 1];
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
		long long int key = minn >> 4;
		key <<= 4;
		key += bit_count;
		valid_data[key] = bit_pos;
		//判断下一条数据
		if (sta_flag)
		{
			if (minn == sta_data.it->first)
				sta_data.it++;
			if (sta_data.it == sta_data.data.end())
				sta_flag = false;
		}
		if (up_flag)
		{
			if (minn == up_data.it->first)
				up_data.it++;
			if (up_data.it == up_data.data.end())
				up_flag = false;
		}
		if (down_flag)
		{
			if (minn == down_data.it->first)
				down_data.it++;
			if (down_data.it == down_data.data.end())
				down_flag = false;
		}
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
		//----------判断数据位是否长于前置字节，
		if (pre >= temp_ptr->ID->ID % 16)
		{
			if (temp_ptr->ID->next != NULL)
				temp_ptr = temp_ptr->ID->next->child;
			else
				break;
			continue;
		}
		//字典KEY采用（ID+前置字节+数据长度(4bit为一位，统计的数据数减一(因为4个二进制数最大值为15))）
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
		//判断下一节点是否存在，如过不存在且下一ID也不存在，则退出循环
		if (temp_ptr->next == NULL)
			if (temp_ptr->ID->next != NULL)
				temp_ptr = temp_ptr->ID->next->child;
			else
				break;
	}
	return result;
}

//采集一组数据
vector<vector<int>> Data_select(Data*& cur_data, int pre)
{
	vector<vector<int>> result;
	int* temp_pre = new int[pre];
	for (int i = 0; i < pre; i++)
		temp_pre[i] = cur_data->com_data[i];
	//临时存一组数据
	vector<int> one_data;
	//在前置字节发生改变或到达链表末尾时结束
	while (1)
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
	for (auto &temp_data : result)
		for (int i = 0; i < (cur_data->size - pre) * 2; i++)
			if (one_data[i] == 0)
				if (temp_data[i] != result.front()[i])
					one_data[i] = 1;
	result.push_back(one_data);

	delete[]temp_pre;
	return result;
}

//获取原始数据
map<string, map<long long,vector<int>>> Raw_data(int pre)
{
	map<string, map<long long, vector<int>>> file_result;
	//指针指向第一个文件
	file_now = file_first->next->next->next;
	while (file_now)
	{
		map<long long, vector<int>>file_valid_data;
		//根据原始文件数据顺序进行迭代
		for (auto& data : file_now->data)
		{//判断该条数据是否含有效数据
			for (auto& val : valid_data)
			{
				//从KEY获取数据
				int ID = val.first >> (pre * 8 + 4);
				int B1 = -1;
				int B2 = -1;
				if (pre >= 1)B1 = (val.first >> (pre * 8 - 4)) % 0x100;
				if (pre == 2)B2 = (val.first >> 4) % 0x100;
				bool flag = false;
				//判断ID是否相同
				if (ID != data->ID->ID)
					continue;
				//判断是否符合筛选条件
				if (B1 == -1)
					flag = true;
				else if (B1 == data->com_data[0])
					if (B2 == -1)
						flag = true;
					else if (B2 == data->com_data[1])
						flag = true;

				if (!flag)
					continue;
				//判断数据符合条件，计算有效数据
				int num = 0;
				for (auto& pos : val.second)
				{
					num *= 16;
					if (pos % 2 == 0)
						num += data->com_data[pre + pos / 2] / 16;
					else
						num += data->com_data[pre + pos / 2] % 16;
				}
				//向字典增加数据
				if (file_valid_data[val.first].size() == 0)
				{
					//记录有效位的具体位置，并放在向量的第一位
					int pos_all = 0;
					for (auto& pos : val.second)
					{
						pos_all *= 16;
						pos_all += pos + pre * 2;
					}
					file_valid_data[val.first].push_back(pos_all);
				}
				file_valid_data[val.first].push_back(num);
				break;
			}
			file_result[file_now->file_path] = file_valid_data;
		}
		file_now = file_now->next;
	}
	return file_result;
}

//打印原始数据中的有效数据
void Raw_print(map<string, map<long long, vector<int>>> data, string path, int pre)
{
	ofstream file_out;
	file_out.open(path + "\\out\\Raw_data.txt", ios::out);
	file_out << "说明：有效位的每一位十六进制数表示一个有效数据的位置，从≥0，≤15" << endl << endl;
	for (auto a : data)
	{
		file_out << "原始文件名：" << a.first << endl;
		for (auto b : a.second)
		{
			file_out << "ID:" << hex << uppercase << setw(3) << setfill('0') << (b.first >> (pre * 8 + 4)) << " 前置字节：";
			if (pre >= 1)
				file_out << hex << uppercase << setw(2) << setfill('0') << (b.first >> (pre * 8 - 4)) % 0x100 << " ";
			if (pre == 2)
				file_out << hex << uppercase << setw(2) << setfill('0') << (b.first >> 4) % 0x100 << " ";
			file_out << endl;
			vector<int>::iterator c = b.second.begin();
			file_out << "有效位：" << hex << uppercase << (*c) << endl << "原始数据：" << endl;
			for (c++; c != b.second.end(); c++)
				file_out << "	" << hex << uppercase << setw(b.first % 16) << setfill('0') << (*c) << endl;
		}
		file_out << endl;
	}
	file_out.close();
}