module;
#include <func.h>
export module Main_process;
export int call_data_process(string filename, int mode, int height)
{
	return call(filename, mode, height);
}