/* Your task is to find a minimum-price flight route from Syrjälä to Metsälä. You have one discount coupon, using which you can halve the price of any single flight during the route. However, you can only use the coupon once.
When you use the discount coupon for a flight whose price is x, its price becomes \lfloor x/2 \rfloor (it is rounded down to an integer).
Input
The first input line has two integers n and m: the number of cities and flight connections. The cities are numbered 1,2,\ldots,n. City 1 is Syrjälä, and city n is Metsälä.
After this there are m lines describing the flights. Each line has three integers a, b, and c: a flight begins at city a, ends at city b, and its price is c. Each flight is unidirectional.
You can assume that it is always possible to get from Syrjälä to Metsälä.
*/
#include"bits/stdc++.h"
using namespace std;
#define int long long
const int INF = 1e18;
struct State{
 int cost,city,coupon_used;
 bool operator>(const State& other)const{
    return cost>other.cost;
 }
};
signed main(){
int n,m;cin>>n>>m;
 vector<vector<pair<int,int>>>adj(n+1);
 for(int i=1;i<=m;i++){
    int u,v,w;
    cin>>u>>v>>w;
    adj[u].push_back({v,w});
 }
 vector<vector<int>>dist(n+1,vector<int>(2,INF));
 dist[1][0]=0;
 priority_queue<State,vector<State>,greater<State>>pq;
 pq.push({0,1,0});
 while(!pq.empty()){
    State curr = pq.top();
    pq.pop();
    int u = curr.city;
    int d = curr.cost;
    int c = curr.coupon_used;
     if(d>dist[u][c])continue;
     for(auto edge : adj[u]){
        int v = edge.first;
        int w = edge.second;
        if(c==0){
         if(dist[u][0]+w < dist[v][0]){
            dist[v][0] = dist[u][0] + w;
            pq.push({dist[v][0], v, 0});
         }
         if(dist[u][0]+w/2 < dist[v][1]){
            dist[v][1] = dist[u][0]+w/2;
            pq.push({dist[v][1],v,1});
         }
        }else if(c==1){
            if(dist[u][1]+w < dist[v][1]){
                dist[v][1] = dist[u][1] + w;
               pq.push({dist[v][1],v,1});
            }
        }
     }
 }
 cout<<dist[n][1]<<endl;
    return 0;
}
