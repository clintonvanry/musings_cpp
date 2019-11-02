#include <fstream>
#include "Book.h"
#include "Library.h"

int Book::MaxBookId = 0;

Book::Book(const string& author, const string& title): m_author(author), m_title(title), m_bookId(++Book::MaxBookId)
{
}

void Book::read(ifstream& inStream)
{
	inStream.read((char*)&m_bookId, sizeof m_bookId);

	getline(inStream, m_author);
	getline(inStream, m_title);

	inStream.read((char*)&m_bookIsBorrowed, sizeof m_bookIsBorrowed);
	inStream.read((char*)&m_customerId, sizeof m_customerId);

	
	int reserveListSize;

	inStream.read((char*)&reserveListSize,sizeof reserveListSize);

	for (int count = 0; count < reserveListSize; ++count) 
	{
		int customerId;
		inStream.read((char*)&customerId, sizeof customerId);
		m_reservationList.push_back(customerId);
	}
	
	
}

void Book::write(ofstream& outStream) const
{

	outStream.write((char*)&m_bookId, sizeof m_bookId);

	outStream << m_author << endl;
	outStream << m_title << endl;

	outStream.write((char*)&m_bookIsBorrowed, sizeof m_bookIsBorrowed);
	outStream.write((char*)&m_customerId, sizeof m_customerId);

	
	int reserveListSize = m_reservationList.size();

	outStream.write((char*)&reserveListSize,
	sizeof reserveListSize);

	for (int customerId : m_reservationList) 
	{
		outStream.write((char*)&customerId, sizeof customerId);
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
	outStream << "\"" << book.m_title << "\" by " << book.m_author;

	if (book.m_bookIsBorrowed) 
	{
		outStream << endl << "  Borrowed by: "
			<< Library::s_customerMap[book.m_customerId].name()
			<< ".";
	}

	if (!book.m_reservationList.empty()) 
	{
		outStream << endl << "  Reserved by: ";

		bool first = true;
		for (int customerId : book.m_reservationList) 
		{
			outStream << (first ? "" : ",") << ""	<< Library::s_customerMap[customerId].name();
			first = false;
		}

		outStream << ".";
	}

	return outStream;
}
