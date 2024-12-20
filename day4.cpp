#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int r, c;
const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
const string order = "XMAS";

int check(vector<string>& puzzle, int x, int y)
{
  int ret = 0;
  if (puzzle[x][y] != 'X') return 0;
  for (int i = 0; i < 8; i++)
  {
    bool good = true;
    for (int j = 1; j < 4; j++)
    {
      int nx = x+j*dx[i];
      int ny = y+j*dy[i];
      if (nx < 0 || nx >= r || ny < 0 || ny >= c || puzzle[nx][ny] != order[j])
          good = false;
      if (!good) break;
    } 
    if (good) ret++;
  }
  return ret;
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