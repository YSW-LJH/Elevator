#include "pch.h"
#include "CppUnitTest.h"
//#include "Tree.h"
#include "func.h"
//extern std::string file_path;
//extern int Floor_height;
//extern int Floor_begin;
//extern int Floor_end;
import Main_process;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::string File_path;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		//树的建立与数据还原
		TEST_METHOD(Tree_Function)
		{
			File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xin";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\zhu";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xi";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\hong";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\mo";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\xin-guang";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\测试数据\\mo-guang";
			Signal_Analyse(File_path);
		}
	};
}
