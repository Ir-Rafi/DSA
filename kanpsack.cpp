#include <bits/stdc++.h>
using namespace std;
#define int long long
signed main() {
    int n;
    long long W;
    cin >> n >> W;

    vector<int> w(n), v(n);
    int sumValue = 0;

    for (int i = 0; i < n; i++) {
        cin >> w[i] >> v[i];
        sumValue += v[i];
    }

    const long long INF = 1e18;
    vector<long long> dp(sumValue + 1, INF);

    dp[0] = 0; 

    for (int i = 0; i < n; i++) {
        for (int val = sumValue; val >= v[i]; val--) {
            dp[val] = min(dp[val], dp[val - v[i]] + w[i]);
        }
    }

    int ans = 0;
    for (int val = 0; val <= sumValue; val++) {
        if (dp[val] <= W) ans = val;
    }

    cout << ans <<endl;
    return 0;
}
