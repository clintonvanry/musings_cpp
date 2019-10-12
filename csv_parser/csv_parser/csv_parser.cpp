/*
 * Requirements for the csv parser:
 * 1) Each record will occupy one line, and newline indicates a new record
 * 2) Fields in the record are separated by commas, unless they are within a quoted string
 * 3) Strings can be quoted using single (') or double quotes ("), in which case they can contain commas as part of the string
 * 4) Quotes immediately repeated ('' or "") is a literal, and a part of the string rather than a delimiter of a string
 * 5) If a string is quoted, then spaces outside of the string are ignored
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>


using namespace  std;
using vec_str = vector<string>;
using list_str = list<string>;
using vec_list = vector<list_str>;


list_str parse_line(const string& line)
{
	list_str data;
	auto it = line.begin();
	string item;

	auto quote  = false;
	auto doubleQuote = false;

	do
	{
		switch (*it)
		{
			case '\'':
			{
					if(quote)
					{
						item.push_back(*it);
					}
					else
					{
						if ((it + 1) != line.end() && *(it + 1) == '\'') 
						{
							item.push_back(*it);
							++it;
						}
						else 
						{
							doubleQuote = !doubleQuote;
							if (doubleQuote)
							{
								item.clear();
							}
						}
					}
				break;
			}
			case '"':
			{
				if (doubleQuote)
				{
					item.push_back(*it);
				}
				else
				{
					if ((it + 1) != line.end() && *(it + 1) == '"')
					{
						item.push_back(*it);
						++it;
					}
					else
					{
						quote = !quote;
						if (quote)
						{
							item.clear();
						}
					}
				}
				break;
			}
			case ',':
			{
				if (quote || doubleQuote)
				{
					item.push_back(*it);  // NOLINT
				}
				else 
				{
					data.push_back(move(item));
				}
				break;
				
			}
			default:
			{
				item.push_back(*it);
			}
		}
		++it;
		
	} while (it != line.end());

	data.push_back(move(item));
	
	return data;
}



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
	fileStream.close();

	vec_list parsed;
	for(auto& line : lines)
	{
		parsed.push_back(move(parse_line(line)));
	}

    auto count = 0;
	for (auto row : parsed)
	{
		cout << ++count << ">| ";
		for (auto field : row)
		{
			cout << field << "| ";
		}
		cout << endl;
	}
	
	return 0;
}

