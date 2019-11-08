#include "Book.h"
#include "Customer.h"
#include <iostream>
#include <utility>
#include <fstream>

using namespace  std;

Book::Book()
{
	cout << "Book() default constructor" << endl;
}

Book::Book(string title, string author) : m_title(std::move(title)), m_author(std::move(author))
{
	cout << "Book(string title, string author)" << endl;
}

Book::~Book()
{
	// cleanup pointers and stuff
	cleanup();

	cout << "~Book()" << endl;
}

Book::Book(const Book& src) : m_title(src.m_title), m_author(src.m_author)
{
	cout << "Book(const Book& src)" << endl;
}

Book& Book::operator=(const Book& rhs)
{
	cout << "Book::operator=(const Book& rhs)" << endl;
	if (this == &rhs)
	{
		return *this;
	}

	auto book(rhs); // do the work in the temp object
	swap(*this, book);
	return *this;
}

// move constructor
Book::Book(Book&& src) noexcept : Book()
{
	cout << "Book(Book&& src)" << endl;
	swap(*this, src);
}

Book& Book::operator=(Book&& rhs) noexcept
{
	cout << "Book::operator=(Book&& rhs)" << endl;
	auto tmp(std::move(rhs));
	swap(*this, tmp);
	return *this;
}


void Book::cleanup() noexcept
{
	cout << "cleanup()" << endl;

	// reset the object
	m_title = "";
	m_author = "";

	if (m_customer) 
	{
		m_customer.reset();
	}
	m_reservationList.clear();

}


void swap(Book& first, Book& second) noexcept
{
	cout << "swap(Book& first, Book& second)" << endl;
	std::swap(first.m_title, second.m_title);
	std::swap(first.m_author, second.m_author);
	std::swap(first.m_customer, second.m_customer);
	std::swap(first.m_reservationList, second.m_reservationList);
}

std::ostream& operator<<(std::ostream& outStream, const std::shared_ptr<Book>& book)
{
	if (book == nullptr)
	{
		return outStream;
	}

	outStream << " \"" << book->Title() << "\" by " << book->Author();

	const auto customerBorrowedBook = book->customer();
	if (book->borrowed() && customerBorrowedBook != nullptr)
	{
		outStream << endl << "  Borrowed by: " << customerBorrowedBook->Name() << ".";
	}

	auto reservationList = book->ReservationList();
	if (!reservationList.empty())
	{
		outStream << endl << "  Reserved by: ";

		auto first = true;
		for (const auto& customer : reservationList)
		{
			outStream << (first ? "" : ",") << "" << customer->Name();
			first = false;
		}

		outStream << ".";
	}

	return outStream;
}


void Book::returnBook()
{
	m_bookIsBorrowed = true;
	m_customer = nullptr;
}

void Book::borrowBook(std::shared_ptr<Customer> customer)
{
	m_bookIsBorrowed = true;
	m_customer = customer;
}

int Book::reserveBook(const std::shared_ptr<Customer>& customer)
{
	m_reservationList.push_back(customer);
	return  m_reservationList.size();
}

void Book::unreserveBook(const std::shared_ptr<Customer>& customer)
{
	m_reservationList.remove(customer);
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