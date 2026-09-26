#include <bits/stdc++.h>
using namespace std;

void solve(int tc) {
    string s, str_t;
    cin >> s >> str_t;

    int n = s.size();
    int m = str_t.size();

    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (s[i] == str_t[j]) {
                dp[i][j] = 1 + dp[i + 1][j + 1];
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }

    
    if (dp[0][0] == 0) {
        cout << "Case " << tc << ": :(\n";
        return;
    }

    string ans = "";
    int i = 0, j = 0;

    
    while (dp[i][j] > 0) {
        for (char c = 'a'; c <= 'z'; c++) {
            int pos_s = -1, pos_t = -1;

            
            for (int k = i; k < n; k++) {
                if (s[k] == c) {
                    pos_s = k;
                    break;
                }
            }

           
            for (int k = j; k < m; k++) {
                if (str_t[k] == c) {
                    pos_t = k;
                    break;
                }
            }

           
            if (pos_s != -1 && pos_t != -1 && dp[pos_s + 1][pos_t + 1] + 1 == dp[i][j]) {
                ans.push_back(c);
                i = pos_s + 1;
                j = pos_t + 1;
                break; 
            }
        }
    }

    cout << "Case " << tc << ": " << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int test_cases;
    if (cin >> test_cases) {
        for (int tc = 1; tc <= test_cases; tc++) {
            solve(tc);
        }
    }
    return 0;
}