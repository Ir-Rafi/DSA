/* You are given an integer n. On each step, you may subtract one of the digits from the number.
How many steps are required to make the number equal to 0?
Input
The only input line has an integer n.
Output
Print one integer: the minimum number of steps.
*/



#include "bits/stdc++.h"
using namespace std;
#define int long long
const int MOD = 1e9 + 7;
void solve(){
 int n;cin>>n;
 const int INF = 1e9;
 vector<int>a(n);
 for(int i=0;i<n;i++)cin>>a[i];
 vector<int>dp(n+1,INF);
 dp[0]=0;
 for(int i=1;i<=n;i++){
    int x = i;
    while(x>0){
        int digit = x%10;
        x/=10;
        if(digit==0)continue;
        dp[i]=min(dp[i],dp[i-digit]+1);
    }
 }
 cout<<dp[n]<<endl;
 
}
 
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int t=1;//cin>>t;
    while(t--)solve();
}
