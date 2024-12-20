#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main()
{
  ifstream input_file("input.txt");
  string s, line;
  while (getline(input_file, line))
  {
    s.append(line);
  }
  regex mul("mul\\((\\d+),(\\d+)\\)");
  regex ctrl("(do\\(\\)|don't\\(\\))");
  auto mul_begin = sregex_iterator(s.begin(), s.end(), mul);
  auto mul_end = sregex_iterator();
  auto ctrl_begin = sregex_iterator(s.begin(), s.end(), ctrl);
  auto ctrl_end = sregex_iterator();
  int sum = 0;
  bool include_product = true;
  auto control_iter = ctrl_begin;
  for (auto mul_iter = mul_begin; mul_iter != mul_end; ++mul_iter)
  {
    smatch mul_match = *mul_iter;
    int num1 = stoi(mul_match[1].str());
    int num2 = stoi(mul_match[2].str());
    while (control_iter != ctrl_end && control_iter->position() < mul_iter->position())
    {
      string control_match = (*control_iter).str();
      if (control_match == "do()")
        include_product = true;
      else if (control_match == "don't()")
        include_product = false;
      ++control_iter;
    }
    if (include_product)
      sum += num1 * num2;
  }
  cout << sum << endl;
}