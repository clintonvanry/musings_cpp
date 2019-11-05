#pragma once
#include <string>
#include <set>

class Customer
{
public:
	Customer() = default;
	virtual ~Customer() = default;
	
	Customer(std::string_view name, std::string_view address);

	// disallow assignment and pass by value
	Customer(const Customer& src) = delete;
	Customer& operator=(const Customer& rhs) = delete;

	// explicit default move constructor and assignment
	Customer(Customer&& src) = default;
	Customer& operator=(Customer&& rhs) = default;

	void read(std::ifstream& inStream);
	void write(std::ofstream& outStream) const;

	void borrowBook(Book* book)
	{
		m_loanSet.insert(book);
	}
	void reserveBook(Book* book)
	{
		m_reservationSet.insert(book);
	}
	void returnBook(Book* book)
	{
		m_loanSet.erase(book);
	}
	void unreserveBook(Book* book)
	{
		m_reservationSet.erase(book);
	}

	[[nodiscard]] auto hasBorrowed() const	{return !m_loanSet.empty();	}
	[[nodiscard]] auto name() const { return m_name; }
	[[nodiscard]] auto address() const { return m_address; }
	[[nodiscard]] std::set<Book*> BooksBorrowed() const { return m_loanSet; }
	[[nodiscard]] std::set<Book*> ReservationList() const { return m_reservationSet; }
	
	friend std::ostream& operator<<(std::ostream& outStream,const Customer& customer);

private:
	
	std::string_view m_name;
	std::string_view m_address;
	std::set<Book*> m_loanSet, m_reservationSet;
};

