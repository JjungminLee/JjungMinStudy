## LCS

- 최장 공통부분 수열
- 최장 공통부분 문자열
  [LCS 이해에 큰 도움을 주신 글](https://velog.io/@emplam27/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-%EA%B7%B8%EB%A6%BC%EC%9C%BC%EB%A1%9C-%EC%95%8C%EC%95%84%EB%B3%B4%EB%8A%94-LCS-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-Longest-Common-Substring%EC%99%80-Longest-Common-Subsequence)

### 최장 공통부분 문자열

#### 점화식

- 문자열 A, 문자열 B를 한글자씩 각각 비교한다.
- 두 문자가 다르면 lcs[i][j]=0
- 두 문자가 같으면 lcs[i][j]=lcs[i-1][j-1]+1

#### 2차원 배열을 생성해서 비교하자

![image](https://github.com/user-attachments/assets/03419962-6f07-4944-8907-e1b47230dafd)
![image](https://github.com/user-attachments/assets/a4649bfe-88fc-428c-8a40-e179be0fb70f)

### 최장 공통부분 수열

- 문자열 A, 문자열 B를 한글자씩 비교한다.
- 두 문자가 다르면 lcs[i-1][j] 와 lcs[i][j-1] 중 큰 값 표시
- 두 문자가 같다면 lcs[i][j]=lcs[i-1][j-1]+1

### 코드

```cpp
#include <bits/stdc++.h>
using namespace std;
string a;
string b;
int lcs[1001][1001];
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> a;
    cin >> b;
    for (int i = 1; i <= a.length(); i++)
    {
        for (int j = 1; j <= b.length(); j++)
        {
            if (a[i - 1] == b[j - 1])
            {
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            }
            else
            {
                lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);
            }
        }
    }

    cout << lcs[a.length()][b.length()] << endl;
}
```
