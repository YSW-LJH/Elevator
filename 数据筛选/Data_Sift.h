#pragma once
#include <func.h>

struct _File_Data;
struct _SData;
typedef struct _SData
{
	map<long long, vector<vector<int>>>data;
	map<long long, vector<vector<int>>>::iterator it;
}SData;


void Data_Sift_main(string path, const int pre, const int sel);