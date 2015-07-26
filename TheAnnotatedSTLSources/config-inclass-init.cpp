#include <iostream>
using namespace std;

template <typename T>
class TestClass
{
public:
	static const int _datai = 5;
	static const long _datal = 3L;
	static const char _datac = 'c';
};

class INT
{
	friend ostream& operator<<(ostream& os, const INT& i);

private:
	int m_i;
public:
	INT(int i) :m_i(i)
	{
	}
	INT& operator++()
	{
		++(this->m_i);
		return *this;
	}
	const INT operator++(int)
	{
		INT temp = *this;
		++(*this);
		return temp;
	}

	INT& operator--()
	{
		--(this->m_i); 
		return *this;
	}
	const INT operator--(int)
	{
		INT temp = *this;
		--(*this);
		return temp;
	}

	int& operator&() const{
		return (int&)m_i;
	}
};
ostream& operator<< (ostream& os, const INT& i)
{
	os << i.m_i;
	return os;
}

int main()
{
	cout << TestClass<int>::_datai << endl;
	cout << TestClass<int>::_datal << endl;
	cout << TestClass<int>::_datac << endl;
}