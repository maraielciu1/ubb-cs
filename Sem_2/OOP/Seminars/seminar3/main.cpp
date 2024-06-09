#include "Tests.h"
#include <iostream>

using namespace std;

template <typename T>
void print(T a)
{
	cout << a << endl;
}

int main()
{
	system("color f4");

	print<int>(2);
	print<double>(2.0);
	print<std::string>("Hello");

	Tests::testAll();
	return 0;
}