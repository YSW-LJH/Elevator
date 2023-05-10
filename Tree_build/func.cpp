#include <func.h>

extern Data* data_now;
extern Data_Root* root_first;
extern Data_Root* root_now;
extern File_Data* file_first;
extern File_Data* file_now;


void format(int* data, char* buff, int& flag);//十六进制字符转换为数字
void getFiles(string path, vector<string>& files);//获取路径下所有*.txt文件
static void print(string& filename);//输出函数
static void print_a(string filename);
static void print_b(string filename);
static void print_c(string filename);
static void file_process(string filename);//文件处理
void _delete();

map<string, string> call(string path)
{
	string out_path = path + "\\out";
	string cmd = "mkdir " + out_path;
	system(cmd.c_str());

	map<string, string>result;
	vector<string>files;
	getFiles(path, files);
	for (auto file : files)
	{
		tree_main(file);
		floor_main();
		floor_verify();
		floor_compare_main();
		door_main();
		Data_Restore(out_path + file.substr(file.rfind('\\'), file.length() - file.rfind('\\')));
	}
	result = file_first->Signal;
	//print(filename);
	_delete();
	return result;
}
void format(int* data, char* buff, int& flag)//将一行数据转换为数组
{
	for (int i = 0; i < SIZE && buff[i] != 0; )
	{
		if (buff[i] == ' ')
		{
			i++;
			continue;
		}
		int data_temp = 0;
		for (; buff[i] != ' ' && buff[i] != 0; i++)//把字符格式转换为数字
		{
			data_temp <<= 4;
			if (buff[i] <= '9')
				data_temp += buff[i] - '0';
			else if (buff[i] <= 'Z')
				data_temp += buff[i] - 'A' + 10;
			else
				data_temp += buff[i] - 'a' + 10;
		}
		data[flag] = data_temp;
		flag++;
		//如果已经完成处理定义的数据量大小，则停止
		if (flag == DATA_SIZE + 1)
			break;
	}
}
void getFiles(string path, vector<string>& files)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.txt").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之 //如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				//if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				//{
				//	getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				//}
				continue;
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
static void print(string& filename)
{
	file_now = file_first;
	size_t pos = filename.find_last_of('.');
	if (pos >= 0)
	{
		filename = filename.substr(0, pos) + "_out" + filename.substr(pos);
	}
	else
		filename += "_out";
	print_a(filename + "_temp_a");
	print_b(filename + "_temp_b");
	print_c(filename + "_temp_c");
	file_process(filename);
}
static void print_a(string filename)
{
	ofstream a;
	a.open(filename, ios::out);
	a << "ID_num:" << root_first->file->ID_num << endl << endl;
	for (root_now = root_first; root_now; root_now = root_now->next)//从ID根节点依次输出
	{
		data_now = root_now->child;
		a << "ID:" << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << root_now->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << root_now->total_type;
		a << endl;
	}
	//打印门信号相关信息
	//if (Mode == 0 || Mode == 2)
	//{
	//	a << "*******门信号（未完成）*******\n";
	//	for (door_now = door_first; door_now; door_now = door_now->next)
	//	{
	//		a << "\nID:" << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << door_now->root->ID << endl;
	//		for (int i = 0; i < door_now->data_sequence.length(); i += 10)
	//			a << "	" << door_now->data_sequence.substr(i, 10) << endl;
	//	}
	//}
	//打印楼层信号相关信息
	//if (Mode == 0 || Mode == 3)
	//{
	//	a << "********楼层信号********\n";
	//	for (floors = floor_first; floors; floors = floors->next)
	//	{
	//		a << "\nID:" << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << floors->ID << endl;
	//		a << "数据量：" << dec << floors->count << " 表示楼层的字节位：" << floors->byte_pos << " 可能的数据定义字节数：" << floors->pre_size << endl;
	//		for (int i = 0; i < floors->count; i++)
	//		{
	//			for (int j = 0; j < DATA_SIZE && floors->data[i][j] != -1; j++)
	//				a << " " << hex << uppercase << setw(2) << setfill('0') << floors->data[i][j];
	//			a << endl;
	//		}
	//		a << "**********" << endl;
	//	}
	//}
	a.close();
}
static void print_b(string filename)
{
	ofstream b;
	b.open(filename, ios::out);
	for (root_now = root_first; root_now; root_now = root_now->next)//从ID根节点依次输出
	{
		data_now = root_now->child;
		b << "ID:" << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << root_now->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << root_now->total_type << endl;
		for (; data_now; data_now = data_now->next)
		{
			if ((data_now->next == NULL) || (data_now->next && data_now->next->pos == 1))
			{
				b << "Pre_data:";
				for (int i = 0; i < PRE_SIZE; i++)
					b << hex << uppercase << setw(2) << setfill('0') << data_now->com_data[i] << ' ';
				b << "	type:" << dec << setw(2) << setfill('0') << data_now->pos << endl;
			}
		}
		b << endl;
	}
	b.close();
}
static void print_c(string filename)
{
	ofstream c;
	c.open(filename, ios::out);
	for (root_now = root_first; root_now; root_now = root_now->next)//从ID根节点依次输出
	{
		data_now = root_now->child;
		c << "**********************************************" << endl;
		c << "ID:" << hex << uppercase << setw(3 + !file_now->Len_same) << setfill('0') << root_now->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << root_now->total_type << endl << endl;
		for (; data_now; data_now = data_now->next)
		{
			c << dec << setw(2) << setfill('0') << data_now->pos << ": ";
			for (int i = PRE_SIZE; i < data_now->size; i++)
				c << hex << uppercase << setw(2) << setfill('0') << data_now->com_data[i] << ' ';
			//c << datas->size;
			c << endl;
			if ((data_now->next == NULL) || (data_now->next && data_now->next->pos == 1))
			{
				c << "Pre_data:";
				for (int i = 0; i < PRE_SIZE; i++)
					c << hex << uppercase << setw(2) << setfill('0') << data_now->com_data[i] << ' ';
				c << "	type:" << dec << setw(2) << setfill('0') << data_now->pos << endl << endl;
			}
		}
	}
	c.close();
}
static void file_process(string filename)
{
	ofstream out;
	ifstream a;
	ifstream b;
	ifstream c;
	char buff[SIZE];
	out.open(filename, ios::out);
	a.open(filename + "_temp_a", ios::in);
	b.open(filename + "_temp_b", ios::in);
	c.open(filename + "_temp_c", ios::in);
	out << "--------------------ID信息--------------------" << endl << endl;
	while (!a.eof())
	{
		a.getline(buff, SIZE, '\n');
		out << buff << endl;
	}
	out << "------------------ID和头两个字节------------------" << endl << endl;
	while (!b.eof())
	{
		b.getline(buff, SIZE, '\n');
		out << buff << endl;
	}
	out << "-------------ID，头两个字节以及其他信息------------" << endl << endl;
	while (!c.eof())
	{
		c.getline(buff, SIZE, '\n');
		out << buff << endl;
	}
	out.close();
	a.close();
	b.close();
	c.close();
	string cmd_0 = "del /a/f/q " + filename + "_temp_a";
	string cmd_1 = "del /a/f/q " + filename + "_temp_b";
	string cmd_2 = "del /a/f/q " + filename + "_temp_c";
	system(cmd_0.c_str());
	system(cmd_1.c_str());
	system(cmd_2.c_str());
}
void _delete()
{
	tree_delete();
	//door_delete();
}