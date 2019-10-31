#include "Customer.h"
#include <utility>
#include <string>
#include <fstream>

using namespace  std;

int Customer::MaxCustomerId;

Customer::Customer(string name, string address): m_name(std::move(name)), m_address(std::move(address)), m_customerId(++MaxCustomerId)
{
	
}

void Customer::read(std::ifstream& inStream)
{
	inStream.read((char*)&m_customerId, sizeof m_customerId);
	getline(inStream, m_name);
	getline(inStream, m_address);

	{ int borrowSetSize;
	inStream.read((char*)&borrowSetSize, sizeof borrowSetSize);

	for (int count = 0; count < borrowSetSize; ++count) {
		int bookId;
		inStream.read((char*)&bookId, sizeof bookId);
		m_loanSet.insert(bookId);
	}
	}

	int reserveListSize;
	inStream.read((char*)&reserveListSize,sizeof reserveListSize);

	for (int count = 0; count < reserveListSize; ++count) 
	{
		int bookId;
		inStream.read((char*)&bookId, sizeof bookId);
		m_loanSet.insert(bookId);
	}
	
}

void Customer::write(std::ofstream& outStream) const
{
	outStream.write((char*)&m_customerId, sizeof m_customerId);
	outStream << m_name << endl;
	outStream << m_address << endl;

	{ int borrowSetSize = m_loanSet.size();
	outStream.write((char*)&borrowSetSize, sizeof borrowSetSize);

	for (int bookId : m_loanSet) {
		outStream.write((char*)&bookId, sizeof bookId);
	}
	}

	int reserveListSize = m_reservationSet.size();
	outStream.write((char*)&reserveListSize,sizeof reserveListSize);

	for (int bookId : m_reservationSet) 
	{
		outStream.write((char*)&bookId, sizeof bookId);
	}
	
}

std::ostream& operator<<(std::ostream& outStream, const Customer& customer)
{
	outStream << customer.m_customerId << ". " << customer.m_name
		<< ", " << customer.m_address << ".";

	if (!customer.m_loanSet.empty()) {
		outStream << endl << "  Borrowed books: ";

		bool first = true;
		for (int bookId : customer.m_loanSet) {
			outStream << (first ? "" : ",") << endl;
				//<< Library::s_bookMap[bookId].author();
			first = false;
		}
	}

	if (!customer.m_reservationSet.empty()) {
		outStream << endl << "  Reserved books: ";

		bool first = true;
		for (int bookId : customer.m_reservationSet) {
			outStream << (first ? "" : ",") << endl;
				//<< Library::s_bookMap[bookId].title();
			first = false;
		}
	}

	return outStream;
}
