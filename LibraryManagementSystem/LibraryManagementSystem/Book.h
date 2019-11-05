#pragma once
#include <string>
#include <list>


using namespace  std;

class Customer;

class Book
{
	string_view m_author;
	string_view m_title;
	Customer* m_customer = nullptr;
	list<Customer*> m_reservationList;
	bool m_bookIsBorrowed = false;
	
public:
	
	// constructor
	Book() = default;
	virtual ~Book() = default;
	Book(string_view author, string_view title);

	// disallow assignment and pass by value
	Book(const Book& src) = delete;
	Book& operator=(const Book& rhs) = delete;

	// explicit default move constructor and assignment
	Book(Book&& src) = default;
	Book& operator=(Book&& rhs) = default;
	
	// methods
	void read(ifstream& inStream);
	void write(ofstream& outStream) const;

	void borrowBook(Customer* customer);
	int reserveBook(Customer* customer);
	void unreserveBook(Customer* customer);
	void returnBook();

	
	
	[[nodiscard]] auto borrowed() const
	{
		return m_bookIsBorrowed;
	}

	[[nodiscard]] Customer* customer() const
	{
		return m_customer;
	}

	[[nodiscard]] auto Author() const
	{
		return m_author;
	}

	[[nodiscard]] auto Title() const
	{
		return m_title;
	}

	[[nodiscard]] auto& ReservationList() const 
	{
		return m_reservationList;
	}

	friend ostream& operator<<(ostream& outStream, const Book* book);
};

