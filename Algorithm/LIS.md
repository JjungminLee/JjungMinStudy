## LIS

- 가장 긴 증가하는 부분 수열
- LCS는 최장 공통 부분 수열, 이건 최장 증가 부분 수열

### DP를 이용해서 풀기 (O(N^2))

```cpp
void LIS_DP() {
    for (int i = 0; i < n; i++) {
        dp[i] = 1;            //해당 원소에서 끝나는 LIS 길이의 최솟값. 즉, 자기 자신
        for (int j = 0; j < i; j++) {
            //i번째 이전의 모든 원소에 대해, 그 원소에서 끝나는 LIS의 길이를 확인한다.
            if (arr[i] > arr[j]) {
                //단, 이는 현재 수가 그 원소보다 클 때만 확인한다.
                dp[i] = max(dp[i], dp[j] + 1);        //dp[j] + 1 : 이전 원소에서 끝나는 LIS에 현재 수를 붙인 새 LIS 길이
            }
        }
    }
}

```

### 이분탐색을 이용해서 풀기 (O(NlogN))

- 이 방법에서는 LIS를 기록하는 배열을 하나 더 두고, 원래 수열에서의 각 원소에 대해 LIS 배열 내에서의 위치를 찾는다.

```cpp
int binary_search(vector<int> lis, int start, int end, int element) {
    //이분 탐색으로 lis 벡터 내에서 element의 위치를 반환
    //lis 벡터의 start - end 구간에서만 확인
    while (start < end) {
        int mid = (start + end) / 2;
        if (element > lis[mid]) start = mid + 1;
        else end = mid;
    }
    return end;
}

int LIS_BS() {
    int ret = 0;
    vector<int> lis;
    lis.push_back(arr[0]);

    for (int i = 1; i < n; i++) {
        //만약 lis 벡터의 마지막 수보다 i번째 수가 크다면, 그냥 뒤에 붙인다.
        if (arr[i] > lis.back()) {
            lis.push_back(arr[i]);
            ret = lis.size() - 1;
        }
        //i번째 수에 대해, lis 벡터 내에서 그 수의 위치를 찾는다.
        int pos = binary_search(lis, 0, ret, arr[i]);
        lis[pos] = arr[i];
    }

    return ret + 1;
}

```

- 중요한 점이 있다면 원본 배열 범위 마지막 수가 LIS 기록 배열 맨 끝의 수보다 크다면 LIS 기록 배열 맨 끝에 추가해야한다는 것!
