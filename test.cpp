#include <iostream>
#include "big_int_light.hpp"
using std::cin;
using std::cout;
using std::endl;
#define test(val) cout << #val" = " << val << endl
int main()
{
	big_int a = 3, b = 5;
	test(a);
	test(b);
	test(a - b);
	test(b - a);
	test(a * b);
	test(b * a);
	test((a += -a * b - b));
	const char str_a[] = "111111111", str_b[] = "-101010101010101";
	test((a = str_a));
	test((b = str_b));
	test((a *= b));
	test((a *= "1000000000"));
	test((big_int(1000000000) * big_int(10000)));
	cout << "Input b: ";
	cin >> b;
	test(b);
	return 0;
}
