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

#define SIZE 200 //一行数据的字符数
#define DATA_SIZE 8   //需要比对的数据量，以两位十六进制数（一个字节）记为一个数据
#define PRE_SIZE 2 //前面固定的数据位数
#define NULL 0
#define TYPE_NUM 4

using namespace std;

#include <tree.h>
#include <floor.h>
#include <door.h>
#include <floor_compare.h>

map<string,string> call(string path);//调用接口
void format(int* data, char* buff, int& flag);//十六进制字符转换为数字
void getFiles(string path, vector<string>& files);//获取路径下所有*.txt文件
void _delete();
