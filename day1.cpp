#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

int main()
{
  // const int MAX_VALUE = 99999;
  // vector<int> freq_left(MAX_VALUE + 1, 0), freq_right(MAX_VALUE + 1, 0);
  vector<int> left, right;
  ifstream input_file("input.txt");
  string line;
  int m = -1;
  while (getline(input_file, line))
  {
    stringstream ss(line);
    int l, r;
    ss >> l >> r;
    left.push_back(l);
    right.push_back(r);
    m = max(m, l);
    // freq_left[l]++;
    // freq_right[r]++;
  }
  input_file.close();
  // int ans = 0;
  // int carry_left = 0, carry_right = 0;
  // for (int i = 0; i <= MAX_VALUE; i++)
  // {
  //   carry_left += freq_left[i];
  //   carry_right += freq_right[i];
  //   int match = min(carry_left, carry_right);
  //   ans += match * i;
  //   carry_left -= match;
  //   carry_right -= match;
  // }
  sort(left.begin(), left.end());
  sort(right.begin(), right.end());
  int ans = 0;
  for (int i = 0; i < left.size(); i++)
    ans += abs(left[i]-right[i]);
  cout << ans << endl;
  cout << m << endl;
  return 0;
}