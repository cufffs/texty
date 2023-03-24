#ifndef STRING_H_
#define STRING_H_
/*
	String.h contains class: String, namespace myString as well as many overloads pertaining to the String class
	external functions used: cout/cin(+variants), assert

	Memory Management:
			all memory is allocated in blocks of size ALLOC_SIZE (defined below), this is to reduce the need for expensive
			functions like new/delete
			Memory does NOT shrink, if a string gets to say 512bytes and then only holds 30bytes of data, no reallocation will be used
			instead i've made a manual method of freeing its memory with [string = ""]
			Reason for not using autoreallocation: for my game usage, it will only raise to a certain max size of maybe 256bytes, and
			i dont want to call unessary news/deletes if it shrinks and raises and shrinks and raises etc
			
			function:_alloc(int) is the main way this classes allocates memory. THOUGH some methods use new for performance reasons (avoiding extra copy)
					:~String() destructor is the main way memory is freed

	Differences between assignment specifications:
			overloaded more operators (-=, -, [] may be a little different)
			NEW FUNCTIONS: RemoveLast(), Formatted(), Append(char)
			Class can be inherited, WriteToConsole and ReadFromConsole are virtual
			Most methods have two variants, one takes in a String whilst the other a char*, this if for convience when calling
											ie Append("this") instead of String x = "this; Append(x);

	READ/WRITE:
			I ultimately switched from <iostream> and cin.getline/cout to <stdio.h> and printf_s/fgets for the sake of not including the huge
			iostream libraries
			Since my game didnt include <iostream> i thought it easy to save 4-5kbs and remove it from string class too!
			(not as bad as you think) both input output are virtual so it is very trivial to override them!
			The problem with printf is that it isnt typesafe, however that is not an issue for my string class, so this in my
			opinion is a valid replacement for cout!

	UTILITIES:
		this class doesnt use things like strcmp, strlen etc it defines its own, all use a _(to say its member)
		ie _strcmp, _strlen, _strcpy are all private methods of string
		they may act slightly different compared to they're standard versions
*/
const int MAX_INPUT = 255;
const int ALLOC_SIZE = 64;

namespace myString
{
	class	String;		//a little more readable
	const	String	operator+	(const String& lhs, const String& rhs);		//return copy of lhs+rhs (two strings)(append)
	const	String	operator+	(const String& lhs, const char* rhs);		//return copy of lhs+rhs (string+cstring)(append)
	const	String	operator-	(const String& lhs, const String& rhs);		//return copy of lhs with all occurrences of rhs removed (two strings)
	const	String	operator-	(const String& lhs, const char* rhs);		//return copy of lhs with all occurrences of rhs removed (string+cstring)

	class String{
	public:
		String();										//default: no memory allocated
		String(char* str);								//copy cstring values			allocates blocks of ALLOC_SIZE
		String::String(const String& str);				//copy another strings value	(memory allocation sizes may vary)
		String(String&& rhs);							//move constructor
		~String();

		char* CStr()			const;					//returns a ptr to our actual cstring
		int	Length()			const;					//return number of chars before the null terminator
		char CharAt(int index)	const;					//returns a copy of char at an index
		void Clear();									//writes null to entire allocated area (keeps all allocated memory)
		void RemoveLast();								//simple removes last char (ie abc\0 ==> ab\0

		bool EqualTo(const String& str) const;			//returns true if two strings are equal, case sensitive
		bool EqualTo(const char* str)	const;			//returns true if string matches cstring, case sensitive

		void Append(const String& str);					//inserts String at the end, allocates more memory only if needed
		void Append(const char* str);					//inserts cstring at end, allocates more memory only if needed
		void Append(char c);							//inserts a single char at end, allocates more memory only if needed

		void Prepend(const String& str);				//inserts String at begining, allocates more memory only if needed
		void Prepend(const char* str);					//inserts cstring at begining, allocates more memroy only if needed

		int Find(const String& str)				const;				//Finds the str, within our string, returns index where its found
		int Find(const char* str)				const;				//
		int Find(int index, const String& str)	const;				//Finds the str, within our string starting from index
		int Find(int index, const char* str)	const;				//
		void Replace(const String& find, const String& replace);	//Replaces all occurrences of find with replace
		void Replace(const char* find, const char* replace);		//

		const String ToLower()			const;			//returns lowercase copy			all three are EXPENSIVE!
		const String ToUpper()			const;			//returns upercase copy
		const String Formatted()		const;			//returns formatted copy

		virtual void WriteToConsole()	const;			//writes string to console with a newline via printf
		virtual bool ReadFromConsole();					//Reads a line from console (fgets)

		virtual char& operator[](int index)		const;	//returns reference to whatever index points to, my assert if index is bad
		virtual String& operator=(const String rhs);	//copies rhs string, allocates memory if needed
		virtual String& operator=(const char* str);		//same as above,but, if str is null, then it will free our allocated memory
		bool operator==	(const String& rhs)		const;	//compare with another String object
		bool operator==	(const char* rhs)		const;	//compare with a cstring
		void operator+=	(const String& rhs);			//append string rhs to lhs
		void operator+=	(const char* rhs);				//...
		void operator-=	(const String& rhs);			//remove all occurrences of rhs from lhs
		void operator-=	(const char* rhs);

		friend void	_swap(String& lhs, String& rhs);

	protected:
		char*	_cstr;										//this is our string
		int		_memAllocated;								//how much mem we have allocated

		/*Utilites*/
		void	_alloc(int min);							//allocates memory and then copies data to it, return false on bad_alloc(never happens)
		void	_strcpy(const char* src);					//copies src to string member, calls _alloc if needed
		void	_strcpy(const char* src, char* dest);		//copies one src to dest, no tests - unsafe!
		int		_strlen(const char* str)	const;			//return length of str up to null terminate
		char	_toLower(char c)			const;			//returns character in lower case
		char	_toUpper(char c)			const;			//to upper
		bool	_isint(char c)				const;			//returns true if char is an int
		int		_countstr(const char* str)	const;			//counts str in data, returns how many str is in data
	};
}
#endif