// 电梯编码预处理.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <fstream>
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
	int mode = -1;
	int height = -1;
	string path;
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
		cout << "输入文件名:\n";//不含后缀，仅支持.txt
		cin >> filename;
	Mode_input:
		cout << "输入处理模式：\n";
		cin >> mode;
		switch (mode)
		{
		case 0:
			cout << "输入当前处理文件的楼层数" << endl;
			cin >> height;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			cout << "输入当前处理文件的楼层数:" << endl;
			cin >> height;
			break;
		default:
			cout << "模式输入错误  ＞︿＜  请重新输入模式代码\n";
			goto Mode_input;
		}
		filename += ".txt";
		ifstream file;
		file.open(filename, ios::in);
		if (!file)
		{
			cout << "打开文件\"" << filename << "\"失败(＞﹏＜), 请检查文件是否存在" << endl;
			return 0;
		}
		file.close();
		if (call_data_process(filename, mode, height) == 0)
			cout << "处理完成(●'v'●),输出文件名为:" << filename << endl;
		else
			cout << "建树失败" << endl;
	}
	return 1;
Multiple:
	cout << "文件命名规则：文件名_起始楼层_结束楼层.txt\n\
    注：文件名不能包含‘_’!!!\n\
    如：竹韵_1_4.txt，新时达_3_1.txt，123_1_55.txt\n\
    如过是平层信息，可以命名为：竹韵_1.txt 或 竹韵_1_1.txt ，即起始楼层和结束楼层可以仅写一个\n\
    输入文件夹路径：\n";
	multiple_file_process();
	return 1;
Percent:
	cout << "##数据概率统计函数##\n输入一个文件夹路径:\n";
	cin >> path;
	cout << percent_main(path);
	return 1;
Formate:
	cout << "输入文件夹路径，格式化后的文件位于\"..\\格式化\"文件夹下\n";
	cin >> path;
	formate_main(path);
	return 1;
}