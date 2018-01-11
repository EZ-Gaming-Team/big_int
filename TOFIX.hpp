/*
	Name: big_int
	Copyright: GPLv3.0
	Author: xukl
	Date: 11/01/18 13:38
	Description: Big integer class.
*/
#ifndef __BIGINT__
#define __BIGINT__
#include <cstdio>
#include <cassert>
#include <cstring>
class big_int
{
		static const long long MOD = 1000000000;
		static const int len = 9, MAX = 100;
		long long x[MAX];
		int l;
		bool sign; // true: >=0
		static int max(const int a, const int b)
		{
			return a > b ? a : b;
		}
		static long long div(const long long & x)
		{
			return x >= 0 ? x / MOD : (x - MOD + 1) / MOD;
		}
		static void mod(long long & x)
		{
			x %= MOD;
			if (x < 0)
				x += MOD;
		}
		static bool unsigned_lesser(const big_int &a, const big_int &b)
		{
			if (a.l != b.l)
				return a.l < b.l;
			const long long *ax = a.x + a.l, *bx = b.x + b.l;
			const long long * const bend = b.x;
			while (bx != bend)
				if (*--ax != *--bx)
					return *ax < *bx;
			return false;
		}
		friend std::ostream &operator<< (std::ostream &, const big_int &);
	public:
		big_int() : x()
		{
			l = 0;
			sign = true;
		}
		big_int(long long a) : x()
		{
			if (a < 0)
			{
				sign = false;
				a = -a;
			}
			else
				sign = true;
			if (a == 0)
				l = 0;
			else
			{
				const int tmp = a / MOD;
				x[1] = tmp;
				x[0] = a - tmp * MOD;
				l = (x[1] ? 2 : 1);
			}
		}
		big_int(const char *a) : x()
		{
			if (*a == '-')
			{
				++a;
				sign = false;
			}
			else
				sign = true;
			int ll = strlen(a);
			if ((ll == 1) && (a[0] == '0'))
			{
				l = 0;
				return;
			}
			int i, k = 0;
			for (i = ll - 9; i >= 0; i -= 18)
			{
				long long s = 0;
				for (int j = 0; j < 9; ++j, ++i)
					s = s * 10 + a[i] - '0';
				x[k++] = s;
			}
			if (i != -9)
			{
				long long s = 0;
				const int j = i + 9;
				for (i = 0; i < j; ++i)
					s = s * 10 + a[i] - '0';
				x[k++] = s;
			}
			l = k;
		}

		bool operator< (const big_int &b) const
		{
			if (sign != b.sign)
				return b.sign;
			return unsigned_lesser(*this, b) ^ !sign;
		}
		bool operator== (const big_int &b) const
		{
			return !(*this < b) && !(b < *this);
		}

		big_int operator- () const
		{
			big_int ans = *this;
			ans.sign = (!sign);
			return ans;
		}
		big_int operator+ (const big_int &b) const
		{
			if (b.l == 0)
				return *this;
			if (sign ^ (b.sign))
				return *this - -b;
			big_int ans;
			const int mx = max(l, b.l);
			for (int i = 0; i < mx; ++i)
			{
				ans.x[i] += x[i] + b.x[i];
				if (ans.x[i] >= MOD)
				{
					ans.x[i + 1] = 1;
					ans.x[i] -= MOD;
				}
			}
			ans.l = (ans.x[mx] ? mx + 1 : mx);
			ans.sign = sign;
			return ans;
		}
		const big_int &operator+= (const big_int &b)
		{
			return *this = *this + b;
		}
		big_int operator- (const big_int &b) const
		{
			if (b.l == 0)
				return *this;
			if (sign ^ (b.sign))
				return *this + -b;
			if (unsigned_lesser(*this, b))
				return -(b - *this);
			big_int ans;
			for (int i = 0; i < l; ++i)
			{
				ans.x[i] += x[i] - b.x[i];
				if (ans.x[i] < 0)
				{
					ans.x[i + 1] = -1;
					ans.x[i] += MOD;
				}
			}
			int &al = ans.l;
			for (al = l - 1; al != -1 && ans.x[al] == 0; --al);
			++al;
			ans.sign = sign;
			return ans;
		}
		const big_int &operator-= (const big_int &b)
		{
			return *this = *this - b;
		}
		big_int operator* (const big_int &b) const
		{
			if (l == 0 || b.l == 0)
				return big_int();
			big_int ans;
			for (int i = 0; i < l; ++i)
				for (int j = 0; j < b.l; ++j)
				{
					ans.x[i + j] += x[i] * b.x[j];
					ans.x[i + j + 1] += ans.x[i + j] / MOD;
					ans.x[i + j] %= MOD;
				}
			ans.l = (ans.x[l + b.l - 1] ? l + b.l : l + b.l - 1);
			ans.sign = !(sign ^ (b.sign));
			return ans;
		}
		const big_int &operator*= (const big_int &b)
		{
			return *this = *this * b;
		}
//		TODO TOFIX: buggy div / mod
//		big_int operator / (const big_int& b) const
//		{
//			big_int rem = *this, quo;
//			bool first = true;
//			assert(b.l);
//			long long m[len], s;
//			int ll, bl = b.l;
//			big_int bn[len];
//			m[0] = 1;
//			for (int i = 1; i < len; i++)
//				m[i] = m[i - 1] * 10;
//			for (int i = 0; i < len; i++)
//			{
//				bn[i] = b * m[i];
//				bn[i].x[bl - 1] += bn[i].x[bl] * MOD;
//				bn[i].x[bl] = 0;
//				bn[i].l = bl;
//			}
//			for (int i = l - 1; i >= bl - 1; i--)
//			{
//				s = 0;
//				for (int j = len - 1; j >= 0; j--)
//				{
//					while (!unsigned_lesser(rem, bn[j]))
//					{
//						ll = i - bl + 1;
//						for (int k = 0; k < bl; ++k, ++ll)
//						{
//							rem.x[ll] -= bn[j].x[k];
//							rem.x[ll + 1] += div(rem.x[ll]);
//							mod(rem.x[ll]);
//						}
//						rem.x[ll - 1] += rem.x[ll] * MOD;
//						rem.x[ll] = 0;
//						s += m[j];
//					}
//				}
//				ll = i - bl + 1;
//				if (first && s)
//				{
//					first = false;
//					quo.l = ll + 1;
//				}
//				quo.x[ll] = s;
//				if (!i)
//					break;
//				rem.x[i - 1] += rem.x[i] * MOD;
//				rem.x[i] = 0;
//				rem.l--;
//			}
//			return quo;
//		}
//		big_int operator /= (const big_int& b)
//		{
//			return *this = *this / b;
//		}
//		big_int operator % (const big_int& b) const
//		{
//			return *this - *this / b * b;
//		}
//		big_int operator %= (const big_int& b)
//		{
//			return *this = *this % b;
//		}
		void print() const
		{
			if (l == 0)
				putchar('0');
			else
			{
				if (!sign)
					putchar('-');
				printf("%lld", x[l - 1]);
				for (int i = l - 2; i >= 0; --i)
					printf("%09lld", x[i]);
			}
		}
		void print_NL() const
		{
			print();
			putchar('\n');
		}
};
std::ostream &operator<< (std::ostream &out, const big_int &b)
{
	if (b.l == 0)
		out << '0';
	else
	{
		if (!b.sign)
			out << '-';
		out << b.x[b.l - 1];
		out << std::setfill('0');
		for (int i = b.l - 2; i >= 0; --i)
			out << std::setw(9) << b.x[i];
	}
	return out;
}
#endif
