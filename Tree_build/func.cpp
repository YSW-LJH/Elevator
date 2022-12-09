#include <func.h>

int Mode = -1;
int Floor_height = 0;
int Floor_begin = 0;
int Floor_end = 0;
bool Len_same = true;
int Window_size = -1;
string File_path;
string File_name;

extern Tree_Root* roots;
extern Data* datas;
extern Tree_Root* root_first;
extern int ID_num;

extern Door* doors;
extern Door* door_first;

extern Floor* floors;
extern Floor* floor_first;

void format(int* data, char* buff, int& flag);//ʮ�������ַ�ת��Ϊ����
void getFiles(string path, vector<string>& files);//��ȡ·��������*.txt�ļ�
static void print(string& filename);//�������
static void print_a(string filename);
static void print_b(string filename);
static void print_c(string filename);
static void file_process(string filename);//�ļ�����
void _delete();

void call(string filename, int mode, int height, int w_size)
{
	filename += ".txt";
	File_path = filename;
	Mode = mode;
	Floor_height = height;
	Window_size = w_size;
	tree_main();
	if (root_first == NULL)
		return;
	if (Mode == 0 || Mode == 2)
		door_main();
	if (Mode == 0 || Mode == 3)
		floor_main();
	print(filename);
	cout << "�������(��'v'��),����ļ���Ϊ:" << filename << endl;
	_delete();
}
void format(int* data, char* buff, int& flag)//��һ������ת��Ϊ����
{
	for (int i = 0; i < SIZE && buff[i] != 0; )
	{
		if (buff[i] == ' ')
		{
			i++;
			continue;
		}
		int data_temp = 0;
		for (; buff[i] != ' ' && buff[i] != 0; i++)//���ַ���ʽת��Ϊ����
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
		//����Ѿ���ɴ��������������С����ֹͣ
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
			//�����Ŀ¼,����֮ //�������,�����б�
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
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
	a << "ID_num:" << ID_num << endl << endl;
	for (roots = root_first; roots; roots = roots->next)//��ID���ڵ��������
	{
		datas = roots->child;
		a << "ID:" << hex << uppercase << setw(3 + !Len_same) << setfill('0') << roots->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << roots->total_type;
		for (int i = 0; i < DATA_SIZE; i++)
			if (roots->Window_count[i] > 0)
				a << "	Window " << dec << i + 1 << " type:" << roots->Window_count[i];
		a << endl;
	}
	//��ӡ���ź������Ϣ
	if (Mode == 0 || Mode == 2)
	{
		a << "*******���źţ�δ��ɣ�*******\n";
		for (doors = door_first; doors; doors = doors->next)
		{
			a << "\nID:" << hex << uppercase << setw(3 + !Len_same) << setfill('0') << doors->root->ID << endl;
			for (int i = 0; i < doors->data_sequence.length(); i += 10)
				a << "	" << doors->data_sequence.substr(i, 10) << endl;
		}
	}
	//��ӡ¥���ź������Ϣ
	if (Mode == 0 || Mode == 3)
	{
		a << "********¥���ź�********\n";
		for (floors = floor_first; floors; floors = floors->next)
		{
			a << "\nID:" << hex << uppercase << setw(3 + !Len_same) << setfill('0') << floors->ID << endl;
			a << "��������" << dec << floors->count << " ��ʾ¥����ֽ�λ��" << floors->byte_pos << " ���ܵ����ݶ����ֽ�����" << floors->pre_size << endl;
			for (int i = 0; i < floors->count; i++)
			{
				for (int j = 0; j < DATA_SIZE && floors->data[i][j] != -1; j++)
					a << " " << hex << uppercase << setw(2) << setfill('0') << floors->data[i][j];
				a << endl;
			}
			a << "**********" << endl;
		}
	}
	a.close();
}
static void print_b(string filename)
{
	ofstream b;
	b.open(filename, ios::out);
	for (roots = root_first; roots; roots = roots->next)//��ID���ڵ��������
	{
		datas = roots->child;
		b << "ID:" << hex << uppercase << setw(3 + !Len_same) << setfill('0') << roots->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << roots->total_type << endl;
		for (; datas; datas = datas->next)
		{
			if ((datas->next == NULL) || (datas->next && datas->next->pos == 1))
			{
				b << "Pre_data:";
				for (int i = 0; i < PRE_SIZE; i++)
					b << hex << uppercase << setw(2) << setfill('0') << datas->com_data[i] << ' ';
				b << "	type:" << dec << setw(2) << setfill('0') << datas->pos << endl;
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
	for (roots = root_first; roots; roots = roots->next)//��ID���ڵ��������
	{
		datas = roots->child;
		c << "**********************************************" << endl;
		c << "ID:" << hex << uppercase << setw(3 + !Len_same) << setfill('0') << roots->ID\
			<< "	type:" << dec << setw(2) << setfill('0') << roots->total_type << endl << endl;
		for (; datas; datas = datas->next)
		{
			c << dec << setw(2) << setfill('0') << datas->pos << ": ";
			for (int i = PRE_SIZE; i < datas->size; i++)
				c << hex << uppercase << setw(2) << setfill('0') << datas->com_data[i] << ' ';
			//c << datas->size;
			c << endl;
			if ((datas->next == NULL) || (datas->next && datas->next->pos == 1))
			{
				c << "Pre_data:";
				for (int i = 0; i < PRE_SIZE; i++)
					c << hex << uppercase << setw(2) << setfill('0') << datas->com_data[i] << ' ';
				c << "	type:" << dec << setw(2) << setfill('0') << datas->pos << endl << endl;
			}
		}
		//���������Ϣͳ��
		if (Mode == 0 || Mode == 4)
		{
			c << "\n***����ͳ����Ϣ***" << endl;
			for (int i = 0; i < DATA_SIZE; i++)
			{
				if (roots->Window_count[i] > 0)
					c << "Window " << dec << i + 1 << " type:" << roots->Window_count[i];
				for (int j = 0; j < roots->Window_count[i]; j++)
				{
					if (j % 10 == 0)
						c << endl;
					c << " " << hex << uppercase << setw(2 * Window_size) << setfill('0') << roots->Window[i][j];
				}
				c << endl;
			}
		}
		c << endl;
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
	out << "--------------------ID��Ϣ--------------------" << endl << endl;
	while (!a.eof())
	{
		a.getline(buff, SIZE, '\n');
		out << buff << endl;
	}
	out << "------------------ID��ͷ�����ֽ�------------------" << endl << endl;
	while (!b.eof())
	{
		b.getline(buff, SIZE, '\n');
		out << buff << endl;
	}
	out << "-------------ID��ͷ�����ֽ��Լ�������Ϣ------------" << endl << endl;
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
	door_delete();
	floor_delete();
	Mode = -1;
	Floor_height = 0;
	Floor_begin = 0;
	Floor_end = 0;
	Len_same = true;
	Window_size = -1;
}