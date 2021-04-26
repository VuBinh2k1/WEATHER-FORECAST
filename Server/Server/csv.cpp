#include "stdafx.h"
#include "csv.h"

using file::csv;

void csv::line::init(std::istream& inp) {
	char temp[1000];
	inp.getline(temp, 1000);

	columns = std::count(temp, temp + strlen(temp), ',') + 1;
	data = new char[strlen(temp) + 1];
	pdata = new char* [columns];

	strcpy_s(data, strlen(temp) + 1, temp);
	pdata[0] = data;
	for (int i = 0, cnt = 1; data[i] != '\0'; ++i) {
		if (data[i] == ',') {
			data[i] = '\0';
			pdata[cnt++] = data + i + 1;
		}
	}
}

csv::line& csv::line::operator=(const line& x) {
	int data_size = 0;
	for (int i = 0; i < x.columns; ++i) {
		data_size += strlen(x.pdata[i]) + 1;
	}

	delete[] data;
	delete[] pdata;

	id = x.id;
	columns = x.columns;
	data = new char[data_size];
	pdata = new char* [columns];

	pdata[0] = data;
	for (int i = 0, cnt = 1; i < data_size; ++i) {
		data[i] = x.data[i];
		if (cnt < columns && data[i] == '\0') {
			pdata[cnt++] = data + i + 1;
		}
	}
	return *this;
}

csv::csv(const char* FILE) {
	std::ifstream inp(FILE);
	if (!inp.is_open()) 
		throw std::runtime_error("data file is not exist");
	
	rows = std::count(std::istreambuf_iterator<char>(inp), std::istreambuf_iterator<char>(), '\n') - 1;
	inp.seekg(0, inp.beg);

	mark.init(inp);
	data = new line[rows];
	for (int i = 0; i < rows; ++i) {
		data[i].init(inp);
		data[i].id = i;
	}
	inp.close();
}

bool file::exists(const char* FILE) {
	if (FILE == nullptr) return 0;
	std::ifstream inp(FILE);
	if (!inp.is_open()) return 0;
	inp.close();
	return 1;
}

csv::line* file::find(csv& file, const char* data1, const char* data2, bool status) {
	for (int i = 0; i < file.rows; ++i) {
		if (status && file.data[i].pdata[0][0] == '0') continue;
		if (strcmp(file.data[i].pdata[1], data1) == 0) {
			if (data2 == nullptr || strcmp(file.data[i].pdata[2], data2) == 0) return &file.data[i];
		}
	}
	return nullptr;
}
