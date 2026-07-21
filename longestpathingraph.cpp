#include <bits/stdc++.h>
using namespace std;
#define int long long
signed main() {
    int n,m;cin>>n>>m;
    vector<vector<int>>g(n+1);
    vector<int>in(n+1,0);
    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;
        g[x].push_back(y);
        in[y]++;
    }
    vector<int>dp(n+1,0);
    queue<int>q;
    for(int i=1;i<=n;i++){
        if(in[i]==0)q.push(i);
    }
    while(!q.empty()){
        int u=q.front();
            q.pop();
            for(int v:g[u]){
            dp[v] = max(dp[v], dp[u] + 1);
            in[v]--;
            if (in[v] == 0) {
                q.push(v);
            }
    }
    }
    int ans=0;
    for(int i=1;i<=n;i++){
        ans=max(ans,dp[i]);
    }
    cout<<ans<<endl;
    return 0;
}
