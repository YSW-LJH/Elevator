module;
#include <func.h>
module Multiple_file_process;

extern int Floor_begin;
extern int Floor_end;
extern int Floor_height;
extern _Floor* floors;
extern _Floor* floor_first;
extern string path;
extern string File_path;
extern string File_name;
extern int Mode;
extern bool Len_same;

vector<vector<_Floor>>floor_each;//保存每一个文件的数据
vector<_Floor>floor_all;//记录并集
vector<bool>status;//记录并集下每个数据的状态
vector<_Floor>floor_possibility;//记录交集
//vector<_Floor>floor_difference;//记录平层文件与运行文件的差集
ostringstream output;//记录输出结果字符串

static void floor_calculate_main();
static void pre_process(string filename);//预处理程序
static void combine();//求并集
static void filename_process(string filename);//文件名数据处理
static bool compare(const _Floor& a, const _Floor& b);//排序用比较函数
static void calculate();//求交集
static void check();

//根据多个输入文件计算可能的楼层数据
string path = "D:\\Code\\Elevator\\数据\\ZY-data\\过滤模式";

extern void getFiles(string path, vector<string>& files);

/*文件命名规则：文件名_起始楼层_结束楼层.txt
* 如：竹韵_1_4.txt，新时达_3_1.txt，123_1_55.txt
* 如过是平层信息，可以命名为：竹韵_1.txt 或 竹韵_1_1.txt ，即起始楼层和结束楼层可以仅写一个
*/

void multiple_file_process()
{
	floor_calculate_main();
}

static void floor_calculate_main()
{
	output.clear();
	vector<string>files;
	vector<string>::iterator file_name;
	getFiles((string)path, files);
	for (file_name = files.begin(); file_name != files.end(); file_name++)
		pre_process(*file_name);
	sort(floor_all.begin(), floor_all.end(), compare);
	calculate();
	//打印
	vector<_Floor>::iterator node;
	for (node = floor_possibility.begin(); node != floor_possibility.end(); node++)
	{
		output << hex << uppercase << setw(3 + !Len_same) << setfill('0') << node->ID \
			<< dec << ' ' << node->pre_size \
			<< ' ' << node->byte_pos \
			<< ' ' << node->count << endl;
		for (int j = 0; j < node->data.size(); j++)
		{
			for (int k = 0; k < DATA_SIZE; k++)
				output << " " << hex << uppercase << setw(2) << setfill('0') << node->data[j][k];
			output << endl;
		}
		output << endl;
	}
}
static void pre_process(string filename)
{
	Mode = 1;
	File_path = filename;
	filename_process(filename);
	//跳过平层文件
	if (Floor_height == 1)
		return;
	tree_main();
	floor_main();
	combine();
	_delete();
}
//文件名格式化处理下，从文件名中获取相关信息，命名格式：name_(floor_begin)[_(floor_end)].txt
static void filename_process(string filename)
{
	filename = filename.substr(filename.rfind('\\') + 1, filename.length() - filename.rfind('\\') + 1);
	File_name = filename;
	string name;
	int floor_begin = 0;
	int floor_end = 0;
	for (size_t pos_1 = 0, pos_2 = 0, i = 0; i < 3; i++)
	{
		pos_2 = filename.find('_', pos_1);//查找符号位置
		if (pos_2 == string::npos && i == 1)
		{
			floor_end = floor_begin;
			break;
		}
		string temp = filename.substr(pos_1, pos_2 - pos_1);//截取pos_1,pos_2之间的字符串
		pos_1 = pos_2 + 1;//pos_1指向下次查找的开头
		switch (i)
		{
		case 0:
			name = temp;
			break;
		case 1:
			for (int j = 0; j < temp.length() && temp[j] != '.'; j++)
			{
				floor_begin *= 10;
				floor_begin += temp[j] - '0';
			}
			break;
		case 2:
			for (int j = 0; j < temp.length() && temp[j] != '.'; j++)
			{
				floor_end *= 10;
				floor_end += temp[j] - '0';
			}
			break;
		}
	}
	Floor_begin = floor_begin;
	Floor_end = floor_end;
	Floor_height = abs(floor_end - floor_begin) + 1;
}
static void combine()
{
	vector<_Floor>floor_temp;
	for (floors = floor_first; floors; floors = floors->next)
	{
		floor_temp.push_back(*floors);
		bool same = false;
		int i = 0;
		for (; i < floor_all.size(); i++)
			if (floor_all[i].ID == floors->ID && floor_all[i].byte_pos == floors->byte_pos)
			{
				same = true;
				int num = floor_all[i].pre_size > floors->pre_size ? floor_all[i].pre_size : floors->pre_size;
				for (int j = 0; j < num; j++)
					if (floor_all[i].data[0][j] != floors->data[0][j])
					{
						same = false;
						break;
					}
				if (same)
					break;
			}
		if (same)
		{
			if (floor_all[i].count < floors->count)
				floor_all[i] = *floors;
		}
		else
			floor_all.push_back(*floors);
	}
	floor_each.push_back(floor_temp);
}
static void calculate()
{
	bool same = false;
	vector<vector<_Floor>>::iterator this_file;
	vector<_Floor>::iterator this_node;
	vector<_Floor>::iterator floor_temp;
	for (this_node = floor_all.begin(); this_node != floor_all.end(); this_node++, same = false)
	{
		for (this_file = floor_each.begin(); this_file != floor_each.end(); this_file++)
		{
			for (floor_temp = this_file->begin(); floor_temp != this_file->end(); floor_temp++, same = false)
			{
				//判断ID，字节位置，前置字节数是否相同
				if (this_node->ID < floor_temp->ID)
					continue;
				if (this_node->ID > floor_temp->ID)
					break;
				if (this_node->byte_pos != floor_temp->byte_pos)
					continue;
				if (this_node->pre_size != floor_temp->pre_size)
					continue;

				//ID相同,字节位置相同，前置字节数相同，判断前置字节是否相同
				same = true;
				for (int i = 0; i < this_node->pre_size; i++)
					if (this_node->data[0][i] != floor_temp->data[0][i])
					{
						same = false;
						break;
					}
				//相同，则开始和下一文件统计的信息进行比对
				if (same)
					break;
			}
			//如果数据在该文件不存在，则不放入交集组;存在且已经比对到最后一个文件数据，则放入交集
			if (!same)
				break;
			else
				if ((this_file + 1) == floor_each.end())
				{
					floor_possibility.push_back(*this_node);
					break;
				}
		}
		status.push_back(same);
	}
}
static bool compare(const _Floor& a, const _Floor& b)
{
	if (a.ID == b.ID)
		if (a.pre_size == b.pre_size)
			if (a.byte_pos == b.byte_pos)
				if (a.count == b.count)
				{
					return false;
				}
				else return a.count < b.count;
			else return a.byte_pos < b.byte_pos;
		else return a.pre_size < b.pre_size;
	else return a.ID < b.ID;
}
static void check()
{
	//ifstream;
}