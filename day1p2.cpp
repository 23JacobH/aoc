#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

int main()
{
  const int MAX_VALUE = 99999;
  set<int> left;
  vector<int> freq(MAX_VALUE+1, 0);
  ifstream input_file("input.txt");
  string line;
  while (getline(input_file, line))
  {
    stringstream ss(line);
    int l, r;
    ss >> l >> r;
    left.insert(l);
    if (r >= 0 && r <= MAX_VALUE)
      freq[r]++;
    // freq_left[l]++;
    // freq_right[r]++;
  }
  input_file.close();
  long long ans = 0;
  for (auto l : left)
  {
    ans += l * freq[l];
  }
  cout << ans << endl;
  return 0;
}