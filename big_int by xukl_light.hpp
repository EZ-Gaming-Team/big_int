/*
	Name: big_int
	Copyright: GPLv3.0
	Author: xukl
	Date: 15/08/17 22:03
	Description: Big integer class.
*/
#ifndef __BIGINT__
#define __BIGINT__
#include <cstdio>
#include <cstring>
class big_int
{
		static const long long MOD = 1000000000;
		static const int len = 9, MAX = 100;
		long long x[MAX];
		int l;
		bool sign; // true: >=0
		static int max(const int & a, const int & b)
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
		static bool unsigned_lesser(const big_int& a, const big_int& b)
		{
			if (a.l < b.l)
				return true;
			const long long *ax = a.x + a.l, *bx = b.x + b.l;
			const long long * const bend = b.x;
			while (bx != bend)
				if (*--ax != *--bx)
					return *ax < *bx;
			return false;
		}
	public:
		big_int() : x()
		{
			l = 0;
			sign = true;
		}
		big_int(long long a) : x()
		{
			sign = (a >= 0);
			if (!sign)
				a = -a;
			if (a > 0)
			{
				const int tmp = a / MOD;
				x[1] = tmp;
				x[0] = a - tmp * MOD;
				l = (x[1] ? 2 : 1);
			}
			else
				l = 0;
		}
		big_int(const char * a) : x()
		{
			if (*a == '-')
				++a;
			else
				sign = 1;
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
				for (int j = 0; j < 9; j++, i++)
					s = s * 10 + a[i] - '0';
				x[k++] = s;
			}
			if (i != -9)
			{
				long long s = 0;
				const int j = i + 9;
				for (i = 0; i < j; i++)
					s = s * 10 + a[i] - '0';
				x[k++] = s;
			}
			l = k;
		}

		bool operator < (const big_int& b) const
		{
			if (sign != b.sign)
				return b.sign;
			if (l != b.l)
				return (l < b.l) ^ !sign;
			for (int i = l - 1; i >= 0; i--)
				if(x[i] != b.x[i])
					return (x[i] < b.x[i]) ^ !sign;
			return false;
		}
		bool operator == (const big_int& b) const
		{
			return !(*this < b) && !(b <*this);
		}

		big_int operator - () const
		{
			big_int ans = *this;
			ans.sign = (!sign);
			return ans;
		}
		big_int operator + (const big_int& b) const
		{
			if (b.l == 0)
				return *this;
			if (sign ^ (b.sign))
				return *this - -b;
			big_int ans;
			const int mx = max(l, b.l);
			for (int i = 0; i < mx; i++)
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
		const big_int & operator += (const big_int& b)
		{
			return *this = *this + b;
		}
		big_int operator - (const big_int& b) const
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
			int & al = ans.l;
			for (al = l - 1; al != -1 && ans.x[al] == 0; --al);
			++al;
			return ans;
		}
		const big_int & operator -= (const big_int& b)
		{
			return *this = *this - b;
		}
		big_int operator * (const big_int& b) const
		{
			if (l == 0 || b.l == 0)
				return big_int();
			big_int ans;
			for (int i = 0; i < l; i++)
				for (int j = 0; j < b.l; ++j)
				{
					ans.x[i + j] += x[i] * b.x[j];
					const long long tmp = ans.x[i + j] / MOD;
					ans.x[i + j + 1] += tmp;
					ans.x[i + j] -= tmp * MOD;
				}
			ans.l = (ans.x[l + b.l - 1] ? l + b.l : l + b.l - 1);
			ans.sign = !(sign ^ (b.sign));
			return ans;
		}
		const big_int & operator *= (const big_int& b)
		{
			return *this = *this * b;
		}
		void print() const
		{
			if (l == 0)
				putchar('0');
			else
			{
				if (!sign)
					putchar('-');
				printf("%lld", x[l - 1]);
				for(int i = l - 2; i >= 0; --i)
					printf("%09lld",x[i]);
			}
		}
		void print_NL() const
		{
			print();
			putchar('\n');
		}
};
#endif
