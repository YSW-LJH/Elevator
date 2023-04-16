#include <Tree.h>

Data* data_now = NULL;
Tree_Root* root_now = NULL;
Tree_Root* root_first = NULL;//���ڵ�
File_Data* file_first = NULL;
File_Data* file_now = NULL;

extern bool Len_same;
extern string File_path;
extern int Floor_height;
extern int Floor_begin;
extern int Floor_end;

static void data_process(char* buff, int& len);//����һ������
static void ID_update(bool& ID_exis, int data_temp);//����ID��Ϣ
static void DATA_update(bool& data_exis, Data* node_temp);//����������Ϣ
void Data_Restore(string out_path);
static void tree_update();//������������

void tree_delete();    //ɾ����

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
	tree_update();
	file_now->tree_root = root_first;
	file_now->Floor_height = Floor_height;
	file_now->Floor_begin = Floor_begin;
	file_now->Floor_end = Floor_end;

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
	if (flag == 0)
		return;
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
	{
		root_now->total_type++;
	}
	root_now->count++;
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
	file_now->ID_num++;
	//�����ǰrootͷ�ڵ�Ϊ�գ������rootͷ�ڵ�
	if (!root_first)
	{
		root_first = new Tree_Root();
		root_now = root_first;
	}

	//�����ǰ�ڵ���β�ڵ㣬���ں��油��������
	else if (root_now->next == NULL && data_temp > root_now->ID)
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
	//���½ڵ��λ��������pos_1��pos_2֮�䣬ȷ������λ��׼ȷ
	Data* pos_1 = data_now, * pos_2 = NULL;

	for (int i = 0; i < DATA_SIZE;)
	{
		//--�½ڵ�С�ڵ�ǰ�ڵ㣬�ڵ�ǰ�ڵ�ǰ����
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
		//--�½ڵ���ڵ�ǰ�ڵ㣬
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
		//--�½ڵ���ڵ�ǰ�ڵ�
		if (node_temp->com_data[i] == pos_1->com_data[i])
		{
			for (i++; i < DATA_SIZE;)
				if (node_temp->com_data[i] == pos_1->com_data[i])
					i++;
				else 
					break;
			for (pos_2 = pos_1; pos_2->next; pos_2 = pos_2->next)
			{
				int j = 0;
				for (; j < i; j++)
					if (pos_2->next->com_data[j] != pos_1->com_data[j])
						break;
				if (j != i)
					break;
			}
		}
	}
	data_now = pos_1;
	data_now->ID = root_now;
}
//������������
void tree_update()
{
	root_now = root_first;
	for (; root_now; root_now = root_now->next)
	{
		//����λ����ͬ��ȥ��ID�������λ
		if (Len_same)
			root_now->ID >>= 4;
		//ͳ��ÿλʮ���������ĳ��ִ���
		int count[DATA_SIZE * 2][16];
		memset(count, 0, sizeof(count));
		int pre_data[PRE_SIZE] = { 0 };
		int num = 1;
		bool pre_same = false;
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			//��¼ÿ�������������е�λ��,��ǰ�����ֽڷֿ�
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
			data_now->pos = num;
			//����һ�ڵ�ȶ�����
			if (data_now->next)
				for (int i = 0; i < PRE_SIZE; i++)
					if (data_now->next->com_data[i] != pre_data[i])
						pre_same = false;
			//ͳ��ÿ����ֽ���ͬ���ݵĳ��ִ���
			for (int i = 0; i < DATA_SIZE && data_now->com_data[i] != -1; i++)
			{
				count[i * 2][data_now->com_data[i] >> 4] += data_now->count;
				count[i * 2 + 1][data_now->com_data[i] % 16] += data_now->count;
			}
		}
		//�������ֵ
		for (data_now = root_now->child; data_now; data_now = data_now->next)
		{
			for (int i = 0; i < DATA_SIZE && data_now->com_data[i] != -1; i++)
			{
				data_now->percent[i][0] = static_cast<double>(root_now->count) / count[i * 2][data_now->com_data[i] >> 4] - 1;
				data_now->percent[i][1] = static_cast<double>(root_now->count) / count[i * 2 + 1][data_now->com_data[i] % 16] - 1;
				data_now->percent_sum += data_now->percent[i][0] + data_now->percent[i][1];
			}
		}
	}
}
//�����ݻ�ԭ��ԭʼ�ļ���˳��
void Data_Restore(string out_path)
{
	ofstream out;
	out.open(out_path, ios::trunc);
	for(file_now=file_first;file_now;file_now=file_now->next)
		for (auto data : file_now->data)
		{
			out << hex << uppercase << setw(3 + !Len_same) << setfill('0') << data->ID->ID << "  ";
			for (int i = 0; i < DATA_SIZE; i++)
				if (i < data->size)
					out << hex << uppercase << setw(2) << setfill('0') << data->com_data[i] << " ";
					//out << hex << uppercase << data->com_data[i] / 16 << " " << data->com_data[i] % 16 << " ";
				else
					out << "-1" << " ";
			//��ǩ��ӡ
			//if (data->tag_all.size() > 0)
			//	for (auto i : data->tag_all)
			//		if (i != 0)
			//			out << dec << i << " ";
			//���ʴ�ӡ
			//out << endl << " ";
			//for (int i = 0; i < DATA_SIZE; i++)
			//	out << dec << data->percent[i][0] << " " << data->percent[i][1] << " ";
			if(data->status[0]!=0)
			out << dec << data->status[0];
			out << endl;
		}
	//�����ݴ�ӡ
	//for (root_now = root_first; root_now; root_now = root_now->next)
	//	for (data_now = root_now->child; data_now; data_now = data_now->next)
	//	{
	//		out << hex << uppercase << setw(3) << setfill('0') << data_now->ID->ID << " ";
	//		for (int i = 0; i < 8; i++)
	//			out << hex << uppercase << setw(2) << setfill('0') << data_now->com_data[i] << " ";
	//		out << endl;
	//	}
	out << endl;
	out.close();
}
//���ļ����ݺϲ�------������
void Combine()
{
	for (file_now = file_first; file_now; file_now = file_now->next)
		for (root_now = file_now->tree_root; root_now; root_now = root_now->next)
			for (data_now = root_now->child; data_now; root_now = root_now->next)
			{

			}
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