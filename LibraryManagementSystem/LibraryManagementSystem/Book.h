#pragma once
#include <string>
#include <list>
#include <memory>

class Customer;

class Book
{
public:
	// constructors
	Book(); // default
	Book(std::string title, std::string author); // user defined

	// deconstructor
	virtual ~Book();

	//copy constructor
	Book(const Book& src);
	// assignment operator
	Book& operator=(const Book& rhs);


	// move semantics
	Book(Book&& src) noexcept; // move constructor
	Book& operator=(Book&& rhs) noexcept; // move assign

	friend void swap(Book& first, Book& second) noexcept; // copy and swap idiom
	friend std::ostream& operator<<(std::ostream& outStream, const std::shared_ptr<Book>& book);

	[[nodiscard]] auto Title() const { return m_title; }
	[[nodiscard]] auto Author() const { return m_author; }
	[[nodiscard]] auto borrowed() const { return m_bookIsBorrowed; }
	[[nodiscard]] auto customer() const { return m_customer; }
	[[nodiscard]] auto& ReservationList() const { return m_reservationList; }

	void returnBook();
	void borrowBook(std::shared_ptr<Customer>& customer);
	int reserveBook(const std::shared_ptr<Customer>& customer);
	void unreserveBook(const std::shared_ptr<Customer>& customer);

	void read(std::ifstream& inStream);
	void write(std::ofstream& outStream) const;

private:
	std::string m_title, m_author;
	bool m_bookIsBorrowed = false;
	std::shared_ptr<Customer> m_customer;
	std::list<std::shared_ptr<Customer>> m_reservationList;

	void cleanup() noexcept;


};

