#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        int rows = matrix.size();
        int cols = matrix[0].size();
        
        // Add an extra 0 at the end to force the stack to flush at the end of the row
        vector<int> heights(cols + 1, 0); 
        int max_area = 0;
        
        for (int i = 0; i < rows; ++i) {
            // Step 1: Update the heights histogram for the current row
            for (int j = 0; j < cols; ++j) {
                if (matrix[i][j] == '1') {
                    heights[j]++;
                } else {
                    heights[j] = 0;
                }
            }
            
            // Step 2: Calculate max area for this histogram using a monotonic stack
            stack<int> st;
            for (int j = 0; j <= cols; ++j) {
                // While current height is less than the height of the bar at stack top
                while (!st.empty() && heights[j] < heights[st.top()]) {
                    int h = heights[st.top()];
                    st.pop();
                    
                    // The width is the distance from the current index j (right boundary) 
                    // to the new top of the stack (left boundary).
                    int w = st.empty() ? j : (j - st.top() - 1);
                    
                    max_area = max(max_area, h * w);
                }
                // Push current index to stack
                st.push(j);
            }
        }
        
        return max_area;
    }
};
