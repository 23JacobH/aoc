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
  int baseTime = distToEnd[startX][startY];
  cout << "baseTime: " << baseTime << endl;
  vector<vector<int>> distNoCheat(n, vector<int>(m, -1));
  distNoCheat[startX][startY] = 0;
  queue<Cell> Q;
  Q.push({startX, startY});
  set<tuple<int, int, int, int, int>> foundCheatsSet;
  vector<CheatInfo> foundCheats;
  while (!Q.empty())
  {
    Cell cur = Q.front();
    Q.pop();
    int cx = cur.x, cy = cur.y;
    int curSteps = distNoCheat[cx][cy];
    for (int k = 0; k < 4; k++)
    {
      int nx = cx + dx[k], ny = cy + dy[k];
      if (nx < 0 || nx >= n || ny < 0 || ny >= m)
        continue;
      if (grid[nx][ny] == '#')
        continue;
      int ns = curSteps + 1;
      if (distNoCheat[nx][ny] == -1 || distNoCheat[nx][ny] > ns)
      {
        distNoCheat[nx][ny] = ns;
        Q.push({nx, ny});
      }
    }
    for (int d1 = 0; d1 < 4; d1++)
    {
      int wx = cx + dx[d1], wy = cy + dy[d1];
      if (wx < 0 || wx >= n || wy < 0 || wy >= m)
        continue;
      if (grid[wx][wy] != '#')
        continue;
      for (int d2 = 0; d2 < 4; d2++)
      {
        int tx = wx + dx[d2], ty = wy + dy[d2];
        if (tx < 0 || tx >= n || ty < 0 || ty >= m)
          continue;
        if (grid[tx][ty] == '#')
          continue;
        if (distToEnd[cx][cy] == -1 || distToEnd[tx][ty] == -1)
          continue;
        int diff = distToEnd[cx][cy] - distToEnd[tx][ty]; // from c to t
        if (diff > 2)
        {
          int savedTime = diff-2;
          auto key = make_tuple(savedTime, cx, cy, tx, ty);
          if (foundCheatsSet.find(key) == foundCheatsSet.end())
          {
            foundCheatsSet.insert(key);
            foundCheats.push_back({cx, cy, tx, ty, savedTime});
          }
        }
      }
    }
  }
  int ans = 0;
  for (auto &c : foundCheats)
  {
    if (c.savedTime >= 100)
      ans++;
  }
  cout << "answer: " << ans << endl;
  map<int, int> freq;
  for (auto &c : foundCheats)
  {
    freq[c.savedTime]++;
    if (c.savedTime == 82)
    {
      cout << c.startX << " " << c.startY << " " << c.endX << " " << c.endY << endl;
    }
  }
  for (auto &entry : freq)
  {
    cout << entry.second << " : " << entry.first << "\n";
  }
  return 0;
}