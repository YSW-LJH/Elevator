#include <Tree.h>

Data* data_now = NULL;
Tree_Root* root_now = NULL;
Tree_Root* root_first = NULL;//���ڵ�
File_Data* file_first = NULL;
File_Data* file_now = NULL;
extern bool Len_same;
extern int Window_size;
extern string File_path;

static void data_process(char* buff, int& len);//����һ������
static void ID_update(bool& ID_exis, int data_temp);//����ID��Ϣ
static void DATA_update(bool& data_exis, Data* node_temp);//����������Ϣ
static void tree_process();//������������
static void Window_exis(unsigned int num, int pos);

void tree_delete();//ɾ����

void tree_main()
{
	ifstream file;
	//�����ļ�ԭʼ������
	if (file_first == NULL)
	{
		file_first = new File_Data();
		file_now = file_first;
		file_now->file_path = File_path;
	}
	else
	{
		file_now->next = new File_Data();
		file_now->next->pre = file_now;
		file_now = file_now->next;
		file_now->file_path = File_path;
	}
	//���ļ�
	file.open(File_path, ios::in);
	char buff[SIZE];
	int len = -1;
	while (!file.eof())
	{
		file.getline(buff, SIZE, '\n');
		data_process(buff, len);
	}
	file.close();
	tree_process();
	file_now->tree_root = root_first;
}
void data_process(char* buff, int& len)//����һ������
{
	int flag = 0;        //���ڼ�¼����������ݸ���������ID
	bool ID_exis = false;//�����жϵ�ǰ��ID�Ƿ��Ѿ�����
	bool data_exis = true;//�����жϵ�ǰ�����Ƿ��Ѿ�����
	Data* node_temp = new Data;
	int data[DATA_SIZE + 1];//��¼�������ݣ�����ID
	memset(data, -1, DATA_SIZE + 1);
	format(data, buff, flag);
	//�������Ϊ�գ�����Ϊ���ݴ������
	if (flag == 0)return;
	//�ж��Ƿ���������һ����
	if (Len_same)
		if (len == -1)
			len = flag;
		else if (len != flag)
			Len_same = false;
	//����IDΪ��ID+���ݳ��ȣ�
	data[0] <<= 4;
	data[0] += flag - 1;
	node_temp->size = flag - 1;
	//����ͷ�ڵ�ID��Ϣ
	ID_update(ID_exis, data[0]);
	//�������ݲ���
	for (flag = 1; flag < DATA_SIZE + 1; flag++)
		node_temp->com_data[flag - 1] = data[flag];
	DATA_update(data_exis, node_temp);
	//�ж������Ƿ��Ѵ���
	if (data_exis)
	{
		data_now->count++;
		delete node_temp;
	}
	else
		root_now->total_type++;
	file_now->Total_data_num++;
	file_now->data.push_back(data_now);
}
void ID_update(bool& ID_exis, int data_temp)
{
	//�ж�ID�Ƿ��Ѵ���
	root_now = root_first;
	if (root_first)
		while (1)
		{
			if (data_temp < root_now->ID)
				break;
			if (data_temp == root_now->ID)
			{
				ID_exis = true;
				return;
			}
			if (root_now->next)
				root_now = root_now->next;
			else
				break;
		}
	//��ǰ�ڵ�ҪôΪ�գ�ҪôΪβ�ڵ㣬Ҫôֵ���½ڵ��
	//ID�����ڣ��򴴽��½ڵ㣬����С����˳�����
	if (!ID_exis)
	{
		file_now->ID_num++;
		//�����ǰrootͷ�ڵ�Ϊ�գ������rootͷ�ڵ�
		if (!root_first)
		{
			root_first = new Tree_Root();
			root_now = root_first;
		}

		//�����ǰ�ڵ���β�ڵ㣬���ں��油��������
		else if (root_now->next == NULL)
		{
			root_now->next = new Tree_Root();
			root_now->next->pre = root_now;
			root_now = root_now->next;
		}
		//��ǰ�ڵ��ֵ���½ڵ��ֵ�����ڸýڵ�ǰ�����
		else
		{
			Tree_Root* temp = new Tree_Root();
			if (root_now->pre)
			{
				temp->pre = root_now->pre;
				temp->pre->next = temp;
			}
			else root_first = temp;//�����ǰ�ڵ���ͷ�ڵ㣬�����ͷ�ڵ�
			temp->next = root_now;
			root_now->pre = temp;
			root_now = temp;
		}
		root_now->ID = data_temp;
		root_now->file = file_now;
	}
}
void DATA_update(bool& data_exis, Data* node_temp)
{
	data_now = root_now->child;//����datasָ��
	if (data_now == NULL)//���ͷָ��Ϊ�գ��������ͷ�ڵ�
	{
		data_exis = false;
		root_now->child = node_temp;
		data_now = node_temp;
		data_now->ID = root_now;
		return;
	}
	int flag = 0;
	Data* pos_1 = data_now, * pos_2 = NULL;
	for (int i = 0; i < DATA_SIZE;)
	{
		if (node_temp->com_data[i] < pos_1->com_data[i])
		{
			data_exis = false;
			//�ж��Ƿ�������ͷ�ڵ����
			if (pos_1->pre)
			{
				//�����м����
				node_temp->pre = pos_1->pre;
				pos_1->pre->next = node_temp;
			}
			else
				root_now->child = node_temp;//��ͷ�ڵ���룬����root������ָ��
			node_temp->next = pos_1;
			pos_1->pre = node_temp;
			pos_1 = pos_1->pre;
			break;
		}
		if (node_temp->com_data[i] > pos_1->com_data[i])
		{
			//�ж��Ƿ�����β������
			if (pos_1->next == NULL)
			{
				data_exis = false;
				node_temp->pre = pos_1;
				pos_1->next = node_temp;
				pos_1 = pos_1->next;
				break;
			}
			if (pos_1 == pos_2)
			{
				data_exis = false;
				pos_1->next->pre = node_temp;
				node_temp->next = pos_1->next;
				node_temp->pre = pos_1;
				pos_1->next = node_temp;
				pos_1 = pos_1->next;
				break;
			}
			pos_1 = pos_1->next;
		}
		if (node_temp->com_data[i] == pos_1->com_data[i])
		{
			for (; i < DATA_SIZE;)
				if (node_temp->com_data[i + 1] == pos_1->com_data[i + 1])
					i++;
				else 
					break;

			for (pos_2 = pos_1; pos_2->next; pos_2 = pos_2->next)
				if (pos_2->next->com_data[i] != pos_1->com_data[i])
					break;
			i++;
		}
	}
	data_now = pos_1;
	data_now->ID = root_now;
}
//������������
void tree_process()
{
	root_now = root_first;
	for (; root_now; root_now = root_now->next)
	{
		//����λ����ͬ��ȥ��ID�������λ
		if (Len_same)
			root_now->ID >>= 4;
		for (int i = 0; i < DATA_SIZE; i++)
		{
			root_now->Window[i] = new unsigned int[root_now->total_type];
			for (int j = 0; j < root_now->total_type; j++)
				root_now->Window[i][j] = 0xF0000000;
		}
		data_now = root_now->child;
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (; data_now; data_now = data_now->next)
		{
			if (pre_same == false)
			{
				root_now->pre_data_type++;
				for (int i = 0; i < PRE_SIZE; i++)
					pre_data[i] = data_now->com_data[i];
				num = 1;
				pre_same = true;
			}
			else
				num++;
			data_now->pos = num;//��¼ÿ�������������е�λ��,��ǰ�����ֽڷֿ�
			//����һ�ڵ�ȶ�����
			if (data_now->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (data_now->next->com_data[i] != pre_data[i])
						pre_same = false;
			//�ж�ͬһλ�����ݵ���������ͳ�ƻ�������
			//temp�洢��Ӧ�����ֽڵĺϲ������ĵ�����Ȼ��ͳ��temp��������
			if (Window_size != -1)
				for (int i = 0; i <= data_now->size - Window_size; i++)
				{
					unsigned int temp = 0;
					for (int j = 0; j < Window_size; j++)
					{
						temp <<= 8;
						temp += data_now->com_data[i + j];
					}
					Window_exis(temp, i);
				}
			//���㵱ǰ������ǰһ�����ݵĶ�Ӧ�ֽ�λ�Ĳ�ֵ
			if (data_now->pre)
				for (int i = 0; i < DATA_SIZE; i++)
					data_now->_pre[i] = data_now->com_data[i] - data_now->pre->com_data[i];
		}
	}
}
//��鵱ǰ�����Ƿ��Ѿ���¼��û��¼���������
void Window_exis(unsigned int num, int pos)
{
	int i = 0;
	for (; i <= root_now->total_type - Window_size && root_now->Window[pos][i] != 0xF0000000; i++)
		if (num == root_now->Window[pos][i])
			return;
	root_now->Window[pos][i] = num;
	root_now->Window_count[pos]++;
	return;
}
//ɾ����
void tree_delete()
{
	file_now = file_first;
	while (file_now)
	{
		root_now = file_now->tree_root;
		while (root_now)
		{
			data_now = root_now->child;
			for (int i = 0; i < DATA_SIZE; i++)
			{
				delete[] root_now->Window[i];
			}
			while (data_now)
			{
				Data* datas_temp = data_now;
				data_now = data_now->next;
				delete datas_temp;
			}
			Tree_Root* roots_temp = root_now;
			root_now = root_now->next;
			delete roots_temp;
		}
		File_Data* file_temp = file_now;
		file_now = file_now->next;
		delete file_temp;
	}
	root_now = NULL;
	root_first = NULL;
	data_now = NULL;
	file_first = NULL;
	file_now = NULL;
}