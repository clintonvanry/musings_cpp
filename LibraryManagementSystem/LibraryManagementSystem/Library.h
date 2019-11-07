#pragma once
#include <string>
#include <list>

class Book;
class Customer;

class Library
{
public:
	// Constructors
	Library() = default;

	Library(const Library& src) = delete; // remove copy constructor
	Library& operator=(Library& rhs) = delete; // remove assignment

	Library(Library&& src) = default; // move constructor
	Library& operator=(Library&& rhs) = default; // remove assignment

	virtual ~Library();

	void run();

private:
	std::list<Book*> m_bookList;
	std::list<Customer*> m_customerList;

	void getBookDetails(std::string& author, std::string& title) const;
	Book* lookupBook(std::string_view author, std::string_view title);

	void addBook();
	void deleteBook();
	void listBooks();

	void getCustomerDetails(std::string& name, std::string& address) const;
	Customer* lookupCustomer(std::string_view name, std::string_view address);

	void addCustomer();
	void deleteCustomer();
	void listCustomer();

	void borrowBook();
	void returnBook();

	void reserveBook();

	void load();
	void save();

	auto lookupBookIndex(const Book* bookPtr);
	auto lookupCustomerIndex(const Customer* customerPtr);

	auto lookupBookPtr(int bookIndex);
	auto lookupCustomerPtr(int customerIndex);

};

