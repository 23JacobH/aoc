#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool safe(vector<int>& v)
{
  if (v.size() < 2)
    return true;
  int inc = v[v.size()-1] > v[0] ? 1 : -1;
  for (int i = 1; i < v.size(); i++)
  {
    int diff = inc * (v[i] - v[i-1]);
    if (diff < 1 || diff > 3)
      return false;
  }
  return true;
}

bool safe2(vector<int> &v)
{
  if (v.size() < 2)
    return true;
  for (int i = 0; i < v.size(); i++)
  {
    vector<int> sub_v;
    for (int j = 0; j < v.size(); j++)
    {
      if (j != i)
        sub_v.push_back(v[j]);
    }
    if (safe(sub_v))
      return true;
  }
  return false;
}

int main()
{
  ifstream input_file("input.txt");
  string line;
  int ans = 0;
  while (getline(input_file, line))
  {
    stringstream ss(line);
    vector<int> v;
    int n;
    while (ss >> n)
      v.push_back(n);
    if (safe2(v))
      ans++;
  }
  cout << ans << endl;
}