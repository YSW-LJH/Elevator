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
		//���Ľ��������ݻ�ԭ
		TEST_METHOD(Tree_Function)
		{
			File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\zhu";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xi";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\hong";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin-guang";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo-guang";
			Signal_Analyse(File_path);
		}
	};
}
