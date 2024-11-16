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
#define MAXSIZE 2000
using namespace std;
class index
{
public:
	int len = 0;
	vector<unsigned int> key;
};

bool operator<(const index& s1, const index& s2)
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
void searchlist(int* query, vector<index>& id, int n)
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
struct ThreadData
{
	vector<unsigned int>* n_bit_bits;
	vector<unsigned int>* bitmap_bits;
	size_t start;
	size_t end;
};

void* thread_function(void* arg)
{
	ThreadData* data = static_cast<ThreadData*>(arg);
	for (size_t i = data->start; i < data->end; ++i)
	{
		(*data->n_bit_bits)[i] &= (*data->bitmap_bits)[i];
	}
	pthread_exit(NULL);
}

void searchlistp(int* query, vector<index>& id, int n, int num_threads)
{
	vector<index> idx;
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

	pthread_t threads[num_threads];
	ThreadData thread_data[num_threads];
	size_t chunk_size = n_bit.m_bits.size() / num_threads;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < num_threads; i+=4)
		{
			size_t start = i * chunk_size;
			size_t end = (i == num_threads - 1) ? n_bit.m_bits.size() : (i + 1) * chunk_size;
			thread_data[i] = { &n_bit.m_bits, &bitmap[j].m_bits, start, end };
			pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
		}

		for (int i = 0; i < num_threads; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	}

	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void* thread_functionsse(void* arg)
{
	ThreadData* data = static_cast<ThreadData*>(arg);
	for (size_t i = data->start; i < data->end; i+=4)
	{
		__m128i tmp, tmp0, tmp1;
		tmp0 = _mm_set_epi32((*data->n_bit_bits)[i], (*data->n_bit_bits)[i+1], (*data->n_bit_bits)[i+2], (*data->n_bit_bits)[i+3]);
		tmp1 = _mm_set_epi32((*data->bitmap_bits)[i], (*data->bitmap_bits)[i+1], (*data->bitmap_bits)[i+2], (*data->bitmap_bits)[i+3]);
		tmp = _mm_and_si128(tmp0, tmp1);
		_mm_storeu_si128((__m128i*) & (*data->n_bit_bits)[i], tmp);
	}
	pthread_exit(NULL);
}

void searchlistpsse(int* query, vector<index>& id, int n, int num_threads)
{
	vector<index> idx;
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

	pthread_t threads[num_threads];
	ThreadData thread_data[num_threads];
	size_t chunk_size = n_bit.m_bits.size() / num_threads;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < num_threads; ++i)
		{
			size_t start = i * chunk_size;
			size_t end = (i == num_threads - 1) ? n_bit.m_bits.size() : (i + 1) * chunk_size;
			thread_data[i] = { &n_bit.m_bits, &bitmap[j].m_bits, start, end };
			pthread_create(&threads[i], NULL, thread_functionsse, &thread_data[i]);
		}

		for (int i = 0; i < num_threads; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	}

	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void* thread_functionavx(void* arg)
{
	ThreadData* data = static_cast<ThreadData*>(arg);
	for (size_t i = data->start; i < data->end; i += 4)
	{
		__m256i tmp, tmp0, tmp1;
		tmp0 = _mm256_set_epi32((*data->n_bit_bits)[i], (*data->n_bit_bits)[i + 1], (*data->n_bit_bits)[i + 2], (*data->n_bit_bits)[i + 3], (*data->n_bit_bits)[i + 4], (*data->n_bit_bits)[i + 5], (*data->n_bit_bits)[i + 6], (*data->n_bit_bits)[i + 7]);
		tmp1 = _mm256_set_epi32((*data->bitmap_bits)[i], (*data->bitmap_bits)[i + 1], (*data->bitmap_bits)[i + 2], (*data->bitmap_bits)[i + 3], (*data->bitmap_bits)[i + 4], (*data->bitmap_bits)[i + 5], (*data->bitmap_bits)[i + 6], (*data->bitmap_bits)[i + 7]);
		tmp = _mm256_and_si256(tmp0, tmp1);
		_mm256_storeu_si256((__m256i*) & (*data->n_bit_bits)[i], tmp);
	}
	pthread_exit(NULL);
}

void searchlistpavx(int* query, vector<index>& id, int n, int num_threads)
{
	vector<index> idx;
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

	pthread_t threads[num_threads];
	ThreadData thread_data[num_threads];
	size_t chunk_size = n_bit.m_bits.size() / num_threads;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < num_threads; ++i)
		{
			size_t start = i * chunk_size;
			size_t end = (i == num_threads - 1) ? n_bit.m_bits.size() : (i + 1) * chunk_size;
			thread_data[i] = { &n_bit.m_bits, &bitmap[j].m_bits, start, end };
			pthread_create(&threads[i], NULL, thread_functionavx, &thread_data[i]);
		}

		for (int i = 0; i < num_threads; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	}

	for (size_t i = 0; i < n_bit.m_bits.size(); ++i)
	{
		count += count_ones(n_bit.m_bits[i]);
	}
	cout << count << " ";
}
void gettime(void (*func)(int* query, vector<index>& idx, int num, int n), int t_query[1000][5], vector<index>& idx)
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
		func(query, idx, num, 2);
		delete query;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&tail);
	cout << ((tail - head) * 1000.0 / freq) / 1000.0 << "s" << '\n';
}
void gettime2(void (*func)(int* query, vector<index>& idx, int num), int t_query[1000][5], vector<index>& idx)
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
	/*int ceshi[2] = { 0,1};
	vector<index>ceshi2;
	index tep;
	tep.len = 8;
	tep.key = { 2,4,6,7,8,9,11,12 };
	ceshi2.push_back(tep);
	tep.len = 8;
	tep.key = { 1,3,4,5,6,7,8,10 };
	ceshi2.push_back(tep);
	searchlistsse(ceshi, ceshi2, 2);
	searchlist(ceshi, ceshi2, 2);*/
	/*cout << "avxLIST" << endl;
	gettime(searchlistpavx, query, idx);
	cout << "SSELIST" << endl;
	gettime(searchlistpsse, query, idx);
	cout << "LISTp" << endl;
	gettime(searchlistp, query, idx);
	cout << "LIST" << endl;
	gettime2(searchlist, query, idx);*/
	return 0;
}
