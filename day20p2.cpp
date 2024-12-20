#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <map>

using namespace std;

struct Cell
{
  int x, y;
};

struct CheatInfo
{
  int startX, startY;
  int endX, endY;
  int savedTime;
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main()
{
  ifstream input_file("input.txt");
  vector<string> grid;
  {
    string line;
    while (getline(input_file, line))
    {
      grid.push_back(line);
    }
  }
  input_file.close();
  int n = (int)grid.size();
  int m = (int)grid[0].size();
  int startX, startY, endX, endY;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (grid[i][j] == 'S')
      {
        startX = i;
        startY = j;
      }
      if (grid[i][j] == 'E')
      {
        endX = i;
        endY = j;
      }
    }
  }
  vector<vector<int>> distFromStart(n, vector<int>(m, -1));
  {
    queue<Cell> q;
    distFromStart[startX][startY] = 0;
    q.push({startX, startY});
    while (!q.empty())
    {
      Cell c = q.front();
      q.pop();
      for (int k = 0; k < 4; k++)
      {
        int nx = c.x + dx[k], ny = c.y + dy[k];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m)
          continue;
        if (grid[nx][ny] == '#')
          continue;
        if (distFromStart[nx][ny] == -1)
        {
          distFromStart[nx][ny] = distFromStart[c.x][c.y] + 1;
          q.push({nx, ny});
        }
      }
    }
  }
  vector<vector<int>> distToEnd(n, vector<int>(m, -1));
  {
    queue<Cell> q;
    distToEnd[endX][endY] = 0;
    q.push({endX, endY});
    while (!q.empty())
    {
      Cell c = q.front();
      q.pop();
      for (int k = 0; k < 4; k++)
      {
        int nx = c.x + dx[k], ny = c.y + dy[k];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m)
          continue;
        if (grid[nx][ny] == '#')
          continue;
        if (distToEnd[nx][ny] == -1)
        {
          distToEnd[nx][ny] = distToEnd[c.x][c.y] + 1;
          q.push({nx, ny});
        }
      }
    }
  }
  int baseTime = distFromStart[endX][endY];
  cout << "baseTime: " << baseTime << endl;
  set<tuple<int, int, int, int, int>> foundCheatsSet;
  vector<CheatInfo> foundCheats;
  for (int ax = 1; ax < n; ax++)
  {
    cout << ax << endl;
    for (int ay = 1; ay < m; ay++)
    {
      for (int bx = ax; bx < n && bx <= ax+19; bx++)
      {
        for (int by = ay; by < m && by <= ay+19; by++)
        {
          if (ax == bx && ay == by)
            continue;
          int manDist = abs(ax-bx) + abs(ay-by);
          if (manDist > 19)
            continue;
          if (distFromStart[ax][ay] == -1 || distFromStart[bx][by] == -1)
            continue;
          if (distToEnd[ax][ay] == -1 || distToEnd[bx][by] == -1)
            continue;
          // a to b
          int savedTime = baseTime - distFromStart[ax][ay] - manDist - distToEnd[bx][by];
          while (savedTime >= 100)
          {
            auto key = make_tuple(savedTime, ax, ay, bx, by);
            if (foundCheatsSet.find(key) == foundCheatsSet.end())
            {
              foundCheatsSet.insert(key);
              foundCheats.push_back({ax, ay, bx, by, savedTime});
            }
            savedTime -= 2;
          }
          // b to a
          savedTime = baseTime - distFromStart[bx][by] - manDist - distToEnd[ax][ay];
          while (savedTime >= 100)
          {
            auto key2 = make_tuple(savedTime, bx, by, ax, ay);
            if (foundCheatsSet.find(key2) == foundCheatsSet.end())
            {
              foundCheatsSet.insert(key2);
              foundCheats.push_back({bx, by, ax, ay, savedTime});
            }
            savedTime -= 2;
          }
        }
      }
    }
  }
  int ans = 0;
  // for (auto &c : foundCheats)
  // {
  //   if (c.savedTime >= 100)
  //     ans++;
  // }
  cout << "answer: " << foundCheats.size() << endl;
  // map<int, int> freq;
  // for (auto &c : foundCheats)
  // {
  //   freq[c.savedTime]++;
  //   if (c.savedTime == 82)
  //   {
  //     cout << c.startX << " " << c.startY << " " << c.endX << " " << c.endY << endl;
  //   }
  // }
  // for (auto &entry : freq)
  // {
  //   cout << entry.second << " : " << entry.first << "\n";
  // }
  return 0;
}