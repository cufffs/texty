#include "FileReader.h"
#include <stdio.h>
#include <string>

int FileReader::CountLines(const char* fname)	//assumes there is at least one valid line^^
{
	FILE* fp;
	int c, lines = 1;
	fopen_s(&fp, fname, "r");				//open file
	if (!fp) return 0;						//if couldnt open file return false
	while ((c = fgetc(fp)) != EOF)			//while getc isnt endoffile
		if (c == '\n')						//if char is a new line , count it 
			++lines;
	fclose(fp);								//close file
	return lines;							//return count
}

int FileReader::Read(void*& strct, int size, const char* fmt, const char* fname)
{
	unsigned int numLines = CountLines(fname);		//read how many records we need to store
	if (!numLines) return 0;						//either no lines in file, or file couldnt be opened

	FILE* fp;
	fopen_s(&fp, fname, "r");						//open file (again)
	if (!fp) return 0;

	strct = operator new(numLines*size);

	char line[256] = { 0 }, buffer[256] = { 0 };			//some buffers to handle reading file
	char* fmtptr = (char*)fmt, *strctptr = (char*)strct;	//pointer to start of struct and start of formatting
	char* bufptr = nullptr, *lineptr = nullptr;

	while (fgets(line, sizeof(line), fp)) {			//while we get line
		lineptr = (char*)line;						//set lineptr to start of line
		while (*fmtptr){							//looop through our formatting, since we use this to copy in correct order
			bufptr = (char*)buffer;					//reset our buffer pointer
			if (*fmtptr == '%')						//move fmtptr to an interesting point
				++fmtptr;
			while (*lineptr && *lineptr != '\n' && *lineptr != '\t')	//copy data from line to buffer
				*bufptr++ = *lineptr++;
			*bufptr = 0, lineptr++;					//add null char
			switch (*fmtptr)						//switch formatting to handle different types of data
			{
			case 's':								//if its a string
			{
				int strlen = _getfmtstrlen(fmtptr);	//determine how much we need to offset
				strcpy_s(strctptr, strlen, buffer);	//copy buffer to struct
				strctptr += strlen;					//move structptr to pass the cstring variable within struct
				break;
			}
			case 'c':											//if its just a char
			{
				fmtptr++;										//must fmtptr++ for next iteration
				*(char*)strctptr = buffer[0];					//copy first char to struct
				strctptr += sizeof(char);						//move passed char
				break;
			}
			case 'i':											//if its an int
			{
				fmtptr++;										//must fmtptr++ for next iteration
				*(unsigned int*)strctptr = atoi(buffer);		//copy buffer to int
				strctptr += sizeof(unsigned int);				//move structptr past int
				break;
			}
			case 'p':											//if padding
			{
				fmtptr++;
				char c[2] = { *fmtptr++, 0 };					//padding shouldnt be more than 3 bytes
				strctptr += atoi(c);							//skip it some amount
				break;
			}
			}
		}
		fmtptr = (char*)fmt;									//next record, start over again
	}
	fclose(fp);
	return numLines;											//return how many records in struct
}

int FileReader::_countchar(char find, const char* str)	//counts char in a string - not used anymore
{
	char* cp = (char*)str;								//set pointer to start of string
	int counter = 0;
	while (*cp)											//while we have string
		if (*cp++ == find)								//if current pos == find
			++counter;									//increment counter
	return counter;
}

int FileReader::_getfmtstrlen(char* &fmtptr)
{
	char tmp[5] = { 0 };		//temp place to hold ints
	for (int i = 0; i < sizeof(tmp) && *fmtptr++ && *fmtptr != '\n' && *fmtptr != '\t' && *fmtptr != '%'; ++i)
		tmp[i] = *fmtptr;		//while its an int, copy it to our temp spot
	return atoi(tmp);			//return it as an int
}