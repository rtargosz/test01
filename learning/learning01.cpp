#include "Learning01.h"

// Find the longest palindrome substring (not entire string)
// e.g.: ababc could return aba or bab
// but cbbc could only return bb, NOT cbbc
string CLearning::longestPalindrome(string s)
{
	// Algorithm
	// 1. is string longer than 2 characters and non-null?
	if (s.empty() || s.length() < 2) return "";

	// 2. Remove non-alpha characters and convert to lower case
	s.erase(remove_if(s.begin(), s.end(), [](char c) { return !isalpha(c); }), s.end());
	transform(s.begin(), s.end(), s.begin(), ::tolower);

	// 3. Check entire string first
	auto t0 = s, r0 = s;
	reverse(r0.begin(), r0.end());
	if (t0 == r0) return t0;

	// 4. palindrome is the same backwards as it is forwards
	string temp = "";

	// Brute force
	for (auto i = 0; i < s.length() - 2; i++) // stop 2 characters short of the last one
	{
		for (auto j = i + 2; j < s.length(); j++)
		{
			auto t1 = s.substr(i, j);
			auto r1 = s.substr(i, j);
			reverse(r1.begin(), r1.end());

			// Check if substring is a palindrome and longer than the previous one
			if (t1 == r1 && t1.length() > temp.length())
			{
				temp = s.substr(i, j);
			}
		}
	}

	return temp;
}

// Google problems
// 1. Given a positive integer, return TRUE if the sum and product of the digits are equal
//    e.g.: 123 -> 1+2+3 = 1*2*3, return TRUE
//
bool CLearning::check_int(int x)
{
	auto prod = 1, sum = 0;
	do
	{
		auto temp = static_cast<int>(x - trunc(x / 10) * 10);

		prod *= temp;
		sum += temp;

		x = x - temp;
	} while (x > 0);

	return (prod == sum);
}

// 2. Given an arbitrary tree (not a BST), print out the items at each level
//    e.g.:
//           1
//          / \
//         4   2
//        /\    \
//       3  9    7
//         /    / \
//        8    5   6
//   print out "1\n  4,2\n, 3,9,7\n, 8,5,6\n
//
void CLearning::print_graph(Graph& g)
{
	// Use BFS algorithm
	//map<int> m;
	queue<int> q;
}
// 3. In a match 3 game with a rectangular board nxm in size, return the total number of remaining moves/swaps
//    A swap is reversible, so only count each swapped pair once. Available colors is in the range of integers.
//    e.g.: 
//      1. swapping x down will match 3 stars (with 2 up and right)
//      2. swapping x up
//      3. swapping x left 
//      4. general case - no matter where X is swapped, it will match
//      5. general case with single matches to each side after swap
//  
//      ------- -------	------- ----------------- -------	
//      |R|R|G| |R|*|G| |R|*|G| |R|R|G|X|G|B|G|B| |X|R|X| 
//      ------- -------	------- ----------------- -------	
//      |R|R|G| |R|*|G| |R|*|G| |R|R|G|X|G|B|G|B| |G|x|G| 
//      ------- -------	------- ----------------- -------	
//      |x|*|*| |*|x|*|	|R|*|Y| |G|X|X|R|X|X|G|B| |X|R|X|	
//      ------- -------	------- ----------------- -------	
//      |*|R|G| |G|R|G|	|*|x|G| |X|X|G|x|G|X|X|B|    5
//      ------- -------	------- ----------------- 	
//      	1      2	   3    |P|P|G|R|G|B|G|B| 	
//      	        	        ----------------- 	
//      	        	        |P|P|G|X|G|B|G|B| 	
//      	        	        ----------------- 	
//      	        	        |P|P|G|X|G|B|G|B| 	
//      	        	        ----------------- 	
//                                      4
int CLearning::match_count(vector<vector<int>> g)
{
	auto matches = 0;
	for (int i = 0u; i < g.size(); i++)
	{
		for (int j = 0u; j < g.size(); j++)
		{
			// TODO: Check rules and increment matches
			matches++;
		}
	}
	return matches;
}

