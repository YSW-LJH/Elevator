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

//std::string File_path;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		//���Ľ��������ݻ�ԭ
		TEST_METHOD(Tree_Function)
		{
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin_1_4.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\zhu_1_2_20_30_32_33.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xi-1-3-5-7gd.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\hong-1-2-3gd.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo_1_2_4.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\xin-guang-1-10-20-21.txt";
			//File_path = "D:\\YSW\\Desktop\\Elevator\\��������\\mo-guang-1-2-3-4.txt";
			const char* File_path = "D:\\YSW\\Desktop\\Elevator\\AppTest";
			//Floor_height = 4;
			//Floor_begin = 1;
			//Floor_end = 4;
			//tree_main(File_path);
			//floor_main();
			//floor_verify();
			//floor_compare_main();
			//door_main();
			//Data_Restore("D:\\YSW\\Desktop\\Elevator\\��������\\out_xin.txt");
			//tree_delete();
			Signal_Analyse(File_path);
			//Assert().AreEqual(9, 8);
		}
	};
}
