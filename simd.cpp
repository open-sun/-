#include<iostream>
#include <fstream>
#include<vector>
#include<algorithm>
//#include <mmintrin.h>     // MMX
//#include <xmmintrin.h>    // SSE
//#include <emmintrin.h>    // SSE2
//#include <pmmintrin.h>    // SSE3
//#include <tmmintrin.h>    // SSSE3
//#include <smmintrin.h>    // SSE4.1
//#include <nmmintrin.h>    // SSE4.2
//#include <wmmintrin.h>    // AES
//#include <immintrin.h>    // AVX
//#include <intrin.h>
#include<string>
#include <sstream>
#include<windows.h>
using namespace std;
class index
{
public:
	int len = 0;
	vector<unsigned int>key;
};
bool operator<(const index& i1, const index& i2)
{
	return i1.len < i2.len;
}
void searchlist(int *query,vector<index>&id,int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final = idx[0];
	index final1;
	for (int i = 1; i < n; i++)
	{
		for (int j = 0; j < final.len; j++)
		{
			bool flag = false;
			for (int p = 0; p < idx[i].len; p++)
			{
				if (final.key[j] == idx[i].key[p])
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
			{
				final.key.erase(final.key.begin() + j);
				final.len--;
				j--;
			}
		}
	}
	cout << final.len<<" ";
}
void searchelement(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final;
	for (int i = 0; i < idx[0].len; i++)
	{
		int count = 1;
		bool flag = false;
		for (int j = 1; j < n; j++)
		{
			for (int p = 0; p < idx[j].len; p++)
			{
				if (idx[j].key[p] == idx[0].key[i])
				{
					flag = true;
					count++;
					break;
				}
			}
		}
		if (flag && count == n)
		{
			final.key.push_back(idx[0].key[i]);
			final.len++;
		}
	}
	cout << final.len<<" ";
}

/*void searchelementsse(int* query, vector<index> &id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final;
	for (int i = 0; i < idx[0].len; i++)
	{
		int count = 1;
		bool flag = false;
		__m128i var0;
		var0 = _mm_set_epi32(idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i]);
		for (int j = 1; j < n; j++)
		{
			int k = 0;
			for (; (k < idx[j].len)&&((idx[j].len-k)%4!=0); k++)
			{
				if (idx[j].key[k] == idx[0].key[i])
				{
					flag = true;
					count++;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
			for (; k < idx[j].len; k += 4)
			{
				__m128i var1;
				var1 = _mm_set_epi32(idx[j].key[k], idx[j].key[k + 1], idx[j].key[k + 2], idx[j].key[k + 3]);
				__m128i result = _mm_cmpeq_epi32(var0, var1);
				int com[4] = { 0 };
				_mm_storeu_si128((__m128i*)com, result);
				if (!_mm_testz_si128(result, result)) {
					count++;
					flag = true;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
		}
		if (flag && count == n)
		{
			final.key.push_back(idx[0].key[i]);
			final.len++;
		}
	}
	cout << final.len<<" ";
}
void searchlistsse(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final = idx[0];
	for (int i = 1; i < n; i++)
	{
		
		for (int j = 0; j < final.len; j++)
		{
			__m128i var0;
			var0 = _mm_set_epi32(final.key[j], final.key[j], final.key[j], final.key[j]);
			bool flag = false;
			int p = 0;
			for (; (p < idx[i].len)&&((idx[i].len-p)%4!=0); p++)
			{
				if (final.key[j] == idx[i].key[p])
				{
					flag = true;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
			for (; p < idx[i].len; p += 4)
			{
				__m128i var1;
				var1 = _mm_set_epi32(idx[i].key[p], idx[i].key[p + 1], idx[i].key[p + 2], idx[i].key[p + 3]);
				__m128i result = _mm_cmpeq_epi32(var0, var1);
				int com[4] = { 0 };
				_mm_storeu_si128((__m128i*)com, result);
				if (!_mm_testz_si128(result, result)) {
					flag = true;
					break;
				}

			}
			if (flag == false)
			{
				final.key.erase(final.key.begin() + j);
				final.len--;
				j--;
			}
		}
	}
	cout << final.len << " ";
}
void searchlistavx(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final = idx[0];
	for (int i = 1; i < n; i++)
	{

		for (int j = 0; j < final.len; j++)
		{
			__m256i var0;
			var0 = _mm256_set_epi32(final.key[j], final.key[j], final.key[j], final.key[j], final.key[j], final.key[j], final.key[j],final.key[j]);
			bool flag = false;
			int p = 0;
			for (; (p < idx[i].len) && ((idx[i].len - p) % 8 != 0); p++)
			{
				if (final.key[j] == idx[i].key[p])
				{
					flag = true;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
			for (; p < idx[i].len; p += 8)
			{
				__m256i var1;
				var1 = _mm256_set_epi32(idx[i].key[p], idx[i].key[p + 1], idx[i].key[p + 2], idx[i].key[p + 3],idx[i].key[p+4], idx[i].key[p + 5], idx[i].key[p + 6], idx[i].key[p + 7]);
				__m256i result = _mm256_cmpeq_epi32(var0, var1);
				int com[8] = { 0 };
				_mm256_storeu_si256((__m256i*)com, result);
				if (!_mm256_testz_si256(result, result)) {
					flag = true;
					break;
				}

			}
			if (flag == false)
			{
				final.key.erase(final.key.begin() + j);
				final.len--;
				j--;
			}
		}
	}
	cout << final.len << " ";
}
void searchelementavx(int* query, vector<index>& id, int n)
{
	vector<index>idx;
	for (int i = 0; i < n; i++)
	{
		idx.push_back(id[query[i]]);
	}
	sort(idx.begin(), idx.end());
	index final;
	for (int i = 0; i < idx[0].len; i++)
	{
		int count = 1;
		bool flag = false;
		__m256i var0;
		var0 = _mm256_set_epi32(idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i], idx[0].key[i]);
		for (int j = 1; j < n; j++)
		{
			int k = 0;
			for (; (k < idx[j].len) && ((idx[j].len - k) % 8 != 0); k++)
			{
				if (idx[j].key[k] == idx[0].key[i])
				{
					flag = true;
					count++;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
			for (; k < idx[j].len; k += 8)
			{
				__m256i var1;
				var1 = _mm256_set_epi32(idx[j].key[k], idx[j].key[k + 1], idx[j].key[k + 2], idx[j].key[k + 3], idx[j].key[k + 4], idx[j].key[k + 5], idx[j].key[k + 6], idx[j].key[k + 7]);
				__m256i result = _mm256_cmpeq_epi32(var0, var1);
				int com[8] = { 0 };
				_mm256_storeu_si256((__m256i*)com, result);
				if (!_mm256_testz_si256(result, result)) {
					count++;
					flag = true;
					break;
				}
			}
			if (flag == true)
			{
				continue;
			}
		}
		if (flag && count == n)
		{
			final.key.push_back(idx[0].key[i]);
			final.len++;
		}
	}
	cout << final.len<<" ";
}*/
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
	cout << ((tail - head) * 1000.0 / freq) /1000.0 << "s" << '\n';
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
	ifstream outfile("ExpQuery", ios::in );
	string temp;
	int query[1000][5] = { 0 };
	int lin = 0;
	while (getline(outfile, temp))

	{
		stringstream ss(temp);
		int addr = 0;
		int tmp;
		while (ss>>tmp)
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
	searchelementavx(ceshi, ceshi2, 2);*/
	//cout << "SSELIST" << endl;
	//gettime(searchlistsse, query, idx);
	cout << "LIST" << endl;
	gettime(searchlist, query, idx);
	/*cout << "AVXLIST" << endl;
	gettime(searchlistavx, query, idx);*/
	cout << "elemenr" << endl;
	gettime(searchelement, query, idx);
	//cout << "SSEelement" << endl;
	//gettime(searchelementsse, query, idx);
	//cout << "avxelement" << endl;
	//gettime(searchelementavx, query, idx);
	return 0;
}