// Convert integer 123 into 321
//
int CLearning::reverse_int(int x)
{
	long long resdbl = x;
	long long mult = 1;
	vector<int> result = {};
	// Write digits into vector
	do
	{
		mult *= 10;
		auto temp = resdbl - trunc(x / mult) * mult;
		if (mult == 0)
			return 0;
		result.emplace_back(temp / (mult / 10));
		resdbl = trunc(resdbl / mult) * mult;
	} while (resdbl != 0);

	for (auto e : result)
	{
		mult /= 10;
		resdbl += e * mult;
	}

	return (resdbl<INT_MIN || resdbl>INT_MAX) ? 0 : resdbl;
}
int CLearning::trap(vector<int>& height)
{
	if (height.size() < 2)
		return 0;

	int left = 0, right = static_cast<int>(height.size() - 1u);
	int accumulation = 0;
	int left_max = 0, right_max = 0;
	while (left < right) {
		if (height[left] < height[right]) {
			height[left] >= left_max ? (left_max = height[left]) : accumulation += (left_max - height[left]);
			++left;
		}
		else {
			height[right] >= right_max ? (right_max = height[right]) : accumulation += (right_max - height[right]);
			--right;
		}
	}

	return accumulation;
}

template<typename T>
void CLearning::bubble_sort(vector<T>& input)
{
	auto swap_count = 0;
	auto loops = 0;
	auto total_swaps = 0;
	do
	{
		swap_count = 0;
		for (auto i = 0u; i < input.size() - 1; ++i)
		{
			if (input[i] > input[i + 1])
			{
				swap(input[i], input[i + 1]);
				swap_count++;
			}
		}
		total_swaps += swap_count;
		loops++;
	} while (swap_count > 0);
	cout << "Swaps: " << total_swaps << " Loops: " << loops << "\n";
}

auto CLearning::test_func1() -> void
{
	auto first = make_unique<int>(10);
	auto second = make_unique<int>(20);

	swap(first, second);

	vector<int> v1 = { 1, 6, 2, 5, 9, 2, 4, 6, 10 ,11 };
	vector<float> v2 = { 6.4f, 6.2f, 4.4f, 1.1f, 8.5f, 9.99f, 2.222f, 10.1010f, 0.957f };
	int a1[] = { 10, 12, 5, 3, 9, 8, 2, 3, 6, 99, 43, 54 };

	sort(begin(v1), end(v1));
	sort(begin(v2), end(v2));
	sort(begin(a1), end(a1));
}

vector<int> CLearning::two_sum(vector<int>& nums, const int target)
{
	for (auto i = 0; i < nums.size(); i++)
	{
		for (auto j = i + 1; j < nums.size(); j++)
		{
			if ((nums[i] + nums[j]) == target)
			{
				return { i, j };
			}
		}
	}

	return { 0, 0 };
}

vector<int> CLearning::two_sum2(vector<int>& nums, const int target)
{
	map<int, int> temp_map;
	temp_map.emplace(nums[0], 0);
	for (auto i = 1; i < nums.size(); i++)
	{
		auto complement = target - nums[i];
		if (end(temp_map) != temp_map.find(complement))
		{
			return { i, temp_map.at(complement) };
		}
		temp_map.emplace(nums[i], i);
	}

	return { 0, 0 };
}

// Add two linked lists - works up to overflow of double
CLearning::ListNode* CLearning::addTwoNumbers1(ListNode* l1, ListNode* l2)
{
	double result = 0.0;
	double multiplier = 1.0;
	do
	{
		if (l1 != nullptr)
		{
			result += multiplier * l1->val;
			l1 = l1->next;
		}
		if (l2 != nullptr)
		{
			result += multiplier * l2->val;
			l2 = l2->next;
		}
		multiplier *= 10;
	} while (l1 != nullptr || l2 != nullptr);

	ListNode* rList = new ListNode(0);
	ListNode** tList = &rList;
	auto resdbl = static_cast<double>(result);
	auto mult = 10;
	do
	{
		auto temp = resdbl - trunc(resdbl / mult) * mult;
		tList = &((*tList)->next);
		if (mult == 0)
			return rList->next;
		*tList = new ListNode(temp / (mult / 10));
		resdbl = trunc(resdbl / mult) * mult;
		mult *= 10;
	} while (resdbl != 0);
	return rList->next;
}

CLearning::ListNode* CLearning::addTwoNumbers(ListNode* l1, ListNode* l2)
{
	auto rList = new ListNode(0);
	ListNode** tList = &rList;
	auto carry = 0;
	while (l1 != nullptr || l2 != nullptr)
	{
		tList = &((*tList)->next);
		auto l1_val = (l1 == nullptr) ? 0 : l1->val;
		auto l2_val = (l2 == nullptr) ? 0 : l2->val;
		auto sum = l1_val + l2_val + carry;

		carry = trunc(sum / 10);
		*tList = new ListNode(sum % 10);
		if (l1 != nullptr) l1 = l1->next;
		if (l2 != nullptr) l2 = l2->next;
	};
	if (carry > 0)
	{
		tList = &((*tList)->next);
		*tList = new ListNode(carry);
	}

	return rList->next;
}

