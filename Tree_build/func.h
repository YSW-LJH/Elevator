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
#include <sstream>
#include <map>
//#include <stdafx.h>

#define SIZE 200 //һ�����ݵ��ַ���
#define DATA_SIZE 8   //��Ҫ�ȶԵ�������������λʮ����������һ���ֽڣ���Ϊһ������
#define PRE_SIZE 2 //ǰ��̶�������λ��
#define NULL 0
#define TYPE_NUM 4

using namespace std;

#include <tree.h>
#include <floor.h>
#include <door.h>
#include <floor_compare.h>

map<string,string> call(string path);//���ýӿ�
void format(int* data, char* buff, int& flag);//ʮ�������ַ�ת��Ϊ����
void getFiles(string path, vector<string>& files);//��ȡ·��������*.txt�ļ�
void _delete();
