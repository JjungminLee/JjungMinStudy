## 다익스트라 알고리즘

- 최단거리 구하기
- 하나의 최단거리를 구할때 이전까지 구했던 최단거리 정보를 그대로 사용
- 우선순위 큐를 통해 구현한다면 O(ElogV)의 시간복잡돌르 보장함
- 이전처럼 모든 원소를 선형탐색하는 것이 아닌 힙 자료구조를 통해 빠르게 작동하도록 만들었다.

### BFS와의 차이?

- BFS는 가중치가 없는 경우에 쓰이고, 다익스트라는 가중치가 있는 경우에 쓰인다.
- 음수간선이 없을때 다익스트라가 쓰인다.
- [BFS 문제 예시] 첫째 줄에 지나야 하는 최소의 칸 수를 출력한다. (근데 칸 수에 위치만 존재할 뿐 가중치는 없음)
- [다익스트라 문제 예시] 제일 오른쪽 아래 칸인 [N-1][N-1]까지 이동해야 한다. 링크가 잃을 수밖에 없는 최소 금액은 얼마일까? (칸에 가중치가 있음)
- 가장 결정적인 차이는 **다익스트라는 어떤 선택을 하느냐에 따라 그래프도 달라져서 비용도 변하는** 반면 **bfs는 어떤 선택을 하느냐에 따라 그래프가 변하지 않는다**

### 예제

- 거쳐서 가는 비용이 더 작다면 그걸 포함해서 갱신, 아니면 패스!

```cpp
#include <stdio.h>
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
#define INF 1e9 // 무한을 의미하는 값으로 10억을 설정

// 노드의 개수(N), 간선의 개수(M), 시작 노드 번호(Start)
// 노드의 개수는 최대 100,000개라고 가정
int n, m, start;

vector<pair<int, int> > graph[100001]; // 각 노드에 연결되어 있는 노드에 대한 정보를 담는 배열
int d[100001]; // 최단 거리 테이블 만들기

void dijkstra(int start)
{
    priority_queue<pair<int,int>>pq; // 거리, 노드 인덱스

    pq.push({0,start}); //시작 노드로 가기위한 최단 경로는 0으로 설정하여, 큐에 삽입.
    d[start]=0;

    while(!pq.empty())
    {
        int dist = -pq.top().first; //현재 노드까지의 비용
        int now = pq.top().second; // 현재 노드
        pq.pop();

        if(d[now]<dist) // 이미 최단경로를 체크한 노드인 경우 패스
            continue;

        for(int i=0; i<graph[now].size(); i++)
        {
            int cost = dist+graph[now][i].second; // 거쳐서 가는 노드의 비용을 계산
                                                  // 현재노드까지 비용 + 다음 노드 비용
            if(cost<d[graph[now][i].first]) // 비용이 더 작다면 최단경로 테이블 값을 갱신.
            {
                d[graph[now][i].first]=cost;
                pq.push(make_pair(-cost,graph[now][i].first));
            }
        }
    }
}

int main(void)
{
    cin >> n >> m >> start;

    // 모든 간선 정보를 입력받기
    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        // a번 노드에서 b번 노드로 가는 비용이 c라는 의미
        graph[a].push_back({b, c});
    }

    // 최단 거리 테이블을 모두 무한으로 초기화
    fill(d, d + 100001, INF);

    // 다익스트라 알고리즘을 수행
    dijkstra(start);

    // 모든 노드로 가기 위한 최단 거리를 출력
    for (int i = 1; i <= n; i++)
    {
        // 도달할 수 없는 경우, 무한(INFINITY)이라고 출력
        if (d[i] == INF) {
            cout << "INFINITY" << '\n';
        }
        // 도달할 수 있는 경우 거리를 출력
        else {
            cout << d[i] << '\n';
        }
    }
}
```

- 녹색옷을 입은 애가 젤다지? 문제

  - 문제 자체는 매우 쉬움

```cpp
 priority_queue<pair<int, pair<int, int>>> pq;
        // 우선순위큐는 내림차순이 기본이기에 반대로
        pq.push({-arr[0][0], {0, 0}});
        dist[0][0] = arr[0][0];
        while (!pq.empty())
        {
            int cost = -pq.top().first;
            int x = pq.top().second.first;
            int y = pq.top().second.second;
            pq.pop();
            // 네 방향으로 이동하기!
            for (int i = 0; i < 4; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < n && ny >= 0 && ny < n)
                {
                    if (dist[nx][ny] > arr[nx][ny] + cost)
                    {
                        dist[nx][ny] = arr[nx][ny] + cost;
                        pq.push({-dist[nx][ny], {nx, ny}});
                    }
                }
            }
        }
```

- 고려해야할 사항
  - 우선순위 큐는 기본적으로 내림차순이기에 이 문제는 오름차순을 사용함 (- 붙여주는걸로)
  - 인접행렬을 고민할때 -> 네방향 탐색을 바로 들어가서 탐색함!
    (이걸 고민했는데 쉽게 해결할 수 있었다!)
