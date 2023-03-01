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
}File_Data;
//���ڼ�¼ID�Ľṹ��
typedef struct _Tree_Root
{
	int ID = -1;
	int total_type = 0;//�Ѿ�������������������
	int pre_data_type = 0;//ǰ����������
	int count = 0;	//ͳ��Data�ڵ�����
	_Data* child = NULL;
	_Tree_Root* next = NULL;
	_Tree_Root* pre = NULL;
	_File_Data* file = NULL;\
}Tree_Root;
//���ڼ�¼���ݵĽṹ��
typedef struct _Data
{
	int com_data[DATA_SIZE] = { 0 };//���ݲ���
	int pos = 1;//��¼���ݵ�λ��
	int size = 0;//��¼����λ��
	int count = 1;//ͳ����ͬ���ݳ��ֵĴ���
	std::vector<int> tag_every;
	std::vector<int>::iterator tag_each;
	_Data* pre = NULL;//ָ�룬ָ��ǰһ��Ԫ���Լ���һ��Ԫ��
	_Data* next = NULL;
	_Tree_Root* ID = NULL;
	int _pre[DATA_SIZE] = { 0 };//��¼ǰ���������ݼ��Ӧ�ֽ�λ�Ĳ�ֵ
	int tag[5] = { 0 };//��ǩ�����ڶ����������ͣ�¥�㡢���С������ٵȣ����嶨������� ��{1.¥�㣬2.�ţ�3.����}
}Data;

void tree_main();
void tree_delete();
