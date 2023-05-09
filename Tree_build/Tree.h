#pragma once
#include <func.h>

struct _File_Data;
struct _Data_Root;
struct _Data;
struct _Floor_Data;
//���ļ��£����ڱ���ԭʼ�ļ��Ľṹ��
typedef struct _File_Data
{
	//��¼������Դ�ļ�
	string file_path;
	//��¼ԭʼ����
	vector<_Data*> data;
	//ָ�룬ָ��ǰһ��Ԫ���Լ���һ��Ԫ��
	_File_Data* pre = NULL;
	_File_Data* next = NULL;
	//ָ��������
	_Data_Root* tree_root = NULL;
	//ͳ��ID��������
	int ID_num = 0;
	//ͳ�Ƶ�ǰ�ļ�������
	int Total_data_num = 0;
	//�ж��Ƿ���������һ����
	bool Len_same = true;
	//ͳ�Ƶ�ǰ�ļ��п��ܱ�ʾ¥�����������ĸ���
	int Floor_count = 0;
	//ͳ�Ƶ�ǰ�ļ�����ʾ¥��߶�
	int Floor_height = 0;
	//��¼�ļ��������ģ���ʼ-��ͣ-�յ� ¥��
	vector<int>Floor_pass;
	//��¼���л�������,true����,false����
	bool direction = true;
	//��¼ÿ��¥����
	_Floor_Data* floor_data;
	//��¼���ҵ����ź�
	map<string, string>Signal = { {"Floor",""},{"Door",""},{"Run",""} };
}File_Data;

//���ڼ�¼ID�Ľṹ��
typedef struct _Data_Root
{
	//��¼IDֵ
	int ID = -1;
	//�Ѿ�������������������
	int total_type = 0;
	//ǰ����������
	int pre_data_type = 0;
	//ͳ�Ƹ��ļ���ǰID����������
	int count = 0;	
	//ID��ʾ�����ͣ�1 ���أ�2 ���ᣬ3 ����(¥��)��4 ����
	int tag = 0;
	_Data* child = NULL;
	_Data_Root* next = NULL;
	_Data_Root* pre = NULL;
	_File_Data* file = NULL;
}Data_Root;

//���ڼ�¼���ݵĽṹ��
typedef struct _Data
{
	//���ݲ���
	int com_data[DATA_SIZE] = { 0 };
	//��¼���ݵ�λ��
	int pos = 1;
	//��¼����λ��
	int size = 0;
	//ͳ����ͬ���ݳ��ֵĴ���
	int count = 1;
	vector<int> tag_all;
	vector<int>::iterator tag_each;
	//ָ�룬ָ��ǰһ��Ԫ�ء���һ��Ԫ���Լ�ID
	_Data* pre = NULL;
	_Data* next = NULL;
	_Data_Root* ID = NULL;
	//����ÿ����ֽ����ݵĸ���ֵ
	double percent[DATA_SIZE][2];
	double percent_sum = 0;
	//״̬�����ڶ����������ͣ�¥�㡢���С������ٵȣ����嶨������� ��{1.¥�㣬2.�ţ�3.����}
	int status[5] = { 0 };
	//ͳ�����ݳ��ֵ�¥��
	bool exist_floor[0xff] = { 0 };
	//���������
	bool operator==(const _Data& k)const
	{
		if (ID->ID == k.ID->ID)
		{
			for (int i = 0; i < DATA_SIZE; i++)
				if (com_data[i] != k.com_data[i])
					return false;
			return true;
		}
		return false;
	}
}Data;

void tree_main(string file_path);
void tree_delete();
void floor_verify();
void Data_Restore(string out_path);