int CLearning::lengthOfLongestSubstring(string s)
{
	set<char> char_set;
	auto max_len = -1;
	auto j = 0u;

	if (s.length() <= 1)
		return static_cast<int>(s.length());

	for (auto i = 0u; i < (s.length() - 1); i++)
	{
		char_set.emplace(s[i]);
		for (j = i + 1; j < s.length(); j++)
		{
			if (s[i] == s[j])
			{
				if (max_len < static_cast<int>(j - i))
				{
					max_len = static_cast<int>(j - i);
				}
				char_set.clear();
				break;
			}

			auto it = char_set.find(s[j]);
			char_set.emplace(s[j]);
			if (it != char_set.end())
			{
				if (max_len < static_cast<int>(j - 1 - i))
				{
					max_len = static_cast<int>(j - 1 - i);
				}
				char_set.clear();
				break;
			}
		}

		if ((max_len < static_cast<int>(j - i))) //(max_len < 0) &&
		{
			max_len = static_cast<int>(j - i);
		}
	}

	return max_len;
}

int CLearning::trap0(vector<int>& height)
{
	if (height.size() < 2)
		return 0;

	// Iterate across vector from 'second' to second-last element
	// No water can accumulate on the ends
	int accumulation = 0;

	for (int pos = 1u; pos < height.size() - 1; pos++)
	{
		int left_max = 0;
		int right_max = 0;

		// For each position, find highest element to left and right
		for (int left = pos - 1; left >= 0; left--)
		{
			if (height[left] > left_max) left_max = height[left];
		}

		for (int right = pos + 1; right < height.size(); right++)
		{
			if (height[right] > right_max) right_max = height[right];
		}

		// Don't negatively accumulate
		if (height[pos] <= left_max && height[pos] <= right_max)
			accumulation += (min(left_max, right_max) - height[pos]);
	}

	return accumulation;
}

int CLearning::trap1(vector<int>& height)
{
	if (height.size() < 2)
		return 0;

	int accumulation = 0;
	// Iterate across vector from 'second' to second-last element
	// No water can accumulate on the ends
	// Store max left/right for each location
	vector<int> left_max(height.size()), right_max(height.size());

	for (int pos = 1u; pos < height.size() - 1; pos++)
	{
		const auto left_it = max_element(begin(height), begin(height) + pos);
		left_max[pos] = *left_it;
		const auto right_it = max_element(begin(height) + pos + 1, end(height));
		right_max[pos] = *right_it;

		// Don't negatively accumulate
		if (height[pos] <= left_max[pos] && height[pos] <= right_max[pos])
			accumulation += (min(left_max[pos], right_max[pos]) - height[pos]);
	}

	return accumulation;
}

