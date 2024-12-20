#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>

using namespace std;

struct State
{
  int x, y;
  int cheatPhase; // 0,1,2 as defined
  int steps;
  int cheatStartX, cheatStartY;
  int cheatEndX, cheatEndY;
};

struct Cheat
{
  int startX, startY;
  int endX, endY;
  int cheatedTime;
};

struct StateKey
{
  int x, y;
  int cheatPhase;
  int cheatStartX, cheatStartY;

  bool operator<(const StateKey &o) const
  {
    if (x != o.x)
      return x < o.x;
    if (y != o.y)
      return y < o.y;
    if (cheatPhase != o.cheatPhase)
      return cheatPhase < o.cheatPhase;
    if (cheatStartX != o.cheatStartX)
      return cheatStartX < o.cheatStartX;
    return cheatStartY < o.cheatStartY;
  }
};

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int main()
{
  vector<string> grid;
  ifstream input_file("input2.txt");
  string line;
  while (getline(input_file, line))
  {
    grid.push_back(line);
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
  vector<vector<int>> distNoCheat(n, vector<int>(m, -1));
  {
    queue<pair<int, int>> q;
    distNoCheat[endX][endY] = 0;
    q.push({endX, endY});
    while (!q.empty())
    {
      auto [x, y] = q.front();
      q.pop();
      for (int k = 0; k < 4; k++)
      {
        int nx = x + dx[k], ny = y + dy[k];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m)
          continue;
        if (grid[nx][ny] == '#')
          continue;
        if (distNoCheat[nx][ny] == -1)
        {
          distNoCheat[nx][ny] = distNoCheat[x][y] + 1;
          q.push({nx, ny});
        }
      }
    }
  }
  int baseTime = distNoCheat[startX][startY];
  if (baseTime == -1)
  {
    // No path without cheating
    cout << 0 << "\n";
    return 0;
  }

  // We'll do BFS with a priority queue or a normal queue (since all costs are 1).
  // dist map: Keyed by (x, y, cheatPhase, cheatStartX, cheatStartY) -> shortest steps
  map<StateKey, int> dist;
  queue<State> Q;
  {
    State s;
    s.x = startX;
    s.y = startY;
    s.cheatPhase = 0;
    s.steps = 0;
    s.cheatStartX = -1;
    s.cheatStartY = -1;
    s.cheatEndX = -1;
    s.cheatEndY = -1;

    StateKey sk = {startX, startY, 0, -1, -1};
    dist[sk] = 0;
    Q.push(s);
  }

  vector<Cheat> cheats;
  while (!Q.empty())
  {
    State cur = Q.front();
    Q.pop();

    // If we finished cheat and reached end
    if (cur.x == endX && cur.y == endY && cur.cheatPhase == 2)
    {
      Cheat c;
      c.startX = cur.cheatStartX;
      c.startY = cur.cheatStartY;
      c.endX = cur.cheatEndX;
      c.endY = cur.cheatEndY;
      c.cheatedTime = cur.steps;
      cheats.push_back(c);
    }
    if (cur.steps + 1 > baseTime)
      break;

    for (int k = 0; k < 4; k++)
    {
      int nx = cur.x + dx[k], ny = cur.y + dy[k];
      if (nx < 0 || nx >= n || ny < 0 || ny >= m)
        continue;

      char cell = grid[nx][ny];
      bool isWall = (cell == '#');
      int ns = cur.steps + 1;

      if (cur.cheatPhase == 0)
      {
        // Move on track normally
        if (!isWall)
        {
          StateKey sk = {nx, ny, 0, -1, -1};
          if (dist.find(sk) == dist.end() || dist[sk] > ns)
          {
            dist[sk] = ns;
            State nsd = cur;
            nsd.x = nx;
            nsd.y = ny;
            nsd.steps = ns;
            Q.push(nsd);
          }
        }
        if (isWall)
        {
          StateKey sk = {nx, ny, 1, cur.x, cur.y};
          if (dist.find(sk) == dist.end() || dist[sk] > ns)
          {
            dist[sk] = ns;
            State nsd = cur;
            nsd.x = nx;
            nsd.y = ny;
            nsd.steps = ns;
            nsd.cheatPhase = 1;
            nsd.cheatStartX = cur.x;
            nsd.cheatStartY = cur.y;
            Q.push(nsd);
          }
        }
      }
      else if (cur.cheatPhase == 1)
      {
        if (!isWall)
        {
          // cheat finished
          StateKey sk = {nx, ny, 2, cur.cheatStartX, cur.cheatStartY};
          if (dist.find(sk) == dist.end() || dist[sk] > ns)
          {
            dist[sk] = ns;
            State nsd = cur;
            nsd.x = nx;
            nsd.y = ny;
            nsd.steps = ns;
            nsd.cheatPhase = 2;
            nsd.cheatStartX = cur.cheatStartX;
            nsd.cheatStartY = cur.cheatStartY;
            nsd.cheatEndX = nx;
            nsd.cheatEndY = ny;
            Q.push(nsd);
          }
        }
      }
      else if (cur.cheatPhase == 2)
      {
        if (!isWall)
        {
          StateKey sk = {nx, ny, 2, cur.cheatStartX, cur.cheatStartY};
          if (dist.find(sk) == dist.end() || dist[sk] > ns)
          {
            dist[sk] = ns;
            State nsd = cur;
            nsd.x = nx;
            nsd.y = ny;
            nsd.steps = ns;
            Q.push(nsd);
          }
        }
      }
    }
  }

  int ans = 0;
  for (auto &c : cheats)
  {
    if (baseTime - c.cheatedTime >= 40)
      ans++;
    cout << baseTime - c.cheatedTime << endl;
    // vector<string> tempGrid = grid;
    // if (c.startX >= 0 && c.startY >= 0 && c.startX < n && c.startY < m)
    //   tempGrid[c.startX][c.startY] = '1';
    // if (c.endX >= 0 && c.endY >= 0 && c.endX < n && c.endY < m)
    //   tempGrid[c.endX][c.endY] = '2';

    // cout << "Time saved: " << (baseTime - c.cheatedTime) << " picoseconds\n";
    // for (auto &row : tempGrid)
    // {
    //   cout << row << "\n";
    // }
    // cout << "\n";
    // ans++;
  }
  // cout << ans << "\n";

  return 0;
}