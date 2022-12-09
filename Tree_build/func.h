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

#define SIZE 200 //一行数据的字符数
#define DATA_SIZE 8   //需要比对的数据量，以两位十六进制数（一个字节）记为一个数据
#define PRE_SIZE 2 //前面固定的数据位数
#define NULL 0
#define TYPE_NUM 4

#include <Tree.h>
#include <door.h>
#include <floor.h>

using namespace std;

void call(string filename, int mode, int height, int w_size);//调用接口
void format(int* data, char* buff, int& flag);//十六进制字符转换为数字
void getFiles(string path, vector<string>& files);//获取路径下所有*.txt文件
void _delete();

extern void floor_main();
extern void tree_main();