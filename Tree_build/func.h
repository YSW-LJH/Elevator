#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <vector>
#include <direct.h>
#include <algorithm>
#include <io.h>
#include <list>
//#include <stdafx.h>

#define SIZE 200 //һ�����ݵ��ַ���
#define DATA_SIZE 8   //��Ҫ�ȶԵ�������������λʮ����������һ���ֽڣ���Ϊһ������
#define PRE_SIZE 2 //ǰ��̶�������λ��
#define NULL 0
#define TYPE_NUM 4

#include <Tree.h>
#include <door.h>
#include <floor.h>

using namespace std;

void call(string filename, int mode, int height, int w_size);//���ýӿ�
void format(int* data, char* buff, int& flag);//ʮ�������ַ�ת��Ϊ����
void getFiles(string path, vector<string>& files);//��ȡ·��������*.txt�ļ�
void _delete();

extern void floor_main();
extern void tree_main();