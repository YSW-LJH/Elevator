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
#include <cmath>
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

map<string,map<string,string>> call(string path);//���ýӿ�
void format(int* data, char* buff, int& flag);//��һ������ת��Ϊ����
int HString2int(string str);//ʮ�������ַ���ת����
string int2HString(int num);//����תʮ�������ַ���
string int2HString(int num, int size);//����תʮ�������ַ���,������С���ȣ�������0����
vector<string> string_splite(const string str, const char* spl);//�ַ����ָ�
void getFiles(string path, vector<string>& files);//��ȡ·��������*.txt�ļ�
void _delete();
