#include <door.h>

Door* doors = NULL;
Door* door_first = NULL;
char rule[4] = { 'A','B','C','D' };

extern Tree_Root* roots;
extern Data* datas;
extern Tree_Root* root_first;
extern string File_path;

static void pre_process();
static void data_replace();//建立树
static void replace_help(char* buff);
static void str_combine();//相同数据合并

void door_delete();

void door_main()
{
	pre_process();
	data_replace();
	//str_combine();
}
static void pre_process()
{
	doors = new Door();
	door_first = doors;
	Door* pre_door = NULL;
	for (roots = root_first; roots; roots = roots->next)
		if (roots->total_type <= TYPE_NUM)
		{
			if (doors == NULL)
				doors = new Door();
			if (pre_door)
			{
				pre_door->next = doors;
				doors->pre = pre_door;
			}
			doors->root = roots;
			pre_door = doors;
			doors = doors->next;
		}
}
static void data_replace()
{
	ifstream source;
	source.open(File_path);
	char buff[SIZE] = { 0 };
	while (!source.eof())
	{
		source.getline(buff, SIZE, '\n');
		replace_help(buff);
	}
	source.close();
}
//先把一行数据整理为一个数组
//统计数据出现的规律
static void replace_help(char* buff)
{
	int flag = 0;
	int data[DATA_SIZE + 1];
	memset(data, -1, DATA_SIZE + 1);
	format(data, buff, flag);
	//更新ID为（ID+数据长度）
	data[0] <<= 4;
	data[0] += flag - 1;
	//根据ID判断数据位置
	int pos = 0;
	bool same = true;
	doors = door_first;
	while (1)
		if (doors == NULL)
			return;
		else if (doors->root->ID != data[0])
			doors = doors->next;
		else
			break;
	//进一步查找数据位置
	for (datas = doors->root->child; datas; datas = datas->next, pos++, same = true)
	{
		for (int i = 0; i < DATA_SIZE; i++)
			if (datas->com_data[i] != data[i + 1])
			{
				same = false;
				break;
			}
		if (same)
			break;
	}
	doors->data_sequence += rule[pos];
}
static void str_combine()
{
	doors = door_first;
	string sequence = "";
	char onechar = '\0';
	for (; doors; doors = doors->next)
	{
		size_t len = doors->data_sequence.length();
		for(int i=0;i<len;i++)
			if (onechar != doors->data_sequence[i])
			{
				onechar = doors->data_sequence[i];
				sequence += onechar;
			}
		doors->data_sequence = sequence;
	}
}
void door_delete()
{
	doors = door_first;
	while (doors)
	{
		Door* temp = doors;
		doors = doors->next;
		delete temp;
	}
	doors = NULL;
	door_first = NULL;
}
