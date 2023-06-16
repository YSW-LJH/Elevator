#include <floor.h>

extern Data_Root* root_now;
extern Data* data_now;
extern Data_Root* root_first;
extern File_Data* file_now;
extern File_Data* file_first;


static void pre_process();
static void pre_process_help(vector<Data*>& data, int ID, int pre_size);
static void floor_verify();

void floor_main()
{
	pre_process();
	if (file_now == NULL)
	{
		//cout << "δ�ҵ����ܵ�¥�����ݣ�����\n";
		return;
	}
	floor_verify();
}
//ͳ��һ���е����ݣ�Ȼ�����ҵ���
static void pre_process()
{
	vector<Data*>data;
	for (int pre_size = 0; pre_size <= 4; pre_size++)
	{
		int* pre_data = new int[pre_size];
		root_first = file_now->tree_root;
		for (root_now = root_first; root_now; root_now = root_now->next)
		{
			memset(pre_data, -1, pre_size);
			bool pre_same = false;
			for (data_now = root_now->child; data_now; data_now = data_now->next)
			{
				//���¥���ǩ����������
				if (data_now->tag_all.size() > 0)
					if (data_now->tag_all[0] % 16 == 1)
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
			if (data[0]->tag_all[0] % 16 == 1)
				if ((data[0]->tag_all[0] / 16) % 16 == pos + 1)
					continue;
		//�ɼ�posλ������
		vector<int>pos_data;
		for (auto i : data)
			pos_data.push_back(i->com_data[pos]);
		if (pos_data.size() <= 1)
			continue;
		//���򣬺ϲ���ͬ������
		sort(pos_data.begin(), pos_data.end());
		vector<int>::iterator a = pos_data.begin();
		while ((a + 1) != pos_data.end())
		{
			if (*a == *(a + 1))
				a = pos_data.erase(a);
			else a++;
		}
		//�ж������Ƿ�Ϊ�Ȳ���������¥��߶���ͬ
		if (pos_data.size() != file_now->Floor_height)
			continue;
		//if (pos_data.size() < 3)
		//	continue;
		int flag = pos_data[1] - pos_data[0];
		bool same = true;
		a = pos_data.begin();
		a += 2;
		for (; a != pos_data.end(); a++)
			if (*a - *(a - 1) != flag)
			{
				same = false;
				break;
			}
		if (!same)
			continue;
		//�ж����ݴ���¥�㣬���ǩ
		file_now->Floor_count++;
		//������ͬ�ֽ�λ������¥���ź�λ
		int same_pos = 0;
		for (int temp_pos = 0; temp_pos < 8; temp_pos++)
		{
			if (temp_pos == pos || temp_pos >= data[0]->size)
			{
				same_pos *= 2;
				continue;
			}
			//ͳ�Ʋ�ͬ¥����ĳһλ���ݣ��ж��Ƿ�ÿ�㶼������ͬ��ֵ�������������źţ�
			map<int, vector<int>>pos_sig;
			pos_sig[data[0]->com_data[pos]].push_back(data[0]->com_data[temp_pos]);
			int run_pos = 0;
			for (int i = 1, floor_temp = data[0]->com_data[pos]; i < data.size(); i++)
			{
				if (data[i - 1]->com_data[temp_pos] != data[i]->com_data[temp_pos])
					flag = false;
				//ͳ��ÿ��¥���ź��µ�����
				pos_sig[data[i]->com_data[pos]].push_back(data[i]->com_data[temp_pos]);
			}
			if (flag)
				same_pos = same_pos * 2 + 1;
			else
			{
				flag = false;
				//ͳ��ÿ�����ݳ��ֵ�¥�����
				map<int, int>pos_count;
				for (auto temp_sig : pos_sig)
				{
					vector<int> temp_data = temp_sig.second;
					sort(temp_data.begin(), temp_data.end());
					for (int i = 0, num = -1; i < temp_data.size(); i++)
						if (num != temp_data[i])
						{
							num = temp_data[i];
							pos_count[num]++;
						}
				}
				//�жϳ��ִ����Ƿ���¥��߶���ͬ
				for(auto count:pos_count)
					if (count.second == file_now->Floor_height)
					{
						flag = true;
						break;
					}
				if (flag)
					same_pos = same_pos * 2 + 1;
				else
					same_pos *= 2;
			}
		}
		//���¥���ǩ
		for (auto i : data)
			//---------------------------------�ڼ�����ǩ(F00000)---��ͬ�ֽ�λͳ��(FF000)---���ǰ���ֽ�(F00)-----¥���ź�λ(F0)--����¥���ź�(F)
			i->tag_all.push_back(file_now->Floor_count * (int)pow(16, 5) + same_pos * (int)pow(16, 3) + pre_size * (int)pow(16, 2) + (pos + 1) * 16 + 1);
	}
	//������ʼ��
	data.clear();
}
//������֤
void floor_verify()
{
	if (file_now->Floor_count == 0)
		return;
	int* data_temp = new int[file_now->Floor_count] {0};//���ڴ����ͬ¥���ǩ��ǰһ������,-1����Ϊ¥��������Ч
	int* D_value = new int[file_now->Floor_count] {0};//���ڴ��ÿ��¥���ǩ�������ݼ��ֵ
	int* floor = new int[file_now->Floor_count] {0};//�����ж�ÿ��¥���ǩ��ǰ���ݱ�ʾ��¥��
	int** floor_signal = new int*[file_now->Floor_count];//���ڼ�¼¥���ź���ϸ��Ϣ
	for (int i = 0; i < file_now->Floor_count; i++)//ID �ֽ�*8 POS MIN MAX Increment Pre_size
	{
		floor_signal[i] = new int[14];
		memset(floor_signal[i], -1, sizeof(int)*14);
	}
	for (auto data : file_now->data)
		if (data->tag_all.size() > 0)
			for (auto tag : data->tag_all)
				if (tag % 16 == 1)//�ж�Ϊ¥���ǩ
				{
					const int a = tag / (int)pow(16, 5) - 1;//��ʾ�ڼ��δ�ı�ǩ
					const int b = (tag % (16 * 16)) / 16 - 1;//��ʾ���ݵĵڼ�λ��ʾ¥��
					const int c = data->com_data[b] - data_temp[a];//��ʾ����
					if (data_temp[a] == -1)//����Ѿ��жϸ�������Ч��������Ƚ���һ��
						continue;
					//���¸ñ�ǩ��¼��¥���ź�
					//��¼ID
					if (floor_signal[a][0] == -1)
						floor_signal[a][0] = data->ID->ID;
					//0x100��ǰ���ֽ�λ�� 0x200��¥���ź�λ�� 0x300������ͬ�ֽ�λ�� 0x400����Ч�ֽ�λ�� 0x000��ͬ�ֽ�λ
					//��¼�����ֽڼ����ǰ���ֽ�,��¥���źŵ�һ�η����ı�ʱ
					if (data_temp[a] != 0 && c != 0)
						for (int i = 0, flag_data = (tag / (16 * 16)) % (16 * 16 * 16); i < 8; i++)
							if (floor_signal[a][i + 1] == -1)
							{
								if (i < flag_data % 16)
									//��¼ǰ���ֽ�
									floor_signal[a][i + 1] = data->com_data[i] + 0x100;
								else if (i == b)
									//��¼¥���ź�
									floor_signal[a][i + 1] = 0x200;
								else if (i < data->size)
								{
									//��¼������ͬ����λ,����ͬ��¼Ϊ0x300
									if ((flag_data / (int)pow(2, (11 - i)) % 2) == 1)
										floor_signal[a][i + 1] = data->com_data[i];
									else
										floor_signal[a][i + 1] = 0x300;
								}
								else
									//��Ч�ֽ�λ
									floor_signal[a][i + 1] = 0x400;
							}
					//��¼¥���ź�λ
					if (floor_signal[a][9] == -1)
						floor_signal[a][9] = b + 1;
					//��¼�ź���Сֵ
					if (floor_signal[a][10] == -1 || floor_signal[a][10] > data->com_data[b])
						floor_signal[a][10] = data->com_data[b];
					//��¼�ź����ֵ
					if (floor_signal[a][11] == -1 || floor_signal[a][11] < data->com_data[b])
						floor_signal[a][11] = data->com_data[b];
					//��¼�ź�����
					if (c != 0)
						floor_signal[a][12] = file_now->direction ? c : -c;
					//��¼ǰ���ֽ�
					if (c != 0)
						floor_signal[a][13] = (tag / (16 * 16)) % 16;
					//------------------------------------------------------------
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
					//ֻ����һ��
					if (D_value[a] == 0)
					{
						data_temp[a] = data->com_data[b];
						D_value[a] = c;
						continue;
					}
					//
					data_temp[a] = data->com_data[b];
					//�жϸ�������ǰһ�������Ƿ�Ȳ������ͬ
					if (c != 0 && c != D_value[a])// && c != (-D_value[a]))
					{
						data_temp[a] = -1;
						file_now->Floor_count--;
						continue;
					}
				}
	//��¥���źż���File_Data�ṹ��
	map<string, string> signal = { {"ID&Bit",""},{"Pos",""},{"Range",""},{"Increment",""} };
	for (int i = 0; i < file_now->Floor_count; i++)
		//�ж��Ƿ�����Ч��¥���ź�
		if (data_temp[i] != -1)
		{
			int j = 0;
			signal["ID&Bit"] += int2HString(floor_signal[i][j], 3) + " ";
			//��¼¥���źŲ���
			while (++j < 9)
			{
				//ǰ���ֽ�
				if (floor_signal[i][j]/(16*16) == 1)
				{
					signal["ID&Bit"] += int2HString(floor_signal[i][j] - 0x100, 2) + " ";
					//signal["ID&Bit"] += int2HString(floor_signal[i][j], 2) + " ";
					continue;
				}
				//¥���ź�λ
				if (floor_signal[i][j] == 0x200)
				{
					signal["ID&Bit"] += "FL ";
					continue;
				}
				//����ͬ�ź�λ
				if (floor_signal[i][j] == 0x300)
				{
					signal["ID&Bit"] += "XX ";
					continue;
				}
				//�Ƿ������
				if (floor_signal[i][j] == 0x400)
				{
					j = 9;
					break;
				}
				//��ͬ����λ
				signal["ID&Bit"] += int2HString(floor_signal[i][j], 2) + " ";
			}
			signal["ID&Bit"] += "\n";
			//��¼¥���ź�λ��
			signal["Pos"] += to_string(floor_signal[i][j++]);
			signal["Pos"] += "\n";
			//��¼¥���źŷ�Χ
			signal["Range"] += int2HString(floor_signal[i][j++], 2) + " ";
			signal["Range"] += int2HString(floor_signal[i][j++], 2);
			signal["Range"] += "\n";
			//��¼¥���ź�����
			signal["Increment"] += to_string(floor_signal[i][j++]);
			signal["Increment"] += "\n";
			//��¼¥���ź�����
			signal["Pre_size"] += to_string(floor_signal[i][j]);
			signal["Pre_size"] += "\n";
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
	for (int i = 0; i < file_now->Floor_count; i++)
		delete[]floor_signal[i];
	delete[]floor_signal;
}


