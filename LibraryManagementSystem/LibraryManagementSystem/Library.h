#pragma once
#include <set>
#include <map>
#include <string_view>
#include "Book.h"
#include "Customer.h"

class Library
{
public:
	Library();
	virtual ~Library();

	// disallow assignment and pass by value
	Library(const Library& src) = delete;
	Library& operator=(const Library& rhs) = delete;

	// explicit default move constructor and assignment
	Library(Library&& src) = default;
	Library& operator=(Library&& rhs) = default;

	
	

	/*static std::map<int, int> bookLoanMap;
	static std::map<int, std::set<int>> bookReservationSetMap;

	static std::map<int, std::set<int>> customerLoanSetMap;
	static std::map<int, std::set<int>> customerReservationSetMap;*/
	
	
private:
	auto lookupBook(std::string_view author, std::string_view title, Book* bookPtr = nullptr) const;
	auto lookupCustomer(std::string_view name, std::string_view address, Customer* customerPtr = nullptr) const;

	auto lookupBookIndex(const Book* bookPtr);
	auto lookupCustomerIndex(const Customer* customerPtr);

	auto lookupBookPtr(int bookIndex);
	auto lookupCustomerPtr(int customerIndex);

	void addBook();
	void deleteBook();
	void listBooks();
	void addCustomer();
	void deleteCustomer();
	void listCustomers();
	void borrowBook();
	void reserveBook();
	void returnBook();
	
	void load();
	void save();

	void getBookDetails(std::string& author, std::string& title) const;
	void getCustomerDetails(std::string& name, std::string& address) const;

	list<Book*> m_bookList;
	list<Customer*> m_customerList;
	
	static string s_binaryPath;
};

