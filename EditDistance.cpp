/*The edit distance between two strings is the minimum number of operations required to transform one string into the other.
The allowed operations are:

Add one character to the string.
Remove one character from the string.
Replace one character in the string.

For example, the edit distance between LOVE and MOVIE is 2, because you can first replace L with M, and then add I.
Your task is to calculate the edit distance between two strings.
Input
The first input line has a string that contains n characters between A–Z.
The second input line has a string that contains m characters between A–Z.
*/
#include "bits/stdc++.h"
using namespace std;
#define int long long
const int MOD = 1e9 + 7;
void solve(){
 string a,b;cin>>a>>b;
 int n = a.size();
 int m = b.size();
 vector<vector<int>>dp(n+1,vector<int>(m+1));
 for(int i=0;i<=n;i++)dp[i][0]=i;
 for(int j=0;j<=m;j++)dp[0][j]=j;
 for(int i=1;i<=n;i++){
    for(int j=1;j<=m;j++){
        if(a[i-1]==b[j-1]){
            dp[i][j]=dp[i-1][j-1];
        }else{
            int first = dp[i-1][j]+1;
            int second = dp[i][j-1]+1;
            int third = dp[i-1][j-1]+1;
            dp[i][j]=min({first,second,third});
        }
    }
 }
 cout<<dp[n][m]<<endl;
}
 
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int t=1;//cin>>t;
    while(t--)solve();
}

