#include <floor.h>

extern Data_Root* root_now;
extern Data* data_now;
extern Data_Root* root_first;
extern File_Data* file_now;
extern File_Data* file_first;


static void pre_process();
static void pre_process_help(vector<Data*>& data, int ID, int pre_size);

void floor_main()
{
	pre_process();
	
	if (file_now == NULL)
	{
		cout << "δ�ҵ����ܵ�¥�����ݣ�����\n";
		return;
	}
}
//ͳ��һ���е����ݣ�Ȼ�����ҵ���
static void pre_process()
{
	vector<Data*>data;
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		for (root_now = root_first; root_now; root_now = root_now->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (data_now = root_now->child; data_now; data_now = data_now->next)
			{
				//���¥���ǩ����������
				if (data_now->tag_all.size() > 0)
					if (data_now->tag_all[0] % 10 == 1)
						continue;
				//�ж�ǰ�������Ƿ���ͬ
				for (int i = 0; i < pre_size; i++)
					if (pre_data[i] != data_now->com_data[i])
					{
						pre_same = false;
						break;
					}
				//ǰ�����ݲ�ͬ������ǰ�����ݲ����������Ѳɼ�������
				if (!pre_same)
				{
					pre_process_help(data, root_now->ID,pre_size);
					for (int i = 0; i < pre_size; i++)
						pre_data[i] = data_now->com_data[i];
					pre_same = true;
				}
				//�����ݷ�������
				data.push_back(data_now);
			}
			pre_process_help(data, root_now->ID,pre_size);
		}
		delete[]pre_data;
	}
}
//�ж������Ƿ����¥��
static void pre_process_help(vector<Data*>& data, int ID, int pre_size)
{
	if (data.size() == 0)
		return;
	for (int pos = pre_size; pos < DATA_SIZE; pos++)
	{
		//�жϸ�λ�Ƿ���¥���ǩ
		if (data[0]->tag_all.size() > 0)
			if (data[0]->tag_all[0] % 10 == 1)
				if ((data[0]->tag_all[0] / 10) % 10 == pos + 1)
					continue;
		//�ɼ�posλ������
		vector<int>temp;
		for (auto i : data)
			temp.push_back(i->com_data[pos]);
		if (temp.size() <= 1)
			continue;
		//���򣬺ϲ���ͬ������
		sort(temp.begin(), temp.end());
		vector<int>::iterator a = temp.begin();
		while ((a + 1) != temp.end())
		{
			if (*a == *(a + 1))
				a = temp.erase(a);
			else a++;
		}
		//�ж������Ƿ�Ϊ�Ȳ���������������������
		if (temp.size() < file_now->Floor_height)
			continue;
		//if (temp.size() < 3)
		//	continue;
		int flag = temp[1] - temp[0];
		int same = 2;
		a = temp.begin();
		a += 2;
		for (; a != temp.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = 0;
				break;
			}
			else
				same++;
		if (same == 0)
			continue;

		//�жϵ��������Ƿ��¥��߶���ͬ
		if (same != file_now->Floor_height)
			continue;
		//�ж����ݴ���¥�㣬���ǩ
		file_now->Floor_count++;

		for (auto i : data)
			i->tag_all.push_back(file_now->Floor_count * 1000 + pre_size * 100 + (pos + 1) * 10 + 1);//�ڼ�����ǩ+ǰ���ֽ�λ+¥���ź�λ+����¥���ź�
	}
	//������ʼ��
	data.clear();
}
//������֤
void floor_verify()
{
	if (file_now->Floor_count == 0)
		return;
	int* data_temp = new int[file_now->Floor_count] {0};//���ڴ����ͬ¥���ǩ��ǰһ������
	int* D_value = new int[file_now->Floor_count] {0};//���ڴ��ÿ��¥���ǩ�������ݼ��ֵ
	int* floor = new int[file_now->Floor_count] {0};//�����ж�ÿ��¥���ǩ��ǰ���ݱ�ʾ��¥��
	int** floor_signal = new int*[file_now->Floor_count];//���ڼ�¼¥���ź���ϸ��Ϣ
	for (int i = 0; i < file_now->Floor_count; i++)//ID �ֽ�*4 POS MIN MAX Increment
	{
		floor_signal[i] = new int[9];
		memset(floor_signal[i], -1, sizeof(int)*9);
	}
	for (auto data : file_now->data)
		if (data->tag_all.size() > 0)
			for (auto num : data->tag_all)
				if (num % 10 == 1)//�ж�Ϊ¥���ǩ
				{
					int a = num / 1000 - 1;//��ʾ�ڼ��δ�ı�ǩ
					int b = (num % 100) / 10 - 1;//��ʾ���ݵĵڼ�λ��ʾ¥��
					int c = data->com_data[b] - data_temp[a];//��ʾ����
					if (data_temp[a] == -1)//����Ѿ��жϸ�������Ч��������Ƚ���һ��
						continue;
					//���¸ñ�ǩ�ļ�¼��¥���ź�
					//��¼ID
					if (floor_signal[a][0] == -1)
						floor_signal[a][0] = data->ID->ID;
					//��¼�����ֽ�
					for (int i = 0; i < (num / 100) % 10; i++)
						if (floor_signal[a][i + 1] == -1)
							floor_signal[a][i + 1] = data->com_data[i];
					//��¼¥���ź�λ
					if (floor_signal[a][5] == -1)
						floor_signal[a][5] = b + 1;
					//��¼�ź���Сֵ
					if (floor_signal[a][6] == -1 || floor_signal[a][6] > data->com_data[b])
						floor_signal[a][6] = data->com_data[b];
					//��¼�ź����ֵ
					if (floor_signal[a][7] == -1 || floor_signal[a][7] < data->com_data[b])
						floor_signal[a][7] = data->com_data[b];
					//��¼�ź�����
					if (c != 0)
						floor_signal[a][8] = c;
					//�жϵ�ǰ���ݱ�ʾ��¥��
					if (floor[a] == 0)
					{
						floor[a] = file_now->Floor_pass.front();
						data->status[0] = floor[a];
					}
					else
						if (c != 0)
						{
							floor[a] += file_now->direction ? 1 : -1;
							data->status[0] = floor[a];
						}
						else
							data->status[0] = floor[a];
					//��ʼ������
					if (data_temp[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						continue;
					}
					if (D_value[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						D_value[a] = c;
						continue;
					}
					//
					data_temp[a] = data->com_data[b];
					//�жϸ�������ǰһ�������Ƿ�Ȳ������ͬ
					if (c != 0 && c != D_value[a] && c != (-D_value[a]))
					{
						data_temp[a] = -1;
						file_now->Floor_count--;
						continue;
					}
				}
	//��¥¥���źż���File_Data�ṹ��
	string signal = "";
	for (int i = 0; i < file_now->Floor_height; i++)
		if (data_temp[i] != -1)
		{
			for (int j = 0; j < 9; j++)
				if (floor_signal[i][j] != -1)
				{
					if (j == 0) signal += "ID&Bit: ";
					if (j == 5)signal += "Pos: ";
					if (j == 6)signal += "Range: ";
					if (j == 8)signal += "Increment: ";
					if (j < 5 || j == 6 || j == 7)
					{
						string hexx = "";
						char a = 0;
						int flag = 0;
						for (int k = floor_signal[i][j]; k > 0; k /= 16, flag++)
							if (k % 16 < 10)
							{
								a = '0' + k % 16;
								hexx.insert(0, 1, a);
							}
							else
							{
								a = 'A' + k % 16 - 10;
								hexx.insert(0, 1, a);
							}
						if (flag < 2 && j>0)
							while (++flag <= 2)
								hexx.insert(0, 1, '0');
						if (flag < 3 && j == 0)
							while (++flag <= 3)
								hexx.insert(0, 1, '0');
						signal += hexx + ' ';
					}
					else
						signal += to_string(floor_signal[i][j]) + ' ';
				}
			signal += '\n';
		}
	file_now->Signal["Floor"] = signal;
	//��������ɾ����Ч¥���ǩ
	for (root_now = file_now->tree_root; root_now; root_now = root_now->next)
		for (data_now = root_now->child; data_now; data_now = data_now->next)
			for (vector<int>::iterator data = data_now->tag_all.begin(); data != data_now->tag_all.end(); )
				if (*data % 10 == 1)
					if (data_temp[*data / 1000 - 1] == -1)
					{
						data = data_now->tag_all.erase(data);
						data_now->status[0] = 0;
					}
					else
						data++;
				else
					break;
	delete[]data_temp;
	delete[]D_value;
	delete[]floor;
	for (int i = 0; i < file_now->Floor_height; i++)
		delete[]floor_signal[i];
	delete[]floor_signal;
}


