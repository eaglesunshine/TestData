#include <iostream>
#include <map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <set>
#include <algorithm>

using namespace std;

// config
string output_test_data_file_name = "data/test_data_test.txt";
string output_result_file_name = "data/result_test.txt";

// func
void generate_1(map<int, unordered_set<int>> &mp, int count_v, int count_e);	// 随机生成，保证边数目，无result生成
bool generate_2(map<int, unordered_set<int>> &mp, map<int, set<vector<int>>> &pathAll, int count_v, int count_e, int count_c);	// 随机生成，保证边数目和最低环数目，并输出部分result
inline void translatePath(vector<int> &path, vector<int> &new_path);	// 将path转为小id在前的new_path
void outputTestData(map<int, unordered_set<int>> &mp, string filename);	// 输出test_data
void outputResult(map<int, set<vector<int>>> &Result, string filename);	// 输出部分result


int main()
{
	srand(int(time(NULL)));

	int count_v = 100000;	// 顶点数
	int count_e = 280000;	// 边数
	int count_c = 1000000;	// 环数

	// 初始化
	map<int, unordered_set<int>> mp;	// key为from_id，value为from_id可以到达的to_id序列
	map<int, set<vector<int>>> pathAll;
	unordered_set<int> st_temp;
	for (int i = 0; i < count_v; i++)
		mp.insert(pair<int, unordered_set<int>>(i, st_temp));

	// process
	//generate_1(mp, count_v, count_e);						// 随机生成，保证边数目，无result生成
	generate_2(mp, pathAll, count_v, count_e, count_c);		// 随机生成，保证边数目和最低环数目，并输出部分result

	// output
	outputTestData(mp, output_test_data_file_name);			//输出test_data
	outputResult(pathAll, output_result_file_name);			//输出result

	cout << "generate file success!!" << endl;
	cout << "test_data file path: " << output_test_data_file_name << endl;
	cout << "result file path: " << output_result_file_name << endl;
	return 0;
}

void generate_1(map<int, unordered_set<int>> &mp, int count_v, int count_e)
{
	for (int i = 0; i < count_e; i++)	// 控制总边数
	{
		int from = rand() % count_v;	// 随机起点
		int to = rand() % count_v;		// 随机终点

		while (from == to || mp[from].find(to) != mp[from].end())	// 保证from!=to，且from、to不玩互相转账，否则重新随机
		{
			from = rand() % count_v;
			to = rand() % count_v;
		}
		mp[from].insert(to);
	}
}

inline void translatePath(vector<int> &path, vector<int> &new_path)
{
	auto smallest = std::min_element(path.begin(), path.end());

	//从最小元素开始链接
	new_path.clear();
	new_path.insert(new_path.begin(), smallest, path.end());

	//头部可能还有元素
	new_path.insert(new_path.end(), path.begin(), smallest);
}

bool generate_2(map<int, unordered_set<int>> &mp, map<int, set<vector<int>>> &pathAll, int count_v, int count_e, int count_c)
{
	// 保证最低环的数目
	int edge_count = 0;
	vector<int> path;
	vector<int> new_path;
	set<int> idAll;
	for (int i = 0; i < count_c; i++)
	{
		// 环的长度在随机在3-7之间
		int len = rand() % 5 + 3;	

		path.clear();
		idAll.clear();
		int start_id = rand() % count_v;	// 随机起点
		path.push_back(start_id);
		idAll.insert(start_id);
		int from, to;
		for (int j = 0; j < len; j++)		
		{					
			if (j == len - 1)
			{
				from = path.back();
				to = start_id;				// 头==尾
			}
			else
			{
				from = path.back();
				to = rand() % count_v;		// 随机终点

				path.push_back(to);
			}
			
			++edge_count;
			mp[from].insert(to);
		}

		translatePath(path, new_path);
		pathAll[len].insert(new_path);
	}

	// 边数目超出警告
	if (edge_count > count_e)
	{
		cout << "generate fixed circles: " << count_c << ", cost edges: " << edge_count << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!! edge_count > count_e, please check !!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
		return false;
	}
	else
	{
		cout << "generate fixed circles: " << count_c << ", cost edges: " << edge_count << endl << endl;
	}

	// 保证边的数目, 不够的随机生成
	for (int i = edge_count; i < count_e; i++)	// 控制总边数
	{
		int from = rand() % count_v;	// 随机起点
		int to = rand() % count_v;		// 随机终点

		while (from == to || mp[from].find(to) != mp[from].end())	// 保证from!=to，且from、to不玩互相转账，否则重新随机
		{
			from = rand() % count_v;
			to = rand() % count_v;
		}
		mp[from].insert(to);
	}

	return true;
}

void outputTestData(map<int, unordered_set<int>> &mp, string filename)
{
	ofstream outfile(filename);
	for (auto iter = mp.begin(); iter != mp.end(); ++iter)
	{
		for (auto iter1 = iter->second.begin(); iter1 != iter->second.end(); ++iter1)
		{
			outfile << iter->first << "," << *iter1 << ",100" << endl;
		}
	}
	outfile.close();
}

void outputResult(map<int, set<vector<int>>> &Result, string filename)
{
	ofstream outFile;
	// 打开文件 判断是否打开成功
	outFile.open(filename, ios::out);
	if (!outFile.is_open()) {
		cout << "open output file failed" << endl;
		return;
	}

	//计算总路径数目
	int sum = 0;
	for (auto it = Result.begin(); it != Result.end(); it++)
	{
		sum += int(it->second.size());
	}

	//输出总路径数目
	outFile << sum << endl;

	//输出具体路径
	for (auto it = Result.begin(); it != Result.end(); it++)	//每一组相同长度的环路序列
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); iter++)		//每一个环路
		{
			for (int i = 0; i<int((*iter).size()); i++)
			{
				if (i == 0)
				{
					outFile << (*iter)[i];
				}
				else
				{
					outFile << "," << (*iter)[i];
				}
			}
			outFile << endl;
		}
	}

	outFile.close();
}