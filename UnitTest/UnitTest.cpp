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
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\zhu";
			File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xi";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\hong";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin-guang";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			Data_Sift(File_path, 0, 2);

		}
	};
}
