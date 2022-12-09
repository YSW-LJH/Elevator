// 电梯编码预处理.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
import Main_process;
import Multiple_file_process;
import Percent;
import Formate;
using namespace std;

int main()
{
	system("color 03");
	//system("mkdir out");
	string filename;
	int window_size = -1;
	int mode = -1;
	int height = -1;
	cout << "输入处理模式：1（单文件），2（多文件找楼层），3（数据出现概率统计），4（CAN数据文件格式化）\n模式：";
	while (true)
	{
		cin >> mode;
		if (mode == 1)
			goto Single;
		else if (mode == 2)
			goto Multiple;
		else if (mode == 3)
			goto Percent;
		else if (mode == 4)
			goto Formate;
		else
			cout << "输入错误，请重新输入！\n模式：";
	}
Single:
	mode = -1;
	cout << "********说明********" << endl;
	cout << "以下输出内容数据部分为 十六 进制，统计部分为 十 进制" << endl;
	cout << "输入文件名(.txt文件);\n输出文件为“文件名_out.txt”\n" << endl;
	cout << "处理模式：\n\
	模式 0 ：输出以下全部内容\n\
	模式 1 ：输出已完成排序的和统计的全部内容\n\
	模式 2 ：在1的基础上输出对门信号的判断（未完成）\n\
	模式 3 ：在1的基础上输出对楼层信号的判断\n\
	模式 4 ：在1的基础上输出对定义大小滑窗的统计\n";
	while (1)
	{
		cout << "输入文件名:\n";
		cin >> filename;
	Mode_input:
		cout << "输入处理模式：\n";
		cin >> mode;
		switch (mode)
		{
		case 0:
			cout << "输入当前处理文件的楼层数" << endl;
			cin >> height;
			cout << "输入窗口大小(>=1且<=4):" << endl;
			cin >> window_size;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			cout << "输入当前处理文件的楼层数:" << endl;
			cin >> height;
			break;
		case 4:
			cout << "输入窗口大小(>=1且<=4):" << endl;
			cin >> window_size;
			break;
		default:
			cout << "模式输入错误  ＞︿＜  请重新输入模式代码\n";
			goto Mode_input;
		}
		call_data_process(filename, mode, height, window_size);
	}
	return 1;
Multiple:
	multiple_file_process();
	return 1;
Percent:
	percent_main();
	return 1;
Formate:
	formate_main();
	return 1;
}