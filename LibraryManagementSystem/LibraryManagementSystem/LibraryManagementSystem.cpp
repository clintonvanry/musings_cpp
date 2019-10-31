// LibraryManagementSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Book.h"

using namespace  std;

int main()
{
	cout << "starting...!\n" << endl;
	
	Book b("Author1", "title 1");
	b.borrowBook(55);

	cout << b << endl;

	return 0;
}

