#include <string>
#include <fstream>
#include "Library.h"
#include "Customer.h"

using namespace  std;

Customer::Customer(string_view name, string_view address): m_name(name), m_address(address)
{
	
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

std::ostream& operator<<(std::ostream& outStream, const Customer* customer)
{
	if(customer == nullptr)
	{
		return outStream;
	}
	
	outStream << customer->name() << ", " << customer->address() << ".";

	auto booksBorrowed = customer->BooksBorrowed();
	if (!booksBorrowed.empty())
	{
		outStream << endl << "  Borrowed books: ";

		auto first = true;
		for (const auto book : booksBorrowed)
		{
			outStream << (first ? "" : ",") <<  book->Author();
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
			outStream << (first ? "" : ",") <<  book->Title();
			first = false;
		}
	}

	return outStream;
}
