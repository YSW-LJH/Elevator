module;
#include <func.h>
export module Main_process;
export map<string,map<string,string>> Signal_Analyse(string path)
{
	return call(path);
}