double CLearning::findMedianSortedArrays0(vector<int>& nums1, vector<int>& nums2)
{
	// * NOT WORKING FULLY *
	// Median is middle number OR two mid numbers over two
	// Given two SORTED arrays, the middle will be 1/2 way through the sum of both

	// Step through both arrays in order
	// { 1, 3 }
	// { 2 }

	// Edge cases
	// 0 items
	if (nums1.empty() && nums2.empty())
		return 0.0;

	// 1 entry in each array
	if (nums1.size() == 1u && nums2.size() == 1u)
		return (nums1[0] + nums2[0]) / 2.0;

	if (nums1.size() == 1 && nums2.empty())
		return nums1[0];
	if (nums2.size() == 1 && nums1.empty())
		return nums2[0];


	// Non-edge cases ASSERT(total_len > 1)
	const auto total_len = nums1.size() + nums2.size();
	auto pos1 = 0;
	auto pos2 = 0;
	auto sum = 0.0;
	auto second = false;
	while (1 != 0)
	{
		if (total_len % 2 == 0) // even number of items (x/2 +/- 1)
		{
			// Are we at the middle?
			if ((total_len / 2u == static_cast<unsigned int>(pos1 + pos2 + 1)) || sum > 0.0)
			{
				// One array empty, other not
				if (!nums1.empty() && nums2.empty())
				{
					sum += nums1[pos1];
					if (!second)
						second = true;
					else
						return sum / 2.0;
					if (pos1 == nums1.size() - 1)
					{
						sum += nums2[pos2];
						return sum / 2.0;
					}
				}
				else if (!nums2.empty() && nums1.empty())
				{
					sum += nums2[pos2];
					if (!second)
						second = true;
					else
						return sum / 2.0;
					if (pos2 == nums2.size() - 1)
					{
						sum += nums1[pos1];
						return sum / 2.0;
					}
				}

				// Need number just below median and just above, then average them
				else if (nums1[pos1] <= nums2[pos2])
				{
					sum += nums1[pos1];
					if (!second)
						second = true;
					else
						return sum / 2.0;
					if (pos1 == nums1.size() - 1)
					{
						sum += nums2[pos2];
						return sum / 2.0;
					}
				}
				else
				{
					sum += nums2[pos2];
					if (!second)
						second = true;
					else
						return sum / 2.0;
					if (pos2 == nums2.size() - 1)
					{
						sum += nums1[pos1];
						return sum / 2.0;
					}
				}

			}
		}
		else // odd number of items (x+1)/2
		{
			if (total_len / 2u == static_cast<unsigned int>(pos1 + 1 + pos2))
			{
				// One array empty, other not
				if (!nums1.empty() && nums2.empty())
					return nums1[++pos1];
				if (!nums2.empty() && nums1.empty())
					return nums2[++pos2];

				return (nums1[pos1] <= nums2[pos2]) ? nums2[pos2] : nums1[pos1];
			}
		}
		// Do we step forward on array 1 or 2?
		if (!nums1.empty() && nums2.empty())
		{
			if (static_cast<unsigned int>(pos1 + 1u) < nums1.size())
				pos1++;
			else
				return 0.0; // error
		}
		else if (nums1.empty() && !nums2.empty())
		{
			if (static_cast<unsigned int>(pos2 + 1u) < nums2.size())
				pos2++;
			else
				return 0.0; // error
		}
		else if (nums1[pos1] <= nums2[pos2])
		{
			// Only step forward if there's another item in the array
			if (static_cast<unsigned int>(pos1 + 1u) < nums1.size())
				pos1++;
			else if (static_cast<unsigned int>(pos2 + 1u) < nums2.size())
				pos2++;
			else
				return 0.0; // error
		}
		else
		{
			// Only step forward if there's another item in the array
			if (static_cast<unsigned int>(pos2 + 1u) < nums2.size())
				pos2++;
			else if (static_cast<unsigned int>(pos1 + 1u) < nums1.size())
				pos1++;
			else
				return 0.0; // error
		}
	}
	return 0.0;
}

double CLearning::findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
{
	if (nums1.empty() && nums2.empty())
		return 0.0;

	// 1 entry in each array
	if (nums1.size() == 1u && nums2.size() == 1u)
		return (nums1[0] + nums2[0]) / 2.0;

	vector<int> nums = nums1;
	nums.insert(end(nums), begin(nums2), end(nums2));
	sort(begin(nums), end(nums));

	if (nums.size() % 2 != 0) // odd size
		return nums[nums.size() / 2.0];
	else // even size
		return (nums[trunc(nums.size() / 2.0) - 1] + nums[trunc(nums.size() / 2.0)]) / 2.0;

	return 0.0;
}

void CLearning::nextPermutation(vector<int>& nums)
{
	// Pattern: starting from right, find first digit with any lower digit to left and swap them
	// Sort digits from that point to the right in order
	if (nums.empty() || nums.size() == 1)
		return;

	for (int i = nums.size() - 1; i > 0; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			if (nums[i] > nums[j])
			{
				// Current digit is larger than a previous one, swap 'em
				auto temp = nums[i];
				nums[i] = nums[j];
				nums[j] = temp;
				sort(begin(nums) + j + 1, end(nums));
				return;
			}
		}
	}

	sort(begin(nums), end(nums));
}

// permute num[begin..end]
// invariant: num[0..begin-1] have been fixed/permuted
void CLearning::permuteRecursive(vector<int> &num, int begin, vector<vector<int> > &result)
{
	if (begin >= num.size()) {
		// one permutation instance
		result.push_back(num);
		return;
	}

	for (int i = begin; i < num.size(); i++) {
		swap(num[begin], num[i]);
		permuteRecursive(num, begin + 1, result);
		// reset
		swap(num[begin], num[i]);
	}
}

vector<vector<int> > CLearning::permute(vector<int> &num)
{
	vector<vector<int> > result;

	permuteRecursive(num, 0, result);
	return result;
}

void CLearning::dfs(int pos, vector<int> &num, vector<vector<int>> &result)
{
	if (pos == num.size()) {
		result.push_back(num);
	}
	else {
		for (int i = pos; i<num.size(); i++) {
			swap(num[i], num[pos]);
			dfs(pos + 1, num, result);
			swap(num[i], num[pos]);
		}
	}
}

auto main() -> int
{
	return 0;
}
