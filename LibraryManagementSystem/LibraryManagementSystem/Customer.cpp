#include "Customer.h"
#include <iostream>
#include <utility>
#include <fstream>

using namespace  std;

Customer::Customer()
{
	cout << "Customer()" << endl;
}

Customer::~Customer()
{
	cleanup();
	
	cout << "~Customer()" << endl;
	
}

Customer::Customer(std::string name, std::string address) : m_name(std::move(name)), m_address(std::move(address))
{
	cout << "Customer(std::string name, std::string address)" << endl;
}

std::ostream& operator<<(std::ostream& outStream, const Customer* customer)
{
	if (customer == nullptr)
	{
		return outStream;
	}

	outStream << " \"" << customer->Name() << "\" address: " << customer->Address();

	auto booksBorrowed = customer->BooksBorrowed();
	if (!booksBorrowed.empty())
	{
		outStream << endl << "  Borrowed books: ";

		auto first = true;
		for (const auto book : booksBorrowed)
		{
			outStream << (first ? "" : ",") << book->Author();
			first = false;
		}
	}

	auto reservationList = customer->ReservationList();
	if (!reservationList.empty())
	{
		outStream << endl << "  Reserved books: ";

		auto first = true;
		for (const auto book : reservationList)
		{
			outStream << (first ? "" : ",") << book->Title();
			first = false;
		}
	}

	return outStream;
}

void Customer::read(std::ifstream& inStream)
{

	string name, address;
	getline(inStream, name);
	getline(inStream, address);
	m_name = name;
	m_address = address;

}

void Customer::write(std::ofstream& outStream) const
{
	outStream << m_name << endl;
	outStream << m_address << endl;
}

void Customer::cleanup() noexcept
{
	cout << "cleanup()" << endl;
	m_name = "";
	m_address = "";
	m_loanSet.clear();
	m_reservationSet.clear();
	
}
