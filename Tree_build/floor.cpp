#include <floor.h>

Floor* floors = NULL;
Floor* floor_first = NULL;

extern Tree_Root* roots;
extern Data* datas;
extern Tree_Root* root_first;
extern int ID_num;

extern int Mode;
extern int Floor_height;
extern string File_name;
extern string File_path;

static void pre_process();
static void pre_process_2();
static void pre_process_2_help(vector<vector<int>>& data, int ID, int pre_size);
static void new_node(Tree_Root* root, Data* data, int count, int pos, int pre_num, int sub_num);//pre_process_help
static void floor_merge();
static bool node_compare(Floor* A, Floor* B);

void floor_delete();

void floor_main()
{
	pre_process_2();
	if (floor_first == NULL)
	{
		cout << "δ�ҵ����ܵ�¥�����ݣ�����\n";
		return;
	}
	//�ϲ���ͬ����
	//floor_merge();
}
//��һ�ִ���ʽ��ֱ�Ӳ��ҵ�������--������������������.
static void pre_process()
{
	for (roots = root_first; roots; roots = roots->next)
		for (int i = 0; i <= 4; i++)//Ŀǰ���ն������ݺ����λ��С�ڵ���4λ����
		{
			int temp_data[DATA_SIZE] = { 0 };//��¼��ʱ����
			bool pre_same = false;//��¼ǰ�������Ƿ���ͬ
			int count[DATA_SIZE] = { 0 };//����
			Data* data_temp[DATA_SIZE] = { NULL };//����ָ��
			datas = roots->child;
			for (; datas; )
			{
				//������Ҫ�ȶԵ�����
				if (pre_same == false)
				{
					//��ʼ������
					for (int j = 0; j < i; j++)
						temp_data[j] = datas->com_data[j];
					for (int j = 0; j < DATA_SIZE; j++)
						data_temp[j] = datas;
					memset(count, 0, sizeof(count));
					pre_same = true;
					datas = datas->next;
					//��һ�ڵ㲻�������˳�
					if (datas == NULL)
						break;
					for (int j = i; j < DATA_SIZE; j++)
						temp_data[j] = datas->_pre[j];
				}
				//�ȶ�ǰ�������Ƿ���ͬ, ��ͬ��������� ,
				for (int j = 0; j < i; j++)
					if (temp_data[j] != datas->com_data[j])
					{
						for (int j = i; j < DATA_SIZE; j++)
							if (count[j] + 1 == Floor_height)
								new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
						pre_same = false;
						break;
					}
				if (pre_same == false)
					continue;
				//�ж�ǰ��������ͬ����ʼ����ͳ��
				if (i == 3 && datas->com_data[0] == 0x1D && datas->com_data[1] == 0x00 && datas->com_data[2] == 0x47)
					datas = datas;
				for (int j = i; j < DATA_SIZE; j++)
				{
					if (datas->_pre[j] == 0)
						continue;
					if (temp_data[j] == datas->_pre[j])
						count[j]++;
					else
					{
						if (count[j] + 1 == Floor_height)
							new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
						count[j] = 1;
						data_temp[j] = datas->pre;
						temp_data[j] = datas->_pre[j];
					}
				}
				datas = datas->next;
			}
			for (int j = i; j < DATA_SIZE; j++)
				if (count[j] + 1 == Floor_height)
					new_node(roots, data_temp[j], count[j], j, i, temp_data[j]);
		}
}
//�ڶ��ִ���ʽ��ͳ��һ���е����ݣ�Ȼ�����ҵ���
static void pre_process_2()
{
	vector<vector<int>>data(8, vector<int>(0, 0));
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		for (roots = root_first; roots; roots = roots->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (datas = roots->child; datas; datas = datas->next)
			{
				//�ж�ǰ�������Ƿ���ͬ
				for (int i = 0; i < pre_size; i++)
					if (pre_data[i] != datas->com_data[i])
					{
						pre_same = false;
						break;
					}
				//ǰ�����ݲ�ͬ������ǰ�����ݲ����������Ѳɼ�������
				if (!pre_same)
				{
					pre_process_2_help(data, roots->ID,pre_size);
					for (int i = 0; i < pre_size; i++)
						pre_data[i] = datas->com_data[i];
					pre_same = true;
				}
				//�����ݷ�������
				for (int i = 0; i < DATA_SIZE; i++)
					data[i].push_back(datas->com_data[i]);
			}
			pre_process_2_help(data, roots->ID,pre_size);
		}
		delete[]pre_data;
	}
}
//�ж������Ƿ����¥��
static void pre_process_2_help(vector<vector<int>>& data, int ID, int pre_size)
{
	vector<int>temp;
	for (int pos = pre_size; pos < DATA_SIZE; pos++)
	{
		temp = data[pos];
		if (temp.size() <= 1)
			continue;
		//���򣬺ϲ���ͬ������
		sort(temp.begin(), temp.end());
		vector<int>::iterator a = temp.begin();
		while (a != temp.end())
		{
			if (*a == *(a + 1))
				a = temp.erase(a);
			else a++;
		}
		if (temp.size() != Floor_height)
			continue;
		//�ж������Ƿ�Ϊ�Ȳ�����
		int flag = temp[1] - temp[0];
		bool same = true;
		a = temp.begin();
		a += 2;
		for (; a != temp.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = false;
				break;
			}
		if (!same)
			continue;
		//�ж����ݴ���¥�㣬����ڵ�
		Floor* temp_floor = new Floor();
		temp_floor->pre_size = pre_size;
		temp_floor->byte_pos = pos + 1;
		temp_floor->height = Floor_height;
		temp_floor->count = Floor_height;
		temp_floor->ID = ID;
		temp_floor->sub_num = flag;
		for (int i = 0; i < Floor_height; i++)
		{
			vector<int>temp_0;
			for (int j = 0; j < DATA_SIZE; j++)
				temp_0.push_back(data[j][i]);
			temp_floor->data.push_back(temp_0);
		}
		if (floor_first == NULL)
		{
			floor_first = temp_floor;
			floors = temp_floor;
		}
		else
		{
			floors->next = temp_floor;
			temp_floor->pre = floors;
			floors = floors->next;
		}
	}
	data.clear();
	data.insert(data.begin(), 8, vector<int>(0, 0));
}
//�����½ڵ㣬����һ�ִ���ʽʹ��
static void new_node(Tree_Root* root, Data* data, int count, int pos, int pre_num, int sub_num)
{
	Floor* temp = new Floor();
	temp->file_path = File_path;
	temp->ID = root->ID;
	temp->count = count + 1;
	temp->byte_pos = pos + 1;
	temp->pre_size = pre_num;
	temp->sub_num = sub_num;
	temp->height = Floor_height;
	temp->data.push_back(vector<int>(0, 0));
	for (int i = 0; i < DATA_SIZE; i++)
		temp->data[0].push_back(data->com_data[i]);
	data = data->next;
	for (int i = 1; i <= count; data = data->next)
		if (data->_pre[pos] != 0)
		{
			vector<int>temp_data;
			for (int j = 0; j < DATA_SIZE; j++)
				temp_data.push_back(data->com_data[j]);
			temp->data.push_back(temp_data);
			i++;
		}
	//���ͷ�ڵ�Ϊ�գ�����ͷ�ڵ�
	if (floor_first == NULL)
	{
		floor_first = temp;
		floors = temp;
		return;
	}
	floors->next = temp;
	temp->pre = floors;
	floors = floors->next;
}
//�ڵ�ϲ�--���ļ�����ʱ��ʹ�ã����ļ���������һ��ʹ�ã�ȡ��ԭ��
static void floor_merge()
{
	for (floors = floor_first; floors && floors->next; floors = floors->next)
		for (Floor* temp = floors->next; temp;)
			if (node_compare(floors, temp))
			{
				Floor* temp_del = temp;
				temp = temp->next;
				if (floors->pre_size < temp_del->pre_size)
				{
					floors->pre_size = temp_del->pre_size;
					floors->file_path = temp_del->file_path;
				}

				temp_del->pre->next = temp_del->next;
				if (temp_del->next)
					temp_del->next->pre = temp_del->pre;
				delete temp_del;
			}
			else
				temp = temp->next;
}
//�Ƚ������ڵ�֮�������
static bool node_compare(Floor* A, Floor* B)
{
	if (A->ID != B->ID)
		return false;
	for (int i = 0; i < DATA_SIZE; i++)
		if (A->data[0][i] != B->data[0][i])
			return false;
	if (A->byte_pos != B->byte_pos)
		return false;
	if (A->count != B->count)
		return false;
	if (A->sub_num != B->sub_num)
		return false;
	return true;
}
//ɾ������
void floor_delete()
{
	floors = floor_first;
	while (floors)
	{
		Floor* temp = floors;
		floors = floors->next;
		delete temp;
	}
	floors = NULL;
	floor_first = NULL;
}
