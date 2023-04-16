#pragma once
#include <func.h>

struct _File_Data;
struct _Tree_Root;
struct _Data;

//���ļ��£����ڱ���ԭʼ�ļ��Ľṹ��
typedef struct _File_Data
{
	std::string file_path;//��¼������Դ�ļ�
	std::vector<_Data*> data;//��¼ԭʼ����
	//����  std::vector<std::vector<int>> data(0,std::vector<int>(0,0));
	_File_Data* pre = NULL;//ָ�룬ָ��ǰһ��Ԫ���Լ���һ��Ԫ��
	_File_Data* next = NULL;
	_Tree_Root* tree_root = NULL;//ָ��������
	int ID_num = 0;//ͳ��ID��������
	int Total_data_num = 0;//ͳ�Ƶ�ǰ�ļ�������
	int Floor_count = 0;//ͳ�Ƶ�ǰ�ļ��п��ܱ�ʾ¥�����������
	int Floor_height = 0;
	int Floor_begin = 0;
	int Floor_end = 0;
}File_Data;

//���ڼ�¼ID�Ľṹ��
typedef struct _Tree_Root
{
	int ID = -1;
	int total_type = 0;//�Ѿ�������������������
	int pre_data_type = 0;//ǰ����������
	int count = 0;	//ͳ�Ƹ��ļ���ǰID����������
	int tag = 0;//ID��ʾ�����ͣ�1 ���أ�2 ���ᣬ3 ����(¥��)��4 ����
	_Data* child = NULL;
	_Tree_Root* next = NULL;
	_Tree_Root* pre = NULL;
	_File_Data* file = NULL;
}Tree_Root;

//���ڼ�¼���ݵĽṹ��
typedef struct _Data
{
	int com_data[DATA_SIZE] = { 0 };//���ݲ���
	int pos = 1;//��¼���ݵ�λ��
	int size = 0;//��¼����λ��
	int count = 1;//ͳ����ͬ���ݳ��ֵĴ���
	std::vector<int> tag_all;
	std::vector<int>::iterator tag_each;
	_Data* pre = NULL;//ָ�룬ָ��ǰһ��Ԫ���Լ���һ��Ԫ��
	_Data* next = NULL;
	_Tree_Root* ID = NULL;
	double percent[DATA_SIZE][2];//����ÿ����ֽ����ݵĸ���ֵ
	double percent_sum = 0;
	int status[5] = { 0 };//״̬�����ڶ����������ͣ�¥�㡢���С������ٵȣ����嶨������� ��{1.¥�㣬2.�ţ�3.����}
}Data;

void tree_main();
void tree_delete();
void floor_verify();
void Data_Restore(std::string out_path);