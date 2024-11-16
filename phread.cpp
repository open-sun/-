#include<iostream>
#include <fstream>
#include<vector>
#include<algorithm>
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
void searchlist(int* query, vector<index>& id, int n)
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
	cout << final.len << " ";
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
	cout << final.len << " ";
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
	cout << "LIST" << endl;
	gettime(searchlist, query, idx);
	cout << "elemenr" << endl;
	gettime(searchelement, query, idx);
	return 0;
}