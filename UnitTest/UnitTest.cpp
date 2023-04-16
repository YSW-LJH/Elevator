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
		//树的建立与数据还原
		TEST_METHOD(Tree_Function)
		{
			File_path = "D:\\Code\\Elevator\\测试数据\\xin-1-4.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\zhu-1-2-20-32-33.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\xi-1-3-5-7gd.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\hong-1-2-3gd.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\mo-1-2-4L.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\xin-guang-1-10-20-21.txt";
			//File_path = "D:\\Code\\Elevator\\测试数据\\mo-guang-1-2-3-4.txt";
			Floor_height = 4;
			Floor_begin = 1;
			Floor_end = 4;
			tree_main();
			floor_main();
			floor_verify();
			//door_main();
			Data_Restore("D:\\Code\\Elevator\\测试数据\\out_xin.txt");
			tree_delete();
			//Assert().AreEqual(9, 8);
		}
	};
}
