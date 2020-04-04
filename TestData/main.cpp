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
void generate_1(map<int, unordered_set<int>> &mp, int count_v, int count_e);	// ������ɣ���֤����Ŀ����result����
bool generate_2(map<int, unordered_set<int>> &mp, map<int, set<vector<int>>> &pathAll, int count_v, int count_e, int count_c);	// ������ɣ���֤����Ŀ����ͻ���Ŀ�����������result
inline void translatePath(vector<int> &path, vector<int> &new_path);	// ��pathתΪСid��ǰ��new_path
void outputTestData(map<int, unordered_set<int>> &mp, string filename);	// ���test_data
void outputResult(map<int, set<vector<int>>> &Result, string filename);	// �������result


int main()
{
	srand(int(time(NULL)));

	int count_v = 100000;	// ������
	int count_e = 280000;	// ����
	int count_c = 1000000;	// ����

	// ��ʼ��
	map<int, unordered_set<int>> mp;	// keyΪfrom_id��valueΪfrom_id���Ե����to_id����
	map<int, set<vector<int>>> pathAll;
	unordered_set<int> st_temp;
	for (int i = 0; i < count_v; i++)
		mp.insert(pair<int, unordered_set<int>>(i, st_temp));

	// process
	//generate_1(mp, count_v, count_e);						// ������ɣ���֤����Ŀ����result����
	generate_2(mp, pathAll, count_v, count_e, count_c);		// ������ɣ���֤����Ŀ����ͻ���Ŀ�����������result

	// output
	outputTestData(mp, output_test_data_file_name);			//���test_data
	outputResult(pathAll, output_result_file_name);			//���result

	cout << "generate file success!!" << endl;
	cout << "test_data file path: " << output_test_data_file_name << endl;
	cout << "result file path: " << output_result_file_name << endl;
	return 0;
}

void generate_1(map<int, unordered_set<int>> &mp, int count_v, int count_e)
{
	for (int i = 0; i < count_e; i++)	// �����ܱ���
	{
		int from = rand() % count_v;	// ������
		int to = rand() % count_v;		// ����յ�

		while (from == to || mp[from].find(to) != mp[from].end())	// ��֤from!=to����from��to���滥��ת�ˣ������������
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

	//����СԪ�ؿ�ʼ����
	new_path.clear();
	new_path.insert(new_path.begin(), smallest, path.end());

	//ͷ�����ܻ���Ԫ��
	new_path.insert(new_path.end(), path.begin(), smallest);
}

bool generate_2(map<int, unordered_set<int>> &mp, map<int, set<vector<int>>> &pathAll, int count_v, int count_e, int count_c)
{
	// ��֤��ͻ�����Ŀ
	int edge_count = 0;
	vector<int> path;
	vector<int> new_path;
	set<int> idAll;
	for (int i = 0; i < count_c; i++)
	{
		// ���ĳ����������3-7֮��
		int len = rand() % 5 + 3;	

		path.clear();
		idAll.clear();
		int start_id = rand() % count_v;	// ������
		path.push_back(start_id);
		idAll.insert(start_id);
		int from, to;
		for (int j = 0; j < len; j++)		
		{					
			if (j == len - 1)
			{
				from = path.back();
				to = start_id;				// ͷ==β
			}
			else
			{
				from = path.back();
				to = rand() % count_v;		// ����յ�

				path.push_back(to);
			}
			
			++edge_count;
			mp[from].insert(to);
		}

		translatePath(path, new_path);
		pathAll[len].insert(new_path);
	}

	// ����Ŀ��������
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

	// ��֤�ߵ���Ŀ, �������������
	for (int i = edge_count; i < count_e; i++)	// �����ܱ���
	{
		int from = rand() % count_v;	// ������
		int to = rand() % count_v;		// ����յ�

		while (from == to || mp[from].find(to) != mp[from].end())	// ��֤from!=to����from��to���滥��ת�ˣ������������
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
	// ���ļ� �ж��Ƿ�򿪳ɹ�
	outFile.open(filename, ios::out);
	if (!outFile.is_open()) {
		cout << "open output file failed" << endl;
		return;
	}

	//������·����Ŀ
	int sum = 0;
	for (auto it = Result.begin(); it != Result.end(); it++)
	{
		sum += int(it->second.size());
	}

	//�����·����Ŀ
	outFile << sum << endl;

	//�������·��
	for (auto it = Result.begin(); it != Result.end(); it++)	//ÿһ����ͬ���ȵĻ�·����
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); iter++)		//ÿһ����·
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