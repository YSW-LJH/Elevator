#include "pch.h"
#include "CppUnitTest.h"
#include "func.h"

import Main_process;
import Formate;
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
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\zhu";
			File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xin";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xi";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\hong";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\mo";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xin-guang";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\mo-guang";
			//Signal_Analyse(File_path);
			//formate_main(File_path);
			//vector<string>str = string_splite(File_path, "\\");
			map<string, map<string, string>> a = Signal_Analyse(File_path);
			string b = "ID&Bit:";
			b += a["Floor"]["ID&Bit"] + "\n";
			b += "Pos:";
			b += a["Floor"]["Pos"] + "\n";
			b += "Range:";
			b += a["Floor"]["Range"] + "\n";
			b += "Increment:";
			b += a["Floor"]["Increment"];
			//
			b += "{Splite}Open:";
			b += a["Door"]["Open"] + "\n";
			b += "Run_O:";
			b += a["Door"]["Run_O"] + "\n";
			b += "Run_C:";
			b += a["Door"]["Run_C"] + "\n";
			b += "Close:";
			b += a["Door"]["Close"];
			//
			b += "{Splite}Up:";
			b += a["Run"]["Up"] + "\n";
			b += "Down:";
			b += a["Run"]["Down"];
			b += "";
		}
	};
}
