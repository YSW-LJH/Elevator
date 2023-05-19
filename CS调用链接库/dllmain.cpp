// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

//BOOL APIENTRY DllMain( HMODULE hModule,
//                       DWORD  ul_reason_for_call,
//                       LPVOID lpReserved
//                     )
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}

// 电梯编码预处理.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <map>
import Main_process;
import Percent;
import Formate;
using namespace std;

//格式化路径下的所有.txt文件（需要是原始采集的电梯数据）
extern "C" __declspec(dllexport) void Formate_Function(const char* path)
{
	formate_main(path);
}
//概率计算（ID+B1+B2在文件中的占比），输入一个文件夹，计算文件夹下所有文件的概率并计算一个所有文件合计的概率，结果位于.\out文件夹下
extern "C" __declspec(dllexport) void Percent_Function(const char* path)
{
	percent_main(path);
}
//主要函数，信号分析，根据路径下的文件计算楼层信号、门信号、运行信号，以及一些数据特征等。
extern "C" __declspec(dllexport)const char* Signal_Function(const char* path)
{
	map<string, map<string,string>> a = Signal_Analyse(path);
	string b = "ID&Bit:\n";
	b += a["Floor"]["ID&Bit"] + "\n";
	b += "{floor}";
	b += "Pos:\n";
	b += a["Floor"]["Pos"] + "\n";
	b += "{floor}";
	b += "Range:\n";
	b += a["Floor"]["Range"] + "\n";
	b += "{floor}";
	b += "Increment:\n";
	b += a["Floor"]["Increment"];
	//
	b += "{Splite}Open:\n";
	b += a["Door"]["Open"] + "\n";
	b += "Run_O:\n";
	b += a["Door"]["Run_O"] + "\n";
	b += "Run_C:\n";
	b += a["Door"]["Run_C"] + "\n";
	b += "Close:\n";
	b += a["Door"]["Close"];
	//
	b += "{Splite}Up:\n";
	b += a["Run"]["Up"] + "\n";
	b += "Down:\n";
	b += a["Run"]["Down"];
	return _strdup(b.c_str());
}

