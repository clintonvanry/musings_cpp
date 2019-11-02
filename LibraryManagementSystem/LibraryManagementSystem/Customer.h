#pragma once
#include <string>
#include <set>

class Customer
{
public:
	Customer() = default;
	Customer(std::string name, std::string address);

	// disallow assignment and pass by value
	Customer(const Customer& src) = delete;
	Customer& operator=(const Customer& rhs) = delete;

	// explicit default move constructor and assignment
	Customer(Customer&& src) = default;
	Customer& operator=(Customer&& rhs) = default;

	void read(std::ifstream& inStream);
	void write(std::ofstream& outStream) const;

	void borrowBook(int bookId)
	{
		m_loanSet.insert(bookId);
	}
	void reserveBook(int bookId)
	{
		m_reservationSet.insert(bookId);
	}
	void returnBook(int bookId)
	{
		m_loanSet.erase(bookId);
	}
	void unreserveBook(int bookId)
	{
		m_reservationSet.erase(bookId);
	}
	bool hasBorrowed(void) const
	{
		return !m_loanSet.empty();
	}

	const std::string& name() const { return m_name; }
	const std::string& address() const { return m_address; }
	int id() const { return m_customerId; }

	static int MaxCustomerId;
	friend std::ostream& operator<<(std::ostream& outStream,const Customer& customer);

private:
	int m_customerId = 0;
	std::string m_name;
	std::string m_address;
	std::set<int> m_loanSet, m_reservationSet;
};

