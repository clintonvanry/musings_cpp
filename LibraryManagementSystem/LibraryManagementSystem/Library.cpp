#include "Library.h"
#include <iostream>
#include "Book.h"
#include "Customer.h"
#include <fstream>

using namespace std;

Library::~Library()
{
	for (auto book : m_bookList)
	{
		delete book;
		book = nullptr;
	}
	m_bookList.clear();

	for (auto customer : m_customerList)
	{
		delete customer;
		customer = nullptr;
	}
	m_customerList.clear();
}

void Library::run()
{
	load();
	auto quit = false;
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
			listCustomer();
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

void Library::getBookDetails(std::string& author, std::string& title) const
{
	cout << "Author: ";
	getline(cin, author);

	cout << "Title: ";
	getline(cin, title);
}
void Library::getCustomerDetails(std::string& name, std::string& address) const
{
	cout << "Name: ";
	getline(cin, name);

	cout << "Address: ";
	getline(cin, address);
}




Book* Library::lookupBook(std::string_view author, std::string_view title)
{
	for (auto book : m_bookList)
	{
		if (book->Author() == author && book->Title() == title)
		{
			return book;
		}
	}

	return nullptr;
}

Customer* Library::lookupCustomer(std::string_view name, std::string_view address)
{
	for (auto customer : m_customerList)
	{
		if (customer->Name() == name && customer->Address() == address)
		{
			return customer;
		}
	}

	return nullptr;
}


void Library::addBook()
{
	string author, title;
	getBookDetails(author, title);

	const auto bookFromLibrary = lookupBook(author, title);
	if (bookFromLibrary != nullptr)
	{
		cout << endl << bookFromLibrary << endl;
		return;
	}

	auto book = new Book(title, author); // should this not be deleted near the end. adding to the list is likely a copy?
	cout << endl << "Added: " << book << endl;
	m_bookList.push_back(book);

}

void Library::addCustomer()
{
	string name, address;
	getCustomerDetails(name, address);

	const auto customerFromLibrary = lookupCustomer(name, address);
	if (customerFromLibrary != nullptr)
	{
		cout << endl << "A customer with name " << name << " and address " << address << " already exists." << endl;
		return;
	}

	auto customer = new Customer(name, address);
	cout << endl << "Added " << customer << endl;
	m_customerList.push_back(customer);
}




void Library::listBooks()
{
	if (m_bookList.empty())
	{
		cout << "No books available in the Library. Please add a book to the library" << endl;
		return;
	}

	for (const auto book : m_bookList)
	{
		cout << book << endl;
	}
}
void Library::listCustomer()
{
	if (m_customerList.empty())
	{
		cout << "No customers in the library" << endl;
		return;
	}

	for (const auto customer : m_customerList)
	{
		cout << customer << endl;
	}
}

void Library::deleteBook()
{

	string author, title;
	getBookDetails(author, title);


	auto bookFromLibrary = lookupBook(author, title);
	if (bookFromLibrary == nullptr)
	{
		cout << endl << "The book \"" << title << "\" by " << author << " does not exist." << endl;
		return;
	}

	auto customerFromBook = bookFromLibrary->customer();
	if (customerFromBook != nullptr)
	{
		customerFromBook->returnBook(bookFromLibrary);
	}

	// many people can reserve a book
	for (auto customer : bookFromLibrary->ReservationList())
	{
		customer->unreserveBook(bookFromLibrary);
	}



	cout << endl << "Deleted book" << bookFromLibrary << endl;
	m_bookList.remove(bookFromLibrary);
	delete bookFromLibrary;
}

void Library::deleteCustomer()
{
	string name, address;
	getCustomerDetails(name, address);

	auto customerFromLibrary = lookupCustomer(name, address);
	if (customerFromLibrary == nullptr)
	{
		cout << endl << "There is no customer with name " << name << " and address " << address << "." << endl;
		return;
	}

	if (customerFromLibrary->hasBorrowed())
	{
		cout << "Customer " << name << " has borrowed at least " << "one book and cannot be deleted." << endl;
		return;
	}

	for (auto& book : m_bookList)
	{
		book->unreserveBook(customerFromLibrary);
	}


	m_customerList.remove(customerFromLibrary);
	cout << endl << "Deleted customer" << customerFromLibrary << endl;
	delete customerFromLibrary;

}


void Library::borrowBook()
{
	cout << "Please enter book details" << endl;
	string author, title;
	getBookDetails(author, title);


	auto bookFromLibrary = lookupBook(author, title);
	if (bookFromLibrary == nullptr)
	{
		cout << endl << "There is no book \"" << title << "\" by " << " author " << author << "." << endl;
		return;
	}

	if (bookFromLibrary->borrowed())
	{
		cout << endl << "The book \"" << title << "\" by " << author << " has already been borrowed." << endl;
		return;
	}

	cout << "Please enter customer details" << endl;
	string name, address;
	getCustomerDetails(name, address);

	auto customerFromLibrary = lookupCustomer(name, address);
	if (customerFromLibrary == nullptr)
	{
		cout << endl << "There is no customer with name " << name << " and address " << address << "." << endl;
		return;
	}

	bookFromLibrary->borrowBook(customerFromLibrary);
	customerFromLibrary->borrowBook(bookFromLibrary);

}

void Library::returnBook()
{
	cout << "Please enter book details" << endl;
	string author, title;
	getBookDetails(author, title);


	auto bookFromLibrary = lookupBook(author, title);
	if (bookFromLibrary == nullptr)
	{
		cout << endl << "There is no book \"" << title << "\" by " << " author " << author << "." << endl;
		return;
	}

	if (!bookFromLibrary->borrowed())
	{
		cout << endl << "The book \"" << title << "\" by " << author << " has not been borrowed." << endl;
		return;
	}

	auto customer = bookFromLibrary->customer();
	bookFromLibrary->returnBook();
	customer->returnBook(bookFromLibrary);
	cout << endl << "Returned book " << bookFromLibrary << endl;


	auto reservationList = bookFromLibrary->ReservationList();

	if (!reservationList.empty())
	{
		auto newCustomer = reservationList.front();
		reservationList.erase(reservationList.begin());

		bookFromLibrary->borrowBook(newCustomer);
		newCustomer->borrowBook(bookFromLibrary);

		cout << endl << "Book:" << bookFromLibrary << endl << "Borrowed by: " << newCustomer->Name() << endl;

	}

}

void Library::reserveBook()
{
	cout << "Please enter book details" << endl;
	string author, title;
	getBookDetails(author, title);


	auto bookFromLibrary = lookupBook(author, title);
	if (bookFromLibrary == nullptr)
	{
		cout << endl << "There is no book \"" << title << "\" by " << " author " << author << "." << endl;
		return;
	}

	if (!bookFromLibrary->borrowed())
	{
		cout << endl << "The book with author " << author << " and title \"" << title << "\" has not been " << "borrowed. Please borrow the book instead." << endl;
		return;
	}

	cout << "Please enter customer details" << endl;
	string name, address;
	getCustomerDetails(name, address);

	auto customerFromLibrary = lookupCustomer(name, address);
	if (customerFromLibrary == nullptr)
	{
		cout << endl << "There is no customer with name " << name << " and address " << address << "." << endl;
		return;
	}

	if (bookFromLibrary->customer() == customerFromLibrary)
	{
		cout << endl << "The book has already been borrowed by " << name << "." << endl;
		return;
	}

	customerFromLibrary->reserveBook(bookFromLibrary);
	auto position = bookFromLibrary->reserveBook(customerFromLibrary);

	cout << endl << "Book reservation position " << position << "nd reserved." << endl;
}

void Library::load()
{
	
	ifstream inStream(s_binaryPath);

	if (!inStream)
	{
		return;
	}

	auto bookListSize = 0;
	inStream.read(reinterpret_cast<char*>(&bookListSize), sizeof bookListSize);
	for (auto i = 0; i < bookListSize; i++)
	{
		auto book = new Book();
		book->read(inStream);
		m_bookList.push_back(book);
	}

	auto customerListSize = 0;
	inStream.read(reinterpret_cast<char*>(&customerListSize), sizeof customerListSize);
	for (auto i = 0; i < customerListSize; i++)
	{
		auto customer = new Customer();
		customer->read(inStream);
		m_customerList.push_back(customer);
	}

	// reset the borrowed state for books
	for (const auto book : m_bookList)
	{
		auto bookIsBorrowed = false;
		inStream.read(reinterpret_cast<char*>(&bookIsBorrowed), sizeof bookIsBorrowed);
		if (bookIsBorrowed)
		{
			auto borrowedBookIndex = 0;
			inStream.read(reinterpret_cast<char*>(&borrowedBookIndex), sizeof borrowedBookIndex);
			book->borrowBook(lookupCustomerPtr(borrowedBookIndex));

			// reservationlist
			auto reservationList = book->ReservationList();
			auto reservationListSizeFromStorage = 0;
			inStream.read(reinterpret_cast<char*>(&reservationListSizeFromStorage), sizeof reservationListSizeFromStorage);

			if (reservationListSizeFromStorage > 0)
			{
				for (auto i = 0; i < reservationListSizeFromStorage; ++i)
				{
					auto reserverationCustIndex = 0;
					inStream.read(reinterpret_cast<char*>(&reserverationCustIndex), sizeof reserverationCustIndex);
					auto reservationCustomer = lookupCustomerPtr(reserverationCustIndex);
					reservationList.push_back(reservationCustomer);
				}
			}
		}
	}

	
	for (const auto customer : m_customerList)
	{
		int borrowedListSize = customer->numberOfBooksBorrowed();
		if (borrowedListSize > 0) 
		{
			auto borrowedList = customer->BooksBorrowed(); // TODO memory leak here not sure how to resolve
			inStream.read(reinterpret_cast<char*>(&borrowedList), sizeof borrowedListSize);
			if (borrowedListSize > 0)
			{
				for (auto i = 0; i < borrowedListSize; ++i)
				{
					auto bookIndex = 0;
					inStream.read(reinterpret_cast<char*>(&bookIndex), sizeof bookIndex);

					auto book = lookupBookPtr(bookIndex);
					borrowedList.insert(book);
				}
			}
		}
		
		
		// reservations
		auto reservations = customer->ReservationList();
		int reservationSize = reservations.size();
		inStream.read(reinterpret_cast<char*>(&reservationSize), sizeof reservationSize);
		if (reservationSize > 0)
		{
			for (auto i = 0; i < reservationSize; ++i)
			{
				auto reservationBookIndex = 0;
				inStream.read(reinterpret_cast<char*>(&reservationBookIndex), sizeof reservationBookIndex);
				auto book =lookupBookPtr(reservationBookIndex);
				reservations.insert(book);
			}
		}
	}
	
}

void Library::save()
{
	
	ofstream outStream(s_binaryPath);
	if (!outStream)
	{
		// should throw exception
		return;
	}

	
	// write the size of the book list
	auto bookListSize = m_bookList.size();
	outStream.write(reinterpret_cast<char*>(&bookListSize), sizeof bookListSize);
	// each book write its contents to disk
	for (const auto book : m_bookList)
	{
		book->write(outStream);
	}

	// write the size of the customer list
	auto customerListSize = m_customerList.size();
	outStream.write(reinterpret_cast<char*>(&customerListSize), sizeof bookListSize);
	// each customer write its contents to disk
	for (const auto customer : m_customerList)
	{
		customer->write(outStream);
	}

	// write the book specifics
	for (const auto book : m_bookList)
	{
		// store the index of the customer who borrowed the book if it is borrowed.
		auto bookIsBorrowed = book->borrowed();
		outStream.write(reinterpret_cast<char*>(&bookIsBorrowed), sizeof bookIsBorrowed);
		if (bookIsBorrowed) // write the index of the customer
		{
			auto customerIndex = lookupCustomerIndex(book->customer());
			outStream.write(reinterpret_cast<char*>(&customerIndex), sizeof customerIndex);
		}

		// store the size of the reservation for the book
		const auto& reservationList = book->ReservationList();
		auto reservationListSize = reservationList.size();
		outStream.write(reinterpret_cast<char*>(&reservationListSize), sizeof reservationListSize);
		for (const auto customer : reservationList)
		{
			// write the index
			auto customerReservationIndex = lookupCustomerIndex(customer);
			outStream.write(reinterpret_cast<char*>(&customerReservationIndex), sizeof customerReservationIndex);
		}
	}

	// write the customer specifics
	for (const auto customer : m_customerList)
	{
		
		auto customerBooksBorrowedSize = customer->numberOfBooksBorrowed();
		outStream.write(reinterpret_cast<char*>(&customerBooksBorrowedSize), sizeof customerBooksBorrowedSize);

		if (customerBooksBorrowedSize > 0) 
		{
			const auto& customerBooksBorrowed = customer->BooksBorrowed();
			for (const auto book : customerBooksBorrowed)
			{
				auto bookIndex = lookupBookIndex(book);
				outStream.write(reinterpret_cast<char*>(&bookIndex), sizeof bookIndex);
			}
		}
		
		const auto& bookReservationList = customer->ReservationList();
		auto bookReservationListSize = bookReservationList.size();
		outStream.write(reinterpret_cast<char*>(&bookReservationListSize), sizeof bookReservationListSize);

		if (bookReservationListSize > 0) 
		{
			for (const auto bookToBeReserved : bookReservationList)
			{
				auto bookReservationIndex = lookupBookIndex(bookToBeReserved);
				outStream.write(reinterpret_cast<char*>(&bookReservationIndex), sizeof bookReservationIndex);
			}
		}

	}
	
}

Customer* Library::lookupCustomerPtr(int customerIndex)
{
	auto customerInterator = m_customerList.begin();

	for (auto i = 0; i < customerIndex; ++i)
	{
		++customerInterator;
	}
	return *customerInterator;
}

Book* Library::lookupBookPtr(int bookIndex)
{
	auto bookInterator = m_bookList.begin(); // read up on this one

	for (auto i = 0; i < bookIndex; ++i)
	{
		++bookInterator;
	}
	return *bookInterator;
}

int Library::lookupCustomerIndex(const Customer* customerPtr)
{
	auto index = 0;
	for (const auto customer : m_customerList)
	{
		if (customer == customerPtr)
		{
			return index;
		}
		index++;
	}
	return -1;
}

int Library::lookupBookIndex(const Book* bookPtr)
{
	auto index = 0;
	for (const auto book : m_bookList)
	{
		if (book == bookPtr)
		{
			return index;
		}
		index++;
	}
	return -1;
}
