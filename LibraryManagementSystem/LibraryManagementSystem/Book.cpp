#include <fstream>
#include "Book.h"
#include "Library.h"


Book::Book(string_view author, string_view title): m_author(author), m_title(title)
{
}

void Book::read(ifstream& inStream)
{
	string author, title;
	getline(inStream, author); // dont know how to use string_view in getline nothing online about it
	getline(inStream, title);
	m_author = author;
	m_title = title;
}

void Book::write(ofstream& outStream) const
{
	outStream << m_author << endl;
	outStream << m_title << endl;
}


void Book::borrowBook(Customer* customer)
{
	m_bookIsBorrowed = true;
	m_customer = customer;
	
}

int Book::reserveBook(Customer* customer)
{
	m_reservationList.push_back(customer);
	return  m_reservationList.size();
}

void Book::unreserveBook(Customer* customer)
{
	m_reservationList.remove(customer);
}

void Book::returnBook()
{
	m_bookIsBorrowed = false;
	m_customer = nullptr;
}


ostream& operator<<(ostream& outStream, const Book& book)
{
	outStream << " \"" << book.m_title << "\" by " << book.m_author;

	const auto customerBorrowedBook = book.customer();
	if (book.m_bookIsBorrowed && customerBorrowedBook != nullptr) 
	{
		outStream << endl << "  Borrowed by: " << customerBorrowedBook->name()<< ".";
	}

	if (!book.m_reservationList.empty()) 
	{
		outStream << endl << "  Reserved by: ";

		auto first = true;
		for (auto customer : book.m_reservationList) 
		{
			outStream << (first ? "" : ",") << ""	<< customer->name();
			first = false;
		}

		outStream << ".";
	}

	return outStream;
}
