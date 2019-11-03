#include "Library.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

map<int, Book> Library::s_bookMap;
map<int, Customer> Library::s_customerMap;

string Library::s_binaryPath(".\\Debug\\Library.binary");

Library::Library()
{
	load();

	bool quit = false;
	while (!quit) 
	{
		cout << "1. Add Book" << endl
			<< "2. Delete Book" << endl
			<< "3. List Books" << endl
			<< "4. Add Customer" << endl
			<< "5. Delete Customer" << endl
			<< "6. List Customers" << endl
			<< "7. Borrow Book" << endl
			<< "8. Reserve Book" << endl
			<< "9. Return Book" << endl
			<< "0. Quit" << endl
			<< ": ";

		string text;
		cin >> text; // should i use getline so i dont have the space issue?
		cin.clear();
		cin.ignore(1, '\n');
		cout << endl;

		auto choice = -1;
		sscanf_s(text.c_str(), "%i", &choice);

		switch (choice) {
		case 1:
			addBook();
			break;

		case 2:
			deleteBook();
			break;

		case 3:
			listBooks();
			break;

		case 4:
			addCustomer();
			break;

		case 5:
			deleteCustomer();
			break;

		case 6:
			listCustomers();
			break;

		case 7:
			borrowBook();
			break;

		case 8:
			reserveBook();
			break;

		case 9:
			returnBook();
			break;

		case 0:
			quit = true;
			break;
		default:
			quit = true;
			break;
		};
		

		cout << endl;
	}

	save();
}

bool Library::lookupBook(const std::string& author, const std::string& title, Book* bookPtr) const
{
	for(auto& entry : s_bookMap) 
	{
		auto& book = entry.second;
		if(book.Author() == author 
		&& book.Title() == title)
		{
			if(bookPtr != nullptr)
			{
				*bookPtr = std::move(book); // investigate further std::move thin this due to assignment and copy constructor
			}
			return true;
		}
	}

	return false;
}

bool Library::lookupCustomer(const std::string& name, const std::string& address, Customer* customerPtr) const
{
	for (auto& entry : s_customerMap) 
	{
		auto& customer = entry.second;

		if ((customer.name() == name) 
		&&(customer.address() == address)) 
		{
			if (customerPtr != nullptr) 
			{
				*customerPtr = std::move(customer); // investigate
			}

			return true;
		}
	}

	return false;
}

void Library::addBook()
{
	string author, title;
	getBookDetails(author, title);

	auto bookFromLibrary = make_unique<Book>();
	if (lookupBook(author, title, bookFromLibrary.get()))
	{
		cout << endl << *bookFromLibrary << endl;
		bookFromLibrary.reset();
		return;
	}
	bookFromLibrary.reset();

	Book book(author, title);
	cout << endl << "Added: " << book << endl;
	s_bookMap[book.bookId()] = std::move(book);
	
}

void Library::getBookDetails(std::string& author, std::string& title)
{
	cout << "Author: ";
	getline(cin, author);
	
	cout << "Title: ";
	getline(cin, title);	
}

void Library::getCustomerDetails(std::string& name, std::string& address)
{
	cout << "Name: ";
	getline(cin, name);

	cout << "Address: ";
	getline(cin, address);
}

void Library::deleteBook()
{
	string author, title;
	getBookDetails(author, title);


	auto bookFromLibrary = make_unique<Book>();
	if (!lookupBook(author, title, bookFromLibrary.get()))
	{
		cout << endl << "The book \"" << title << "\" by "	<< author << " does not exist." << endl;
		bookFromLibrary.reset();
		return;
	}

	for (auto& entry : s_customerMap) 
	{
		auto& customer = entry.second;
		customer.returnBook(bookFromLibrary->bookId());
		customer.unreserveBook(bookFromLibrary->bookId());
		s_customerMap[customer.id()] = std::move(customer);
	}

	s_bookMap.erase(bookFromLibrary->bookId());
	bookFromLibrary.reset();
	cout << endl << "Deleted." << endl;
}

void Library::listBooks()
{
	if (s_bookMap.empty()) 
	{
		cout << "No books." << endl;
		return;
	}

	for (const auto& entry : s_bookMap) 
	{
		const auto& book = entry.second;
		cout << book << endl;
	}
}

void Library::addCustomer()
{
	string name, address;
	getCustomerDetails(name, address);

	if (lookupCustomer(name, address)) 
	{
		cout << endl << "A customer with name " << name	<< " and address " << address << " already exists."	<< endl;
		return;
	}

	Customer customer(name, address);
	s_customerMap[customer.id()] = std::move(customer);
	cout << endl << "Added." << endl;
}

void Library::deleteCustomer()
{
	string name, address;
	getCustomerDetails(name, address);

	Customer customer;
	if (!lookupCustomer(name, address, &customer)) 
	{
		cout << endl << "There is no customer with name " << name 	<< " and address " << address << "." << endl;
		return;
	}

	if (customer.hasBorrowed()) 
	{
		cout << "Customer " << name << " has borrowed at least " << "one book and cannot be deleted." << endl;
		return;
	}

	for (auto& entry : s_bookMap) 
	{
		auto& book = entry.second;
		book.unreserveBook(customer.id());
		s_bookMap[book.bookId()] = std::move(book);
	}

	cout << endl << "Deleted." << endl;
	s_customerMap.erase(customer.id());
}

