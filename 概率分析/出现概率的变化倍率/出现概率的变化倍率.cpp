// 出现概率的变化倍率.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
module;
#include <func.h>
module Percent;

extern Data_Root* root_now;
extern Data* data_now;
extern Data_Root* root_first;

static void pre_process(string file, int& total, vector<vector<int>>& data);
static void calculate_compare();
static int compare(vector<int>& a, vector<int>& b);
static void calculate();
static void merger(vector<vector<int>>& data, int& total);
static void print(string& file_out, vector<vector<int>>& data, int& total);

vector<vector<int>>data_all;
vector<vector<int>>data_1;
vector<vector<int>>data_2;
int total_all = 0;
int total_1 = 0;
int total_2 = 0;

//using namespace std;

void percent_main(string path)
{
	return;
	string path_out;
	vector<string>files;
	path_out = path + "\\out";
	string cmd;
	cmd = "mkdir " + path_out;
	system(cmd.c_str());
	getFiles(path, files);
	for (int i = 0; i < files.size(); i++)
	{
		pre_process(files[i], total_1, data_1);
		_delete();
		string out_name = path_out;
		out_name.append(files[i].substr(files[i].rfind('\\'), files[i].length() - files[i].rfind('\\') - 4) + "_概率统计.txt");
		merger(data_1, total_1);
		print(out_name, data_1, total_1);
		data_1.clear();
	}
	print(path_out.append("\\total.txt"), data_all, total_all);
	//return ("输出文件位于“" + path_out + "”\n");
}
//int main()
//{
//	string file1, file2;
//	cout << "输入两个文件名，第一个不包含内招，第二个包含内招" << endl;
//	cin >> file1;
//	cin >> file2;
//
//	Mode = 1;
//	pre_process(file1, total_1, data_1);
//	pre_process(file2, total_2, data_2);
//	calculate_compare();
//	getchar();
//	getchar();
//	return 0;
//}
static void pre_process(string file, int& total, vector<vector<int>>& data)
{
	tree_main(file);
	if (root_first == NULL)
		return;
	total = root_first->file->Total_data_num;
	vector<int>temp_data(4, 0);
	for (root_now = root_first; root_now; root_now = root_now->next)
	{
		int pre_data[2] = { -1 };
		bool pre_same = false;
		int count = 0;
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			if (pre_data[0] != data_now->com_data[0] || pre_data[1] != data_now->com_data[1])
				pre_same = false;
			if (!pre_same)
			{
				if (pre_data[0] != -1)
				{
					temp_data[0] = root_now->ID;
					temp_data[1] = pre_data[0];
					temp_data[2] = pre_data[1];
					temp_data[3] = count;
					data.push_back(temp_data);
				}
				pre_data[0] = data_now->com_data[0];
				pre_data[1] = data_now->com_data[1];
				count = 0;
				pre_same = true;
			}
			count += data_now->count;
		}
		temp_data[0] = root_now->ID;
		temp_data[1] = pre_data[0];
		temp_data[2] = pre_data[1];
		temp_data[3] = count;
		data.push_back(temp_data);
	}
}
static void calculate_compare()
{
	vector<vector<int>>::iterator a, b;
	vector<vector<int>>a_b;
	vector<vector<int>>b_a;
	a = data_1.begin();
	b = data_2.begin();
	int max[3] = { 0 };
	double result = 0;
	while (a != data_1.end() && b != data_2.end())
	{
		switch (compare(*a, *b))
		{
		case 0:
			double aa, bb;
			aa = (double)(*a)[3] / total_1;
			bb = (double)(*b)[3] / total_2;
			if (result < (bb / aa))
			{
				result = bb / aa;
				for (int i = 0; i < 3; i++)
					max[i] = (*a)[i];
			}
			a++;
			b++;
			break;
		case 1:
			a_b.push_back(*a);
			a++;
			break;
		case 2:
			b_a.push_back(*b);
			b++;
			break;
		}
	}
	for (; a != data_1.end(); a++)
		a_b.push_back(*a);
	for (; b != data_2.end(); b++)
		b_a.push_back(*b);
	cout << "ID:";
	cout << hex << uppercase << setw(2) << setfill('0') << max[0];
	cout << "    pre_data:";
	cout << hex << uppercase << setw(2) << setfill('0') << max[1] << " "\
		<< hex << uppercase << setw(2) << setfill('0') << max[2] << endl;
	for (int i = 0; i < b_a.size(); i++)
	{
		cout << "ID:";
		cout << hex << uppercase << setw(2) << setfill('0') << b_a[i][0];
		cout << "    pre_data:";
		cout << hex << uppercase << setw(2) << setfill('0') << b_a[i][1] << " "\
			<< hex << uppercase << setw(2) << setfill('0') << b_a[i][2] << endl;
	}
}
static void print(string& file_out, vector<vector<int>>& data, int& total)
{
	ofstream out;
	out.open(file_out, ios::out);
	out << "ID	 Byte_1_2	 Percentage\n";
	for (int i = 0; i < data.size(); i++)
	{
		out << hex << uppercase << setw(2) << setfill('0') << data[i][0] << "	"\
			<< hex << uppercase << setw(2) << setfill('0') << data[i][1] << "	"\
			<< hex << uppercase << setw(2) << setfill('0') << data[i][2] << "	"\
			<< dec << (double)data[i][3] / total << endl;
	}
	out.close();
}
static void merger(vector<vector<int>>& data, int& total)
{
	vector<vector<int>>::iterator a = data.begin();
	vector<vector<int>>::iterator b = data_all.begin();
	total_all += total;
	while (a != data.end() && b != data_all.end())
		switch (compare(*a, *b))
		{
		case 0:
			(*b)[3] += (*a)[3];
			a++;
			b++;
			break;
		case 1:
			b = data_all.emplace(b, *a);
			a++;
			b++;
			break;
		case 2:
			b++;
			break;
		}
	if (b == data_all.end())
		for (; a != data.end(); a++)
			data_all.push_back(*a);
}
static int compare(vector<int>& a, vector<int>& b)
{
	for (int i = 0; i < 3; i++)
	{
		if (a[i] < b[i])
			return 1;
		else if (a[i] > b[i])
			return 2;
	}
	return 0;
}
