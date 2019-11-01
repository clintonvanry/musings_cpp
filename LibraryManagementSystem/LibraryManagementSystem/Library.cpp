#include "Library.h"

using namespace std;

map<int, Book> Library::s_bookMap;
map<int, Customer> Library::s_customerMap;

string Library::s_binaryPath(".\\Library.binary");

bool Library::lookupBook(const std::string& author, const std::string& title, Book* bookPtr)
{
	for(auto& entry : s_bookMap) // investigate const pair<int,Book>&
	{
		auto& book = entry.second;
		if(book.Author() == author)
		{
			if(bookPtr != nullptr)
			{
				*bookPtr = std::move(book); // investigate further std::move thin this due to assignment and copy constructor
			}
			return true;
		}
	}

	return false;
}

bool Library::lookupCustomer(const std::string& name, const std::string& address, Customer* customerPtr)
{
	for (auto& entry : s_customerMap) {
		auto& customer = entry.second;

		if ((customer.name() == name) &&(customer.address() == address)) 
		{
			if (customerPtr != nullptr) 
			{
				*customerPtr = std::move(customer); // investigate
			}

			return true;
		}
	}

	return false;
}

void Library::addBook()
{
	
}

void Library::deleteBook()
{
}
