// LibraryManagementSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Book.h"
#include "Customer.h"

using namespace  std;

int main()
{
	cout << "starting...!\n" << endl;
	
	Book b("Author1", "title 1");
	b.borrowBook(55);

	cout << b << endl;

	Customer c{ "Billy", "Bob" };
	c.borrowBook(66);
	c.reserveBook(77);
	cout << c << endl;
	
	return 0;
}

