#include <Tree.h>

Tree_Root* roots = NULL;
Data* datas = NULL;
Tree_Root* root_first = NULL;
int ID_num = 0;
int Total_data_num = 0;

extern bool Len_same;
extern int Window_size;
extern string File_path;

static void data_process(char* buff, int& len);//����һ������
static void ID_update(bool& ID_exis, int data_temp);//����ID��Ϣ
static void DATA_update(bool& data_exis, int flag, Data* node_temp, int data_temp);//����������Ϣ
static void tree_process();//������������
static void Window_exis(unsigned int num, int pos);

void tree_delete();//ɾ����

void tree_main()
{
	ifstream file;
	file.open(File_path, ios::in);
	if (!file)
	{
		cout << "���ļ�\"" << File_path << "\"ʧ��(���n��), �����ļ��Ƿ����" << endl;
		return;
	}
	char buff[SIZE];
	int len = -1;
	while (!file.eof())
	{
		file.getline(buff, SIZE, '\n');
		data_process(buff, len);
	}
	file.close();
	tree_process();
}
void data_process(char* buff, int& len)//����һ������
{
	int flag = 0;//���ڼ�¼������еڼ�������
	bool ID_exis = false;//�����жϵ�ǰ��ID�Ƿ��Ѿ�����
	bool data_exis = true;//�����жϵ�ǰ�����Ƿ��Ѿ�����
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];
	memset(data, -1, DATA_SIZE + 1);
	format(data, buff, flag);
	if (flag == 0)return;
	//�ж��Ƿ���������һ����
	if (Len_same)
		if (len == -1)
			len = flag - 1;
		else if (len != flag - 1)
			Len_same = false;
	//����IDΪ��ID+���ݳ��ȣ�
	data[0] <<= 4;
	data[0] += flag - 1;
	node_temp->size = flag - 1;
	//����ͷ�ڵ�ID��Ϣ
	ID_update(ID_exis, data[0]);
	//�жϸ�����Ϊ���ݲ���
	for (flag = 1; flag < DATA_SIZE + 1; flag++)
			DATA_update(data_exis, flag, node_temp, data[flag]);
	if (data_exis)
		//�ж�Ϊ��ͬ����
		{
			datas->count++;
			delete node_temp;
		}
	else
		roots->total_type++;
	Total_data_num++;
}
void ID_update(bool& ID_exis, int data_temp)
{
	//�ж�ID�Ƿ��Ѵ���
	for (Tree_Root* temp = root_first; temp; temp = temp->next)
	{
		if (data_temp < temp->ID)
			break;
		if (data_temp == temp->ID)
		{
			ID_exis = true;
			roots = temp;//������ڣ���rootsָ��ǰ����ID�ĸ��ڵ�
			break;
		}
	}
	//ID�����ڣ��򴴽��½ڵ㣬����С����˳�����
	if (!ID_exis)
	{
		roots = root_first;
		while (1)
		{
			//�����ǰrootͷ�ڵ�Ϊ�գ������rootͷ�ڵ�
			if (!roots)
			{
				roots = new Tree_Root();
				root_first = roots;
				break;
			}
			if (roots->ID < data_temp)
			{
				//�����ǰ�ڵ���β�ڵ㣬���ں��油��������
				if (roots->next == NULL)
				{
					roots->next = new Tree_Root();
					roots->next->pre = roots;
					roots = roots->next;
					break;
				}
				roots = roots->next;
			}
			else
			{
				Tree_Root* temp = new Tree_Root();
				if (roots->pre)
				{
					temp->pre = roots->pre;
					temp->pre->next = temp;
				}
				else root_first = temp;//�����ǰ�ڵ���ͷ�ڵ㣬�����ͷ�ڵ�
				temp->next = roots;
				roots->pre = temp;
				roots = temp;
				break;
			}
		}
		roots->ID = data_temp;
	}
	datas = roots->child;//����datasָ��
	roots->count++;
}
void DATA_update(bool& data_exis, int flag, Data* node_temp, int data_temp)
{
	node_temp->com_data[flag - 1] = data_temp;
	if (datas == NULL)//���ͷָ��Ϊ�գ��������ͷ�ڵ�
	{
		data_exis = false;
		roots->child = node_temp;
	}
	if (data_exis)
		while (1)
			//�ж����ݲ���λ��
			if (node_temp->com_data[flag - 1] < datas->com_data[flag - 1])
			{
				data_exis = false;
				//�ж��Ƿ�������ͷ�ڵ����
				if (datas->pre)
				{
					//�����м����
					node_temp->pre = datas->pre;
					node_temp->pre->next = node_temp;
				}
				else
					roots->child = node_temp;//��ͷ�ڵ���룬����root������ָ��
				node_temp->next = datas;
				datas->pre = node_temp;
				break;
			}
			else if (node_temp->com_data[flag - 1] > datas->com_data[flag - 1])
			{
				if (datas->next)
				{
					datas = datas->next;
					bool pre_same = true;
					//�ж�ǰ��������Ƿ���ͬ
					for (int j = 0; j < flag - 1; j++)
						if (node_temp->com_data[j] != datas->com_data[j])
						{
							pre_same = false;
							break;
						}
					//���ǰ�������ݲ�ͬ��������ڴ˴�
					if (!pre_same)
					{
						data_exis = false;
						node_temp->pre = datas->pre;
						node_temp->pre->next = node_temp;
						node_temp->next = datas;
						datas->pre = node_temp;
						break;
					}
				}
				else
				{
					//����β������
					data_exis = false;
					datas->next = node_temp;
					node_temp->pre = datas;
					break;
				}
			}
			else
				break;
}
//������������
void tree_process()
{
	roots = root_first;
	for (; roots; roots = roots->next)
	{
		//����λ����ͬ��ȥ��ID�������λ
		if (Len_same)
			roots->ID >>= 4;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			roots->Window[i] = new unsigned int[roots->total_type];
			for (int j = 0; j < roots->total_type; j++)
				roots->Window[i][j] = 0xF0000000;
		}
		ID_num++;
		datas = roots->child;
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (; datas; datas = datas->next)
		{
			if (pre_same == false)
			{
				roots->pre_data_type++;
				for (int i = 0; i < PRE_SIZE; i++)
					pre_data[i] = datas->com_data[i];
				num = 1;
				pre_same = true;
			}
			else
				num++;
			datas->pos = num;//��¼ÿ�������������е�λ��
			//����һ�ڵ�ȶ�����
			if (datas->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (datas->next->com_data[i] != pre_data[i])
						pre_same = false;
			//�ж�ͬһλ�����ݵ���������ͳ�ƻ�������
			if (Window_size != -1)
				for (int i = 0; i <= datas->size - Window_size; i++)
				{
					unsigned int temp = 0;
					for (int j = 0; j < Window_size; j++)
					{
						temp <<= 8;
						temp += datas->com_data[i + j];
					}
					Window_exis(temp, i);
				}
			//���㵱ǰ������ǰһ�����ݵĶ�Ӧ�ֽ�λ�Ĳ�ֵ
			if (datas->pre)
				for (int i = 0; i < DATA_SIZE; i++)
					datas->_pre[i] = datas->com_data[i] - datas->pre->com_data[i];
		}
	}
}
//��鵱ǰ�����Ƿ��Ѿ���¼��û��¼���������
void Window_exis(unsigned int num,int pos)
{
	int i = 0;
	for (; i <= roots->total_type - Window_size && roots->Window[pos][i] != 0xF0000000; i++)
		if (num == roots->Window[pos][i])
			return;
	roots->Window[pos][i] = num;
	roots->Window_count[pos]++;
	return;
}
//ɾ����
void tree_delete()
{
	roots = root_first;
	while (roots)
	{
		datas = roots->child;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			delete[] roots->Window[i];
		}
		while (datas)
		{
			Data* datas_temp = datas;
			datas = datas->next;
			delete datas_temp;
		}
		Tree_Root* roots_temp = roots;
		roots = roots->next;
		delete roots_temp;
	}

	roots = NULL;
	root_first = NULL;
	datas = NULL;
	ID_num = 0;
	Total_data_num = 0;
}