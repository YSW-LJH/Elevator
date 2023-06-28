#include "pch.h"
#include "CppUnitTest.h"
//#include "func.h"

import Main_process;
import Formate;
import Data_Sift_module;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using namespace std;
std::string File_path;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		//树的建立与数据还原
		TEST_METHOD(Tree_Function)
		{
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\zhu";
			File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xin";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xi";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\hong";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\mo";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xin-guang";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			Data_Sift(File_path, 1, 2);
		}
		TEST_METHOD(Tree_Function_2)
		{
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\zhu";
			File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xin";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xi";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\hong";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\mo";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\xin-guang";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\测试数据\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			Data_Sift(File_path, 1, 2);
		}
	};
}
