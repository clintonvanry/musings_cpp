#pragma once
#include <set>
#include <map>
#include "Book.h"
#include "Customer.h"

class Library
{
public:
	Library();
	virtual ~Library() = default;

	// disallow assignment and pass by value
	Library(const Library& src) = delete;
	Library& operator=(const Library& rhs) = delete;

	// explicit default move constructor and assignment
	Library(Library&& src) = default;
	Library& operator=(Library&& rhs) = default;

	static std::map<int, Book> s_bookMap;
	static std::map<int, Customer> s_customerMap;

	static std::map<int, int> bookLoanMap;
	static std::map<int, std::set<int>> bookReservationSetMap;

	static std::map<int, std::set<int>> customerLoanSetMap;
	static std::map<int, std::set<int>> customerReservationSetMap;
	
	
private:
	bool lookupBook(const std::string& author, const std::string& title, Book* bookPtr = nullptr) const;
	bool lookupCustomer(const std::string& name, const std::string& address, Customer* customerPtr = nullptr) const;

	void addBook();
	void deleteBook();
	static void listBooks();
	void addCustomer();
	void deleteCustomer();
	void listCustomers();
	void borrowBook();
	void reserveBook();
	void returnBook();
	
	void load();
	void save();

	void getBookDetails(std::string& author, std::string& title);
	
	static string s_binaryPath;
};

