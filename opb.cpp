#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <ctime>
#include <cmath>
#include <math.h>
#include <string>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <mmintrin.h>     // MMX
#include <xmmintrin.h>    // SSE
#include <emmintrin.h>    // SSE2
#include <pmmintrin.h>    // SSE3
#include <tmmintrin.h>    // SSSE3
#include <smmintrin.h>    // SSE4.1
#include <nmmintrin.h>    // SSE4.2
#include <wmmintrin.h>    // AES
#include <immintrin.h>    // AVX
#include<omp.h>
#define MAXSIZE 2000
int ok = 2;
using namespace std;
class index
{
public:
	int len = 0;
	vector<unsigned int> key;
};

bool operator < (const index& s1, const index& s2)
{
	return s1.len < s2.len;
}
class BitMap
{
public:
	BitMap(int range)
	{
		this->m_bits.resize((range + 31) / 32, 0);
	}

	void set_value(int data)
	{
		int index = data / 32;
		int offset = data % 32;
		this->m_bits[index] |= (1 << offset);
	}


	vector<unsigned int> m_bits;
};
int count_ones(unsigned int value)
{
	int count = 0;
	while (value)
	{
		count += value & 1;
		value >>= 1;
	}
	return count;
}
void searchlists(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	vector<BitMap> bitmap;
	for (int i = 0; i < n; i++)
	{
		bitmap.push_back(30000000);
		for (int j = 0; j < idx[i].len; j++)
		{
			bitmap[i].set_value(idx[i].key[j]);
		}
	}
	int count = 0;
	BitMap n_bit = bitmap[0];
	for (int i = 0; i < n; i++)
	{
      #pragma omp parallel for num_threads(ok) shared(n_bit) schedule(static)
		for (size_t j = 0; j < n_bit.m_bits.size(); ++j)
		{
			n_bit.m_bits[j] &= bitmap[i].m_bits[j];
		}
	}
	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void searchlistd(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	vector<BitMap> bitmap;
	for (int i = 0; i < n; i++)
	{
		bitmap.push_back(30000000);
		for (int j = 0; j < idx[i].len; j++)
		{
			bitmap[i].set_value(idx[i].key[j]);
		}
	}
	int count = 0;
	BitMap n_bit = bitmap[0];
	for (int i = 0; i < n; i++)
	{
#pragma omp parallel for num_threads(ok) shared(n_bit) schedule(dynamic)
		for (size_t j = 0; j < n_bit.m_bits.size(); ++j)
		{
			n_bit.m_bits[j] &= bitmap[i].m_bits[j];
		}
	}
	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void searchlistg(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	vector<BitMap> bitmap;
	for (int i = 0; i < n; i++)
	{
		bitmap.push_back(30000000);
		for (int j = 0; j < idx[i].len; j++)
		{
			bitmap[i].set_value(idx[i].key[j]);
		}
	}
	int count = 0;
	BitMap n_bit = bitmap[0];
	for (int i = 0; i < n; i++)
	{
#pragma omp parallel for num_threads(ok) shared(n_bit) schedule(guided)
		for (size_t j = 0; j < n_bit.m_bits.size(); ++j)
		{
			n_bit.m_bits[j] &= bitmap[i].m_bits[j];
		}
	}
	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void searchlistssse(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	vector<BitMap> bitmap;
	for (int i = 0; i < n; i++)
	{
		bitmap.push_back(30000000);
		for (int j = 0; j < idx[i].len; j++)
		{
			bitmap[i].set_value(idx[i].key[j]);
		}
	}
	int count = 0;
	BitMap n_bit = bitmap[0];
	for (int i = 0; i < n; i++)
	{
#pragma omp parallel for num_threads(ok) shared(n_bit) schedule(static)
		for (size_t j = 0; j < n_bit.m_bits.size(); j += 4)
		{
			__m128i tmp, tmp0, tmp1;
			tmp0 = _mm_set_epi32(n_bit.m_bits[j], n_bit.m_bits[j + 1], n_bit.m_bits[j + 2], n_bit.m_bits[j + 3]);
			tmp1 = _mm_set_epi32(bitmap[i].m_bits[j], bitmap[i].m_bits[j + 1], bitmap[i].m_bits[j + 2], bitmap[i].m_bits[j + 3]);
			tmp = _mm_and_si128(tmp0, tmp1);
			_mm_storeu_si128((__m128i*) & n_bit.m_bits[j], tmp);
		}
	}
	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void gettime(void (*func)(int* query, vector<index>& idx, int num), int t_query[1000][5], vector<index>& idx)
{
	long long head, tail, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&head);
	for (int i = 0; i < 1000; i++)
	{
		int num = 0;
		for (int j = 0; j < 5; j++)
		{
			if (t_query[i][j] != 0)
			{
				num++;
			}
		}
		int* query = new int[num];
		for (int j = 0; j < num; j++)
		{
			query[j] = t_query[i][j];
		}
		func(query, idx, num);
		delete query;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&tail);
	cout << ((tail - head) * 1000.0 / freq) / 1000.0 << "s" << '\n';
}
int main()
{
	vector<index>idx;
	int num = 0;
	ifstream infile("ExpIndex", ios::in | ios::binary); //二进制读方式打开
	if (!infile) {
		cout << "error" << endl;
		return 0;
	}
	for (int i = 0; i < 1756; i++)
	{
		index id;
		infile.read((char*)&id.len, sizeof(id.len));
		for (int j = 0; j < (id.len); j++)
		{
			unsigned int n_tmp;
			infile.read((char*)&n_tmp, sizeof(n_tmp));
			id.key.push_back(n_tmp);
		}
		idx.push_back(id);
	}
	cout << endl;
	infile.close();
	ifstream outfile("ExpQuery", ios::in);
	string temp;
	int query[1000][5] = { 0 };
	int lin = 0;
	while (getline(outfile, temp))

	{
		stringstream ss(temp);
		int addr = 0;
		int tmp;
		while (ss >> tmp)
		{
			query[lin][addr] = tmp;
			addr++;
		}
		lin++;

	}
	outfile.close();
	
	/*cout << "SSELIST" << endl;
	gettime(searchlistsse, query, idx);*/
	cout << "LISTs" << endl;
	gettime(searchlists, query, idx);
	cout << "LISTd" << endl;
	gettime(searchlistd, query, idx);
	cout << "LISTg" << endl;
	gettime(searchlistg, query, idx);
	cout << "LISTssse" << endl;
	gettime(searchlistssse, query, idx);
	//cout << "AVXLIST" << endl;
	//gettime(searchlistavx, query, idx);
	return 0;
}
