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

vector<vector<_Floor>>floor_each;//����ÿһ���ļ�������
vector<_Floor>floor_all;//��¼����
vector<bool>status;//��¼������ÿ�����ݵ�״̬
vector<_Floor>floor_possibility;//��¼����
//vector<_Floor>floor_difference;//��¼ƽ���ļ��������ļ��Ĳ
ostringstream output;//��¼�������ַ���

static void floor_calculate_main();
static void pre_process(string filename);//Ԥ�������
static void combine();//�󲢼�
static void filename_process(string filename);//�ļ������ݴ���
static bool compare(const _Floor& a, const _Floor& b);//�����ñȽϺ���
static void calculate();//�󽻼�
static void check();

//���ݶ�������ļ�������ܵ�¥������
string path = "D:\\Code\\Elevator\\����\\ZY-data\\����ģʽ";

extern void getFiles(string path, vector<string>& files);

/*�ļ����������ļ���_��ʼ¥��_����¥��.txt
* �磺����_1_4.txt����ʱ��_3_1.txt��123_1_55.txt
* �����ƽ����Ϣ����������Ϊ������_1.txt �� ����_1_1.txt ������ʼ¥��ͽ���¥����Խ�дһ��
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
	//��ӡ
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
	//����ƽ���ļ�
	if (Floor_height == 1)
		return;
	tree_main();
	floor_main();
	combine();
	_delete();
}
//�ļ�����ʽ�������£����ļ����л�ȡ�����Ϣ��������ʽ��name_(floor_begin)[_(floor_end)].txt
static void filename_process(string filename)
{
	filename = filename.substr(filename.rfind('\\') + 1, filename.length() - filename.rfind('\\') + 1);
	File_name = filename;
	string name;
	int floor_begin = 0;
	int floor_end = 0;
	for (size_t pos_1 = 0, pos_2 = 0, i = 0; i < 3; i++)
	{
		pos_2 = filename.find('_', pos_1);//���ҷ���λ��
		if (pos_2 == string::npos && i == 1)
		{
			floor_end = floor_begin;
			break;
		}
		string temp = filename.substr(pos_1, pos_2 - pos_1);//��ȡpos_1,pos_2֮����ַ���
		pos_1 = pos_2 + 1;//pos_1ָ���´β��ҵĿ�ͷ
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
				//�ж�ID���ֽ�λ�ã�ǰ���ֽ����Ƿ���ͬ
				if (this_node->ID < floor_temp->ID)
					continue;
				if (this_node->ID > floor_temp->ID)
					break;
				if (this_node->byte_pos != floor_temp->byte_pos)
					continue;
				if (this_node->pre_size != floor_temp->pre_size)
					continue;

				//ID��ͬ,�ֽ�λ����ͬ��ǰ���ֽ�����ͬ���ж�ǰ���ֽ��Ƿ���ͬ
				same = true;
				for (int i = 0; i < this_node->pre_size; i++)
					if (this_node->data[0][i] != floor_temp->data[0][i])
					{
						same = false;
						break;
					}
				//��ͬ����ʼ����һ�ļ�ͳ�Ƶ���Ϣ���бȶ�
				if (same)
					break;
			}
			//��������ڸ��ļ������ڣ��򲻷��뽻����;�������Ѿ��ȶԵ����һ���ļ����ݣ�����뽻��
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