#pragma once
#include <string>
#include <list>
#include <memory>

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
	inline static const std::string s_binaryPath = ".\\Debug\\Library.binary"; // dont know why ths does not work {R"(C:\Users\Marius\Documents\)"s};
	std::list<std::shared_ptr<Book>> m_bookList;
	std::list<std::shared_ptr<Customer>> m_customerList;
	
	void getBookDetails(std::string& author, std::string& title) const;
	std::shared_ptr<Book> lookupBook(std::string_view author, std::string_view title);

	void addBook();
	void deleteBook();
	void listBooks();

	void getCustomerDetails(std::string& name, std::string& address) const;
	std::shared_ptr<Customer> lookupCustomer(std::string_view name, std::string_view address);

	void addCustomer();
	void deleteCustomer();
	void listCustomer();

	void borrowBook();
	void returnBook();

	void reserveBook();

	void load();
	void save();

	int lookupBookIndex(const Book* bookPtr);
	int lookupCustomerIndex(const Customer* customerPtr);

	std::shared_ptr<Book>& lookupBookPtr(int bookIndex);
	std::shared_ptr<Customer>& lookupCustomerPtr(int customerIndex);

};

