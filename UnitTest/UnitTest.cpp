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
		//���Ľ��������ݻ�ԭ
		TEST_METHOD(Tree_Function)
		{
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\zhu";
			File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xin";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xi";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\hong";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\mo";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xin-guang";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			Data_Sift(File_path, 1, 2);
		}
		TEST_METHOD(Tree_Function_2)
		{
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\zhu";
			File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xin";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xi";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\hong";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\mo";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\xin-guang";
			//File_path = "C:\\Users\\29171\\source\\repos\\YSW-LJH\\Elevator\\��������\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			Data_Sift(File_path, 1, 2);
		}
	};
}
