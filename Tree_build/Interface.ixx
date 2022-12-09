module;
#include <func.h>
export module Main_process;
export void call_data_process(string filename, int mode, int height, int windows_size)
{
	call(filename, mode, height, windows_size);
}