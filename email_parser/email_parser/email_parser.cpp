// email_parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> 
#include <fstream> 
#include <string>
#include <utility>
#include <vector>

using namespace std;

class header_body
{
	string body;
public:
	header_body() = default;
	header_body(string b) : body(std::move(b)) {}
	string get_body() const { return body; }
};

// RFC 5322
// headers are one per line in form name : value
//   may be "folded white space"
//   may contain subitems
//   not unique and later ones may add to or supercede earlier ones 
// body is after  first blank line after headers, ignore newlines
// 
class email
{
	using iter = vector<pair<string, header_body>>::iterator;
	vector<pair<string, header_body>> headers;
	string body;

public:
	email() : body("") {}

	// accessors 
	string get_body() const { return body; }

	string get_headers() const
	{
		// aggregate all headers as a single string
		string all;
		for (auto a : headers)
		{
			all += a.first + ": " + a.second.get_body();
			all += "\n";
		}
		return all;
	}
	
	iter begin() { return headers.begin(); }
	iter end() { return headers.end(); }

	// two stage construction 
	void parse(istream& fin);
private:
	void process_headers(const vector<string>& lines);
};

void email::parse(istream& fin)
{
	string line;
	vector<string> headerLines;
	while (getline(fin, line))
	{
		if (line.empty())
		{
			// end of headers 
			break;
		}
		headerLines.push_back(line);
	}

	process_headers(headerLines);

	while (getline(fin, line))
	{
		if (line.empty()) body.append("\n");
		else body.append(line);
	}
}

void email::process_headers(const vector<string>& lines)
{
	string header = "";
	string body = "";
	for (string line : lines)
	{
		if (isspace(line[0])) body.append(line);
		else
		{
			if (!header.empty())
			{
				headers.push_back(make_pair(header, body));
				header.clear();
				body.clear();
			}

			size_t pos = line.find(':');
			header = line.substr(0, pos);
			pos++;
			while (isspace(line[pos])) pos++;
			body = line.substr(pos);
		}
	}

	if (!header.empty())
	{
		headers.push_back(make_pair(header, body));
	}
}


void print_usage()
{
	cout << "usage: email_parser file" << "\n";
	cout << "where file is the path to a file" << "\n";
}

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		print_usage();
		return 1;
	}

	ifstream filestream;
	filestream.open(argv[1], ios_base::in);
	if (!filestream.is_open())
	{
		print_usage();
		cout << "cannot open " << argv[1] << "\n";
		return 1;
	}

	email eml;
	eml.parse(filestream);

	cout << eml.get_headers();
	
	/*
	for (auto header : eml)
	{
		cout << header.first << " : ";
		vector<pair<string, string>> subItems = header.second.subitems();
		if (subItems.size() == 0)
		{
			cout << header.second.get_body() << "\n";
		}
		else
		{
			cout << "\n";
			for (auto sub : subItems)
			{
				cout << "   " << sub.first;
				if (!sub.second.empty())
				{
					cout << " = " << sub.second;
				}
				cout << "\n";
			}
		}
	}
	*/
	
	
	cout << "\n";
	cout << eml.get_body() << endl;

	return 0;
}

