#include<door.h>

extern File_Data* file_now;

Data* rule_1[3] = { NULL };//˳��A-B-C-B-A
Data* rule_2[4] = { NULL };//˳��A-B-C-D-A
Floor_Data* floor_data;

static bool find_1();
static bool find_2();
string data_to_string(Data* data);

void door_main()
{
	floor_data = file_now->floor_data;
	map<string, string> signal = { {"Open",""},{"Run_O",""},{"Run_C",""},{"Close",""} };
	bool flag;
	flag = find_1();
	if (!flag)
	{
		flag = find_2();
		if (flag)
		{
			signal["Open"] = data_to_string(rule_2[0]);
			signal["Run_O"] = data_to_string(rule_2[1]);
			signal["Run_C"] = data_to_string(rule_2[2]);
			signal["Close"] = data_to_string(rule_2[3]);
		}
	}
	else
	{
		signal["Open"] = data_to_string(rule_1[0]);
		signal["Run_O"] = data_to_string(rule_1[1]);
		signal["Run_C"] = data_to_string(rule_1[1]);
		signal["Close"] = data_to_string(rule_1[2]);
	}
	file_now->Signal["Door"] = signal;
	return;
}
//���ݹ���һ����
bool find_1()
{
	//��¼¥��߶�
	int height = file_now->Floor_height;
	//��¼��һ����ͣ�����յ㣩¥������ݣ�������ǰһ��¥�����ݣ���������ʼ¥�㣩
	Floor_Data data_temp;
	for (int i = 2; i >= 1; i--)
		for (int j = 0; j < floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data.size(); j++)
			data_temp.data.push_back(floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data[j]);
	Floor_Data* data = &data_temp;
	//��¼��ͣ���յ�¥�㣬��ʼ��Ϊ false
	bool* flag = new bool[height];
	for (int i = 0; i < height; i++)
		flag[i] = false;
	//���¾�ͣ���յ�¥��λ��
	for (int i = 1; i < file_now->Floor_pass.size(); i++)
		flag[file_now->Floor_pass[i] - 1] = true;
	//��ȡ���ŵ�λ�źţ�ɸѡ��������ʼ����ͣ���յ�¥�㶼����,����¥�㲻��
	for (vector<Data*>::iterator pos_1 = data->data.begin(); pos_1 != data->data.end(); pos_1++)
	{
		bool flag_1 = true;
		for (int i = 0; i < file_now->Floor_pass.size(); i++)
			if (!(*pos_1)->exist_floor[file_now->Floor_pass[i]-1])
			{
				flag_1 = false;
				break;
			}
		if (!flag_1)
			continue;
		rule_1[0] = *pos_1;
		(*pos_1)->status[1] = 1;
		//��ȡ�������źţ�ɸѡ������ID��ͬ��δ��ѡȡ���ҽ��ھ�ͣ���յ�¥�����
		for (vector<Data*>::iterator pos_2 = pos_1 + 1; pos_2 != data->data.end(); pos_2++)
		{
			if ((*pos_2)->status[1] != 0)
				continue;
			if ((*pos_2)->ID->ID != rule_1[0]->ID->ID)
				continue;
			bool flag_2 = true;
			for (int i = 0; i < height; i++)
				if ((*pos_2)->exist_floor[i] != flag[i])
				{
					flag_2 = false;
					break;
				}
			if (!flag_2)
				continue;
			rule_1[1] = *pos_2;
			(*pos_2)->status[1] = 2;
			//��ȡ���ŵ�λ�źţ�ɸѡ������ID��ͬ��δ��ѡȡ���ҽ��ھ�ͣ���յ�¥�����
			for (vector<Data*>::iterator pos_3 = pos_2 + 1; pos_3 != data->data.end(); pos_3++)
			{
				if ((*pos_3)->status[1] != 0)
					continue;
				if ((*pos_3)->ID->ID != rule_1[0]->ID->ID)
					continue;
				bool flag_3 = true;
				for (int i = 0; i < height; i++)
					if ((*pos_3)->exist_floor[i] != flag[i])
					{
						flag_3 = false;
						break;
					}
				if (!flag_3)
					continue;
				rule_1[2] = *pos_3;
				(*pos_3)->status[1] = 3;
				//��֤���ź�
				vector<Data*>::iterator temp = data->data.begin();
				//��ǰ��״̬ 1 ���ŵ�λ��2 ���˶���3 ���ŵ�λ
				int door_stat = 1;
				//��ǰ�����ڿ��Ż��ǹ���
				bool door_flag = true;
				//��֤���
				bool verify = true;
				for (; temp != data->data.end(); temp++)
					if ((*temp)->status[1] != 0 && (*temp)->status[1] != door_stat)
					{
						switch (door_stat)
						{
							//���ţ��ҽ�Ҫ���ţ�����һ״̬Ϊ���˶�
						case 1:
							if (door_flag)
								door_stat = 2;
							else
								verify = false;
							break;
							//���˶�����������һ״̬Ϊ���ŵ�λ����������һ״̬Ϊ���ŵ�λ
						case 2:
							if (door_flag)
								door_stat = 3;
							else
								door_stat = 1;
							break;
							//���ŵ�λ������������й��ţ�״̬Ϊ���˶�
						case 3:
							if (door_flag)
								door_flag = false;
							else
								verify = false;
							door_stat = 2;
							break;
						}
						//�ж��Ƿ��Ԥ��ֵһ��
						if ((*temp)->status[1] != door_stat)
							verify = false;
						if (!verify)
							break;
					}
				if (verify)
				{
					delete[]flag;
					return true;
				}
				(*pos_3)->status[1] = 0;
			}
			(*pos_2)->status[1] = 0;
		}
		(*pos_1)->status[1] = 0;
	}
	delete[]flag;
	return false;
}
//���ݹ��������
bool find_2()
{
	//��¼¥��߶�
	int height = file_now->Floor_height;
	//��¼��һ����ͣ�����յ㣩¥������ݣ�������ǰһ��¥�����ݣ���������ʼ¥�㣩
	Floor_Data data_temp;
	for (int i = 2; i >= 1; i--)
		for (int j = 0; j < floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data.size(); j++)
			data_temp.data.push_back(floor_data[file_now->Floor_pass[1] - i * (file_now->direction ? 1 : -1)].data[j]);
	Floor_Data* data = &data_temp;
	//��¼��ͣ���յ�¥�㣬��ʼ��Ϊ false
	bool* flag = new bool[height];
	for (int i = 0; i < height; i++)
		flag[i] = false;
	//���¾�ͣ���յ�¥��λ��
	for (int i = 1; i < file_now->Floor_pass.size(); i++)
		flag[file_now->Floor_pass[i] - 1] = true;
	//��ȡ���ŵ�λ�źţ�ɸѡ��������ʼ����ͣ���յ�¥�㶼����,����¥�㲻��
	for (vector<Data*>::iterator pos_1 = data->data.begin(); pos_1 != data->data.end(); pos_1++)
	{
		bool flag_1 = true;
		for (int i = 0; i < file_now->Floor_pass.size(); i++)
			if (!(*pos_1)->exist_floor[file_now->Floor_pass[i]-1])
			{
				flag_1 = false;
				break;
			}
		if (!flag_1)
			continue;
		rule_2[0] = *pos_1;
		(*pos_1)->status[1] = 1;
		//��ȡ���������źţ�ɸѡ������ID��ͬ��δ��ѡȡ���ҽ��ھ�ͣ���յ�¥�����
		for (vector<Data*>::iterator pos_2 = pos_1 + 1; pos_2 != data->data.end(); pos_2++)
		{
			if ((*pos_2)->status[1] != 0)
				continue;
			if ((*pos_2)->ID->ID != rule_2[0]->ID->ID)
				continue;
			bool flag_2 = true;
			for (int i = 0; i < height; i++)
				if ((*pos_2)->exist_floor[i] != flag[i])
				{
					flag_2 = false;
					break;
				}
			if (!flag_2)
				continue;
			rule_2[1] = *pos_2;
			(*pos_2)->status[1] = 2;
			//��ȡ���ŵ�λ�źţ�ɸѡ������ID��ͬ��δ��ѡȡ���ҽ��ھ�ͣ���յ�¥�����
			for (vector<Data*>::iterator pos_3 = pos_2 + 1; pos_3 != data->data.end(); pos_3++)
			{
				if ((*pos_3)->status[1] != 0)
					continue;
				if ((*pos_3)->ID->ID != rule_2[0]->ID->ID)
					continue;
				bool flag_3 = true;
				for (int i = 0; i < height; i++)
					if ((*pos_3)->exist_floor[i] != flag[i])
					{
						flag_3 = false;
						break;
					}
				if (!flag_3)
					continue;
				rule_2[2] = *pos_3;
				(*pos_3)->status[1] = 3;
				//��ȡ���������źţ�ɸѡ������ID��ͬ��δ��ѡȡ���ҽ��ھ�ͣ���յ�¥�����
				for (vector<Data*>::iterator pos_4 = pos_3 + 1; pos_4 != data->data.end(); pos_4++)
				{
					if ((*pos_4)->status[1] != 0)
						continue;
					if ((*pos_4)->ID->ID != rule_2[0]->ID->ID)
						continue;
					bool flag_4 = true;
					for (int i = 0; i < height; i++)
						if ((*pos_4)->exist_floor[i] != flag[i])
						{
							flag_4 = false;
							break;
						}
					if (!flag_4)
						continue;
					rule_2[3] = *pos_4;
					(*pos_4)->status[1] = 4;
					//��֤���ź�
					vector<Data*>::iterator temp = data->data.begin();
					//��ǰ��״̬ 1 ���ŵ�λ��2 �����˶���3 ���ŵ�λ��4 ��������
					int door_stat = 1;
					//��ǰ�����ڿ��Ż��ǹ���
					bool door_flag = true;
					//��֤���
					bool verify = true;
					for (; temp != data->data.end(); temp++)
						if ((*temp)->status[1] != 0 && (*temp)->status[1] != door_stat)
						{
							switch (door_stat)
							{
								//���ţ��ҽ�Ҫ���ţ�����һ״̬Ϊ���˶�
							case 1:
								if (door_flag)
									door_stat = 2;
								else
									verify = false;
								break;
								//�����˶�����������һ״̬Ϊ���ŵ�λ����������һ״̬Ϊ���ŵ�λ
							case 2:
								if (door_flag)
									door_stat = 3;
								else
									verify = false;
								break;
								//���ŵ�λ������������й��ţ�״̬Ϊ���˶�
							case 3:
								if (door_flag)
									door_flag = false;
								else
									verify = false;
								door_stat = 4;
								break;
								//�����˶���������Ϊ���ű���
							case 4:
								if (door_flag)
									verify = false;
								door_stat = 1;
								break;
							}
							//�ж��Ƿ��Ԥ��ֵһ��
							if ((*temp)->status[1] != door_stat)
								verify = false;
							if (!verify)
								break;
						}
					if (verify)
					{

						delete[]flag;
						return true;
					}
					(*pos_4)->status[1] = 0;
				}
				(*pos_3)->status[1] = 0;
			}
			(*pos_2)->status[1] = 0;
		}
		(*pos_1)->status[1] = 0;
	}
	delete[]flag;
	return false;
}
//��Data->com_data����ת��Ϊ�ַ���
string data_to_string(Data* data)
{
	string temp = int2HString(data->ID->ID);
	if (temp.size() < 3)
		temp.insert(0, 3 - temp.size(), '0');		
	temp += " ";
	for (int i = 0; i < data->size; i++)
	{
		string hexx = int2HString(data->com_data[i]);
		if (hexx.size() < 2)
			hexx.insert(0, 2 - hexx.size(), '0');
		temp += hexx;
		temp += " ";
	}
	return temp;
}