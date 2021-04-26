#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>

namespace file {

	class csv {
	public:
		class line {
		public:
			int id;			// Location row
		public:
			int columns;	// Number of columns
			char** pdata;
			char* data;

		public:
			~line() { delete[] data; delete[] pdata; }
			void init(std::istream& inp);
			line& operator=(const line& x);
		};

	public:
		int rows;			// Number of rows
		line mark;
		line* data;

	public:
		csv(const char* FILE);
		~csv() { delete[] data; }
	};


	bool exists(const char* FILE);
	csv::line* find(csv& file, const char* data1, const char* data2 = nullptr, bool status = FALSE);
}

#endif