#pragma once
#include <string>
#include "Book.h"
#include <set>

class Customer
{
public:
	// Constructor
	Customer();
	virtual ~Customer();

	// user defined
	Customer(std::string name, std::string address);

	// remove copy and assignment
	Customer(const Customer& src) = delete;
	Customer& operator=(const Customer& rhs) = delete;

	// move and assignment
	Customer(Customer&& src) = default;
	Customer& operator=(Customer&& rhs) = default;

	friend std::ostream& operator<<(std::ostream& outStream, const Customer* customer);

	[[nodiscard]] std::string Name() const { return  m_name; }
	[[nodiscard]] std::string Address() const { return  m_address; }
	[[nodiscard]] auto hasBorrowed() const { return !m_loanSet.empty(); }
	[[nodiscard]] std::set<Book*> BooksBorrowed() const { return m_loanSet; }
	[[nodiscard]] std::set<Book*> ReservationList() const { return m_reservationSet; }

	void borrowBook(Book* book) { m_loanSet.insert(book); }
	void returnBook(Book* book) { m_loanSet.erase(book); }

	void unreserveBook(Book* book) { m_reservationSet.erase(book); }
	void reserveBook(Book* book) { m_reservationSet.insert(book); }

	void read(std::ifstream& inStream);
	void write(std::ofstream& outStream) const;

private:
	std::string m_name, m_address;
	std::set<Book*> m_loanSet, m_reservationSet;

	void cleanup() noexcept;
};

