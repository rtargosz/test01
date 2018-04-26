#pragma once

#include <crtdbg.h>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <numeric>
#include <cmath>
#include <limits>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>

#define ASSERT _ASSERTE
#ifdef _DEBUG
#define VERIFY ASSERT
#define TRACE printf
#else
#define VERIFY(expression) (expression)
#define TRACE(expression) expression
#endif

using namespace std;

class CLearning
{
public:
	struct Graph
	{
		int val;
		Graph* left;
		Graph* right;

		Graph(int x) { val = x; }
	};

	struct player
	{
		int id;
		float latency;

		player(int id_, float latency_) : id{ id_ }, latency{ latency_ } {};
	};

	struct ListNode {
		int val;
		ListNode *next;
		ListNode(int x) : val(x), next(NULL) {}
	};

	CLearning() {};
	string longestPalindrome(string s);
	bool check_int(int x);
	void print_graph(Graph& g);
	int match_count(vector<vector<int>> g);
	int reverse_int(int x);
	int trap(vector<int>& height);
	template<typename T> void bubble_sort(vector<T>& input);
	auto test_func1() -> void;
	vector<int> two_sum(vector<int>& nums, const int target);
	vector<int> two_sum2(vector<int>& nums, const int target);
	ListNode* addTwoNumbers1(ListNode* l1, ListNode* l2);
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);
	int lengthOfLongestSubstring(string s);
	int trap0(vector<int>& height);
	int trap1(vector<int>& height);
	double findMedianSortedArrays0(vector<int>& nums1, vector<int>& nums2);
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2);
	void nextPermutation(vector<int>& nums);
	void permuteRecursive(vector<int> &num, int begin, vector<vector<int> > &result);
	vector<vector<int>> permute(vector<int> &num);
	void dfs(int pos, vector<int> &num, vector<vector<int>> &result);
};
