#pragma once
#include <func.h>

//���ڼ�¼���ݵĽṹ��
typedef struct _Data
{
	int com_data[DATA_SIZE] = { 0 };//���ݲ���
	int pos = 1;//��¼���ݵ�λ��
	int size = 0;//��¼����λ��
	int count = 1;//ͳ����ͬ���ݳ��ֵĴ���
	_Data* pre = NULL;
	_Data* next = NULL;
	int _pre[DATA_SIZE] = { 0 };//��¼ǰ���������ݼ��Ӧ�ֽ�λ�Ĳ�ֵ
}Data;
//���ڼ�¼ID�Ľṹ��
typedef struct _Tree_Root
{
	int ID = -1;
	int total_type = 0;//�Ѿ�������������������
	int pre_data_type = 0;//ǰ����������
	int count = 0;//ͳ��Data�ڵ�����
	unsigned int* Window[DATA_SIZE] = { NULL };//ͳ��Data��ÿһ����������
	int Window_count[DATA_SIZE] = { 0 };
	Data* child = NULL;
	_Tree_Root* next = NULL;
	_Tree_Root* pre = NULL;
}Tree_Root;

void tree_main();
void tree_delete();
