#include <fstream>
#include <utility>
#include "Book.h"
#include "Library.h"

int Book::MaxBookId = 0;

Book::Book(string author, string title): m_author(std::move(author)), m_title(std::move(title)), m_bookId(++Book::MaxBookId)
{
}

void Book::read(ifstream& inStream)
{
	inStream.read(reinterpret_cast<char*>(&m_bookId), sizeof m_bookId);

	getline(inStream, m_author);
	getline(inStream, m_title);

	inStream.read(reinterpret_cast<char*>(&m_bookIsBorrowed), sizeof m_bookIsBorrowed);
	inStream.read(reinterpret_cast<char*>(&m_customerId), sizeof m_customerId);

	
	auto reserveListSize = 0;

	inStream.read(reinterpret_cast<char*>(&reserveListSize),sizeof reserveListSize);

	for (auto count = 0; count < reserveListSize; ++count) 
	{
		auto customerId = 0;
		inStream.read(reinterpret_cast<char*>(&customerId), sizeof customerId);
		m_reservationList.push_back(customerId);
	}
	
	
}

void Book::write(ofstream& outStream) const
{

	outStream.write(reinterpret_cast<char*>(const_cast<int*>(&m_bookId)), sizeof m_bookId);

	outStream << m_author << endl;
	outStream << m_title << endl;

	outStream.write(reinterpret_cast<char*>(const_cast<bool*>(&m_bookIsBorrowed)), sizeof m_bookIsBorrowed);
	outStream.write(reinterpret_cast<char*>(const_cast<int*>(&m_customerId)), sizeof m_customerId);

	
	auto reserveListSize = m_reservationList.size();

	outStream.write(reinterpret_cast<char*>(const_cast<unsigned*>(&reserveListSize)),sizeof reserveListSize);

	for (auto customerId : m_reservationList) 
	{
		outStream.write(reinterpret_cast<char*>(const_cast<int*>(&customerId)), sizeof customerId);
	}

	
}


void Book::borrowBook(int customerId)
{
	m_bookIsBorrowed = true;
	m_customerId = customerId;
	
}

int Book::reserveBook(int customerId)
{
	m_reservationList.push_back(customerId);
	return  m_reservationList.size();
}

void Book::unreserveBook(int customerId)
{
	m_reservationList.remove(customerId);
}

void Book::returnBook()
{
	m_bookIsBorrowed = false;
}


ostream& operator<<(ostream& outStream, const Book& book)
{
	outStream <<"book id: " << book.bookId() << endl << " \"" << book.m_title << "\" by " << book.m_author;

	if (book.m_bookIsBorrowed) 
	{
		outStream << endl << "  Borrowed by: " << Library::s_customerMap[book.m_customerId].name()	<< ".";
	}

	if (!book.m_reservationList.empty()) 
	{
		outStream << endl << "  Reserved by: ";

		auto first = true;
		for (auto customerId : book.m_reservationList) 
		{
			outStream << (first ? "" : ",") << ""	<< Library::s_customerMap[customerId].name();
			first = false;
		}

		outStream << ".";
	}

	return outStream;
}
