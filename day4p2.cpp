#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int r, c;

int check(vector<string> &puzzle, int x, int y)
{
  if (x == 0 || x == r || y == 0 || y == r || puzzle[x][y] != 'A')
    return 0;
  char tl = puzzle[x-1][y-1];
  char tr = puzzle[x-1][y+1];
  char bl = puzzle[x+1][y-1];
  char br = puzzle[x+1][y+1];
  if ((tl != 'M' || br != 'S') && (tl != 'S' || br != 'M'))
    return 0;
  if ((tr != 'M' || bl != 'S') && (tr != 'S' || bl != 'M'))
    return 0;
  return 1;
}

int main()
{
  vector<string> puzzle;
  ifstream input_file("input.txt");
  string line;
  while (getline(input_file, line))
    puzzle.push_back(line);
  r = puzzle.size();
  c = puzzle[0].size();
  int ans = 0;
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      ans += check(puzzle, i, j);
    }
  }
  cout << ans << endl;
}