void Library::listCustomers()
{
	if (s_customerMap.empty()) 
	{
		cout << "No customers." << endl;
		return;
	}

	for (const auto& entry : s_customerMap) 
	{
		const auto& customer = entry.second;
		cout << customer << endl;
	}
}

void Library::borrowBook()
{
	string author, title;
	getBookDetails(author, title);


	Book book;
	if (!lookupBook(author, title, &book)) 
	{
		cout << endl << "There is no book \"" << title << "\" by " << " author " << author << "." << endl;
		return;
	}

	if (book.borrowed()) 
	{
		cout << endl << "The book \"" << title << "\" by " << author << " has already been borrowed." << endl;
		return;
	}

	string name, address;
	getCustomerDetails(name, address);

	Customer customer;
	if (!lookupCustomer(name, address, &customer)) 
	{
		cout << endl << "There is no customer with name " << name << " and address " << address << "." << endl;
		return;
	}

	book.borrowBook(customer.id());
	customer.borrowBook(book.bookId());

	s_bookMap[book.bookId()] = std::move(book);
	s_customerMap[customer.id()] = std::move(customer);
	
	cout << endl << "Borrowed." << endl;
	
}

void Library::reserveBook()
{
	string author, title;
	getBookDetails(author, title);


	Book book;
	if (!lookupBook(author, title, &book)) 
	{
		cout << endl << "There is no book \"" << title << "\" by "	<< "author " << author << "." << endl;
		return;
	}

	if (!book.borrowed()) 
	{
		cout << endl << "The book with author " << author	<< " and title \"" << title << "\" has not been "	<< "borrowed. Please borrow the book instead." << endl;
		return;
	}

	string name, address;
	getCustomerDetails(name, address);

	Customer customer;
	if (!lookupCustomer(name, address, &customer)) 
	{
		cout << endl << "No customer with name " << name
			<< " and address " << address << " exists." << endl;
		return;
	}

	if (book.customerId() == customer.id()) {
		cout << endl << "The book has already been borrowed by "
			<< name << "." << endl;
		return;
	}

	customer.reserveBook(book.bookId());
	auto position = book.reserveBook(customer.id());
	
	s_bookMap[book.bookId()] = std::move(book);
	s_customerMap[customer.id()] = std::move(customer);
	cout << endl << position << "nd reserve." << endl;
	
}

void Library::returnBook()
{
	string author, title;
	getBookDetails(author, title);


	Book book;
	if (!lookupBook(author, title, &book)) 
	{
		cout << endl << "No book \"" << title	<< "\" by " << author << " exists." << endl;
		return;
	}

	if (!book.borrowed()) 
	{
		cout << endl << "The book \"" << title	<< "\" by " << author	<< "\" has not been borrowed." << endl;
		return;
	}

	book.returnBook();
	cout << endl << "Returned." << endl;

	auto customer = std::move(s_customerMap[book.customerId()]);
	customer.returnBook(book.bookId());
	s_customerMap[customer.id()] = std::move(customer);

	auto& reservationList = book.reservationList();

	if (!reservationList.empty()) 
	{
		auto newCustomerId = reservationList.front();
		reservationList.erase(reservationList.begin());
		book.borrowBook(newCustomerId);

		auto newCustomer = std::move( s_customerMap[newCustomerId]);
		newCustomer.borrowBook(book.bookId());

		cout << endl << "Borrowed by " << newCustomer.name() << endl;
		s_customerMap[newCustomerId] = std::move(newCustomer);
		
	}

	s_bookMap[book.bookId()] = std::move(book);
}

void Library::load()
{
	ifstream inStream(s_binaryPath);

	if (inStream) 
	{
		int numberOfBooks;
		inStream.read(reinterpret_cast<char*>(&numberOfBooks), sizeof numberOfBooks);

		for (auto count = 0; count < numberOfBooks; ++count) 
		{
			Book book;
			book.read(inStream);
			auto bookId = book.bookId();
			s_bookMap[bookId] = std::move(book);
			Book::MaxBookId = max(Book::MaxBookId, bookId);
		}

		int numberOfCustomers;
		inStream.read(reinterpret_cast<char*>(&numberOfCustomers),sizeof numberOfCustomers);

		for (auto count = 0; count < numberOfCustomers; ++count) 
		{
			Customer customer;
			customer.read(inStream);
			auto customerId = customer.id();
			s_customerMap[customer.id()] = std::move(customer);
			Customer::MaxCustomerId = max(Customer::MaxCustomerId, customerId);
		}
	}
}

void Library::save()
{
	ofstream outStream(s_binaryPath);
	if (outStream) 
	{
		auto numberOfBooks = s_bookMap.size();
		outStream.write(reinterpret_cast<char*>(&numberOfBooks), sizeof numberOfBooks);

		for (const auto& entry : s_bookMap) 
		{
			const auto& book = entry.second;
			book.write(outStream);
		}

		auto numberOfCustomers = s_customerMap.size();
		outStream.write(reinterpret_cast<char*>(&numberOfCustomers),sizeof numberOfCustomers);

		for (const auto& entry : s_customerMap) 
		{
			const auto& customer = entry.second;
			customer.write(outStream);
		}
	}
}


