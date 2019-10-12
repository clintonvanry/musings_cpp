// csv_parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>


using namespace  std;
using vec_str = vector<string>;
using list_str = list<string>;
using vec_list = vector<list_str>;

void print_usage()
{
	cout << "usage: csv_parser file " << endl;
	cout << "where file is the path to a csv file" << endl;
}

int main(int argc, const char* argv[])
{
    if(argc < 1)
    {
		print_usage();
		return 1;
    }

	ifstream fileStream;
	fileStream.open(argv[1], ios_base::in);
	if(!fileStream.is_open())
	{
		print_usage();
		cout << "cannot open file " << argv[1] << endl;
		return 1;
	}
	
	vec_str lines;
	for(string line; getline(fileStream, line);)
	{
		if(line.empty())
		{
			continue;
		}
		lines.push_back(move(line)); // as we are not using the string after this for loop use the move semantics
	}

	return 0;
}

