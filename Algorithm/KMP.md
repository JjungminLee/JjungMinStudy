## KMP

- 문자열 검색 알고리즘
- brute force방식으로 문자열 매칭을 구현한다면 시간복잡도는 O(nm)
- KMP를 사용하면 O(N+M)에 해결할 수 있다!

### KMP를 알기 위해서 이해해야하는 것

1. 접두사와 접미사

- apple이 있다면
  - 접두사는 a ap app appl apple
  - 접미사는 e le ple pple apple

2. pi배열

- ABAABAB 의 pi배열 구하면
  A 0
  AB 0
  ABA 1 (접두사 A, 접미사 A같음)
  ABAA 1
  ABAAAB 2
  ABAABA 3
  ABAABAB 2

3. 일치했다는 사실 이용하기

- 접두사와 접미사가 일치하는 마지막 부분의 정보를 사용한다!
- 텍스트 "ABCDABCDABEE"에서 패턴 "ABCDABE"를 찾는 상황을 생각해보면
  - 6번째부터 불일치한다.
  - 일치하는 부분에 집중해보자.
  - ABCDAB에서 접두사와 접미사가 일치하는것은 (0,1) (4,5) 부분이다!
  - 고로 pi[5]=2이다!
  - 일치하는 부분에 집중해서 나중에 껑충 뛸 수 있게한다!
  - 한마디로 최대한 빠르게 텍스트를 가리키는 포인터를 뒤로 넘겨주는 작업을 하는것!
    - 이걸 안하면 이제 텍스트를 가리키는 포인터가 0번,1번,2번 .. 요런식으로 움직이는데
    - 이 작업을 하면 0번,9번 이런식으로 움직이기 가능!

```cpp
#include <iostream>

#include <string>

#include <cstdio>

#include <vector>

using namespace std;

vector<int> getPi(string p)
{

    int m = (int)p.size(), j = 0;

    vector<int> pi(m, 0);

    for (int i = 1; i < m; i++)
    {

        while (j > 0 && p[i] != p[j])

            j = pi[j - 1];

        if (p[i] == p[j])

            pi[i] = ++j;
    }

    return pi;
}

vector<int> kmp(string s, string p)
{

    vector<int> ans;

    auto pi = getPi(p);

    int n = (int)s.size(), m = (int)p.size(), j = 0;

    for (int i = 0; i < n; i++)
    {

        while (j > 0 && s[i] != p[j])

            j = pi[j - 1];

        if (s[i] == p[j])
        {

            if (j == m - 1)
            {

                ans.push_back(i - m + 1);

                j = pi[j];
            }
            else
            {

                j++;
            }
        }
    }

    return ans;
}

int main()
{

    string s, p;

    getline(cin, s);

    getline(cin, p);

    auto matched = kmp(s, p);

    printf("%d\n", (int)matched.size());

    for (auto i : matched)

        printf("%d ", i + 1);

    return 0;
}

```

엄청난 도움을 주신 글!
https://bowbowbow.tistory.com/6
