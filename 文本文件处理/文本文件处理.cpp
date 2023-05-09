// 文本文件处理.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <main.h>
module;
#include <iostream>
#include <cstring>
#include <vector>
#include <direct.h>
#include <algorithm>
#include <io.h>
#include <fstream>
#include <iomanip>
module Formate;

using namespace std;

static void getFiles(string path, vector<string>& files);
static void process(string infile, string outfile);

void formate_main(string path)
{
    vector<string>files;
    getFiles(path, files);
    string cmd;
    path.append("\\格式化");
    cmd = "mkdir " + path;
    system(cmd.c_str());
    vector<string>::iterator file;
    for (file = files.begin(); file != files.end(); file++)
    {
        process(*file, path);
    }
}
static void process(string infile,string outfile)
{
    outfile += infile.substr(infile.rfind('\\'), infile.length() - infile.rfind('\\'));
    ifstream in;
    ofstream out;
    char buff[150];
    in.open(infile, ios::in);
    in.getline(buff, 150, '\n');
    out.open(outfile, ios::out);
    while (!in.eof())
    {
        in.getline(buff, 150, '\n');
        string data = "   ";
        int pos = 0;
        while (buff[pos] == ' ' && buff[pos] != 0) pos++;
        if (buff[pos] == 0)
            break;
        for (int i = 1;;i++)
        {
            if (i == 4)
            {
                for (; buff[pos] != ' '; pos++)
                    data.append(1, buff[pos]);
                data.append("   ");
            }
            if (i == 8)
            {
                for (; buff[pos] != '\n' && buff[pos] != 0; pos++)
                    data.append(1, buff[pos]);
                data.append("\n");
                out << data;
                break;
            }
            while (buff[pos] != ' ') pos++;
            while (buff[pos] == ' ') pos++;
        }
    }
    in.close();
    out.close();
}
static void getFiles(string path, vector<string>& files)
{
    intptr_t hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*.txt").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之 //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                //if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                //{
                //    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                //}
                continue;
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
