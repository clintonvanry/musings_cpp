#pragma once
#include <string>
#include "Book.h"
#include <set>
#include <vector>

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

	friend std::ostream& operator<<(std::ostream& outStream, const std::shared_ptr<Customer>& customer);

	[[nodiscard]] std::string Name() const { return  m_name; }
	[[nodiscard]] std::string Address() const { return  m_address; }
	[[nodiscard]] auto hasBorrowed() const { return !m_loanSet.empty(); }
	[[nodiscard]] auto numberOfBooksBorrowed() const { return m_loanSet.size(); }
	[[nodiscard]] auto numberOfBooksReserved() const { return m_reservationSet.size(); }
	[[nodiscard]] auto& BooksBorrowed() const { return m_loanSet; }
	[[nodiscard]] auto& ReservationList() const { return m_reservationSet; }

	void borrowBook(std::weak_ptr<Book> book)
	{
		m_loanSet.push_back(book);
	}
	//void returnBook(const std::shared_ptr<Book>& book) { m_loanSet.erase(book); }

	void unreserveBook(const std::shared_ptr<Book>& book) { m_reservationSet.erase(book); }
	void reserveBook(const std::shared_ptr<Book>& book) { m_reservationSet.insert(book); }

	void read(std::ifstream& inStream);
	void write(std::ofstream& outStream) const;

private:
	std::string m_name, m_address;
	std::vector<std::weak_ptr<Book>> m_loanSet;
	std::set<std::shared_ptr<Book>> m_reservationSet;

	void cleanup() noexcept;
};

