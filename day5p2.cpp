#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
  vector<string> input;
  ifstream input_file("input.txt");
  string line;
  while (getline(input_file, line))
  {
    input.push_back(line);
  }
}