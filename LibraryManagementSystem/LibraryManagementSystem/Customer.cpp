#include <utility>
#include <string>
#include <fstream>
#include "Library.h"
#include "Customer.h"

using namespace  std;

int Customer::MaxCustomerId;

Customer::Customer(string name, string address):  m_customerId(++MaxCustomerId), m_name(std::move(name)), m_address(std::move(address))
{
	
}

void Customer::read(std::ifstream& inStream)
{
	inStream.read(reinterpret_cast<char*>(&m_customerId), sizeof m_customerId);
	getline(inStream, m_name);
	getline(inStream, m_address);

	int borrowSetSize;
	inStream.read(reinterpret_cast<char*>(&borrowSetSize), sizeof borrowSetSize);

	for (auto count = 0; count < borrowSetSize; ++count) 
	{
		int bookId;
		inStream.read(reinterpret_cast<char*>(&bookId), sizeof bookId);
		m_loanSet.insert(bookId);
	}
	

	int reserveListSize;
	inStream.read(reinterpret_cast<char*>(&reserveListSize),sizeof reserveListSize);

	for (auto count = 0; count < reserveListSize; ++count) 
	{
		int bookId;
		inStream.read(reinterpret_cast<char*>(&bookId), sizeof bookId);
		m_loanSet.insert(bookId);
	}
	
}

void Customer::write(std::ofstream& outStream) const
{
	outStream.write(reinterpret_cast<char*>(const_cast<int*>(&m_customerId)), sizeof m_customerId);
	outStream << m_name << endl;
	outStream << m_address << endl;

	auto borrowSetSize = m_loanSet.size();
	outStream.write(reinterpret_cast<char*>(const_cast<unsigned*>(&borrowSetSize)), sizeof borrowSetSize);

	for (auto bookId : m_loanSet) 
	{
		outStream.write(reinterpret_cast<char*>(const_cast<int*>(&bookId)), sizeof bookId);
	}
	

	auto reserveListSize = m_reservationSet.size();
	outStream.write(reinterpret_cast<char*>(const_cast<unsigned*>(&reserveListSize)),sizeof reserveListSize);

	for (auto bookId : m_reservationSet) 
	{
		outStream.write(reinterpret_cast<char*>(const_cast<int*>(&bookId)), sizeof bookId);
	}
	
}

std::ostream& operator<<(std::ostream& outStream, const Customer& customer)
{
	outStream << customer.m_customerId << ". " << customer.m_name << ", " << customer.m_address << ".";

	if (!customer.m_loanSet.empty()) 
	{
		outStream << endl << "  Borrowed books: ";

		auto first = true;
		for (int bookId : customer.m_loanSet) 
		{
			outStream << (first ? "" : ",") <<  Library::s_bookMap[bookId].Author();
			first = false;
		}
	}

	if (!customer.m_reservationSet.empty()) 
	{
		outStream << endl << "  Reserved books: ";

		auto first = true;
		for (auto bookId : customer.m_reservationSet) 
		{
			outStream << (first ? "" : ",") <<  Library::s_bookMap[bookId].Title();
			first = false;
		}
	}

	return outStream;
}
