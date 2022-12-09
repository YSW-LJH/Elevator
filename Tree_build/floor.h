#pragma once
#include <func.h>

typedef struct _Floor
{
	int ID = 0;
	std::vector<std::vector<int>>data;
	int pre_size = 0;//��¼ǰ���ֽ���
	int count = 0;//��¼ͳ������
	int byte_pos = 0;//��¼�ֽ�λ��
	int sub_num = 0;//��¼����
	int height = 0;//��¼¥��߶�
	std::string file_path;
	_Floor* next = NULL;
	_Floor* pre = NULL;
}Floor;

void floor_main();
void floor_delete();