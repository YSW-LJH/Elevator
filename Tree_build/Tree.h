#pragma once
#include <func.h>

struct _File_Data;
struct _Data_Root;
struct _Data;
struct _Floor_Data;
//多文件下，用于保存原始文件的结构体
typedef struct _File_Data
{
	//记录数据来源文件
	string file_path;
	//记录原始数据
	vector<_Data*> data;
	//指针，指向前一个元素以及后一个元素
	_File_Data* pre = NULL;
	_File_Data* next = NULL;
	//指向建立的树
	_Data_Root* tree_root = NULL;
	//统计ID的种类数
	int ID_num = 0;
	//统计当前文件数据量
	int Total_data_num = 0;
	//判断是否所有数据一样长
	bool Len_same = true;
	//统计当前文件中可能表示楼层的数据种类的个数
	int Floor_count = 0;
	//统计当前文件所表示楼层高度
	int Floor_height = 0;
	//记录文件所包含的：起始-经停-终点 楼层
	vector<int>Floor_pass;
	//记录上行还是下行,true上行,false下行
	bool direction = true;
	//记录每层楼数据
	_Floor_Data* floor_data = NULL;
	//记录查找到的信号
	map<string, map<string, string>>Signal = {
		{"Floor",{
			{"ID&Bit",""},{"Pos",""},{"Range",""},{"Increment",""},{"Pre_size",""}
	}},
		{"Door",{
			{"Open",""},{"Run_O",""},{"Run_C",""},{"Close",""}
	}},
		{"Run",{
			{"Up",""},{"Down",""}
	}}
	};
}File_Data;

//用于记录ID的结构体
typedef struct _Data_Root
{
	//记录ID值
	int ID = -1;
	//已经包含的数据类型数量
	int total_type = 0;
	//前置数据种类
	int pre_data_type = 0;
	//统计该文件当前ID的数据总数
	int count = 0;	
	//ID表示的类型：1 主控，2 轿厢，3 内招(楼层)，4 外招
	int tag = 0;
	_Data* child = NULL;
	_Data_Root* next = NULL;
	_Data_Root* pre = NULL;
	_File_Data* file = NULL;
	//运算符重载
	bool operator>(const _Data_Root& k)const
	{
		return (ID > k.ID);
	}
	bool operator==(const _Data_Root& k)const
	{
		return (ID == k.ID);
	}
	//建立新副本并返回指针
	static _Data_Root* Add(const _Data_Root* k)
	{
		_Data_Root* temp = new _Data_Root();
		temp->count = k->count;
		temp->ID = k->ID;
		return temp;
	}
}Data_Root;

//用于记录数据的结构体
typedef struct _Data
{
	//数据部分
	int com_data[DATA_SIZE] = { 0 };
	//记录数据的位置
	int pos = 1;
	//记录数据位数
	int size = 0;
	//统计相同数据出现的次数
	int count = 1;
	//标签，楼层、门等
	vector<int> tag_all;
	vector<int>::iterator tag_each;
	//指针，指向前一个元素、后一个元素以及ID
	_Data* pre = NULL;
	_Data* next = NULL;
	_Data_Root* ID = NULL;
	//计算每半个字节数据的概率值
	double percent[DATA_SIZE][2];
	double percent_sum = 0;
	//状态，用于定义数据类型（楼层、运行、内外召等，具体定义见数组 ）{1.楼层，2.门，3.运行}
	int status[5] = { 0 };
	//统计数据出现的楼层
	bool exist_floor[0xff] = { 0 };
	//运算符重载
	bool operator>(const _Data& k)const
	{
		for (int i = 0; i < size; i++)
			if (com_data[i] == k.com_data[i])
				continue;
			else if (com_data[i] > k.com_data[i])
				return true;
			else 
				return false;
		return false;
	}
	bool operator==(const _Data& k)const
	{
		for (int i = 0; i < size; i++)
			if (com_data[i] != k.com_data[i])
				return false;
		return true;
	}
	//建立新副本并返回指针
	static _Data* Add(const _Data* k)
	{
		_Data* temp = new _Data();
		for (int i = 0; i < k->size; i++)
			temp->com_data[i] = k->com_data[i];
		temp->size = k->size;
		temp->count = k->count;
		return temp;
	}
}Data;

void tree_main(string file_path);
void tree_delete();
void Data_Restore(string out_path);
void Data_Combine();