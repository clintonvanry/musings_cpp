#pragma once
#include <string>
#include <list>

using namespace  std;

class Book
{
	string m_author;
	string m_title;
	int m_customerId = 0;
	int m_bookId = 0;
	list<int> m_reservationList;
	bool m_bookIsBorrowed = false;
	
public:
	static int MaxBookId;
	
	// constructor
	Book() = default;
	virtual ~Book() = default;
	Book(string author, string title);

	// disallow assignment and pass by value
	Book(const Book& src) = delete;
	Book& operator=(const Book& rhs) = delete;

	// explicit default move constructor and assignment
	Book(Book&& src) = default;
	Book& operator=(Book&& rhs) = default;
	
	// methods
	void read(ifstream& inStream);
	void write(ofstream& outStream) const;

	void borrowBook(int customerId);
	int reserveBook(int customerId);
	void unreserveBook(int customerId);
	void returnBook();

	// inline statements
	int bookId() const
	{
		return m_bookId;
	}

	bool borrowed() const
	{
		return m_bookIsBorrowed;
	}

	int customerId() const
	{
		return m_customerId;
	}

	const string& Author() const
	{
		return m_author;
	}

	const string& Title() const
	{
		return m_title;
	}

	list<int>& reservationList()
	{
		return m_reservationList;
	}

	friend ostream& operator<<(ostream& outStream, const Book& book);
};

