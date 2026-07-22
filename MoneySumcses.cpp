/* You have n coins with certain values. Your task is to find all money sums you can create using these coins.
Input
The first input line has an integer n: the number of coins.
The next line has n integers x_1,x_2,\dots,x_n: the values of the coins.
Output
First print an integer k: the number of distinct money sums. After this, print all possible sums in increasing order.
*/
#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    bitset<100005> dp;
    dp[0] = 1;

    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        dp |= (dp << x);
    }

    vector<int> sums;
    for (int i = 1; i < 100005; ++i) {
        if (dp[i]) {
            sums.push_back(i);
        }
    }

    cout << sums.size() << "\n";
    for (int i = 0; i < sums.size(); ++i) {
        cout << sums[i] << (i + 1 == sums.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
