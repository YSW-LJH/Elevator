#include "pch.h"
#include "CppUnitTest.h"
//#include "Tree.h"
#include "func.h"
extern std::string File_path;
extern int Floor_height;
extern int Floor_begin;
extern int Floor_end;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		//���Ľ��������ݻ�ԭ
		TEST_METHOD(Tree_Function)
		{
			File_path = "D:\\Code\\Elevator\\��������\\xin-1-4.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\zhu-1-2-20-32-33.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\xi-1-3-5-7gd.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\hong-1-2-3gd.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\mo-1-2-4L.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\xin-guang-1-10-20-21.txt";
			//File_path = "D:\\Code\\Elevator\\��������\\mo-guang-1-2-3-4.txt";
			Floor_height = 4;
			Floor_begin = 1;
			Floor_end = 4;
			tree_main();
			floor_main();
			floor_verify();
			//door_main();
			Data_Restore("D:\\Code\\Elevator\\��������\\out_xin.txt");
			tree_delete();
			//Assert().AreEqual(9, 8);
		}
	};
}
