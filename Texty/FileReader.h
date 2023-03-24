#pragma once
/*
FileReader.H - class FileReader [static]
by Callum Buerckner

Basically to read a file into a struct without knowing the struct
Will allocate enough memory to read the file, and then read it filling the given struct
requires, pointer reference to struct, size of struct, and format of struct aswell as a filename to be read

example
format: "%s32%i%i" => struct unknown { char s[32]; int; int }
format  "%i%c%p3"  => struct unknown { int; char; <padding of size 3> }
*/
const int MAX_FILE_NAME = 64;
class FileReader{
public:
	static int Read(void*& strct, int size, const char* fmt, const char* fname);	//copy data from a file to a given struct
	static int CountLines(const char* fname);										//count lines in a file
private:
	FileReader(){}																	//static class, no instances
	static int _countchar(char find, const char* str);								//count how many times find appears in a cstring
	static int _getfmtstrlen(char* &fmtptr);										//get int from our fmt string ie "%s32" ==> 32
};