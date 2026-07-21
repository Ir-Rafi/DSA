#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        
        // Edge cases for very small neighborhoods
        if (n == 1) return nums[0];
        if (n == 2) return max(nums[0], nums[1]);
        
        // Return the maximum of the two linear scenarios
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }
    
private:
    // Helper function to solve the linear House Robber problem
    int robLinear(const vector<int>& nums, int start, int end) {
        int prev1 = 0; // Represents the max money robbed up to the previous house (i-1)
        int prev2 = 0; // Represents the max money robbed up to the house before last (i-2)
        
        for (int i = start; i <= end; ++i) {
            // We either skip the current house (keep prev1), 
            // or rob it and add its value to prev2.
            int current = max(prev1, prev2 + nums[i]);
            
            // Shift our window forward for the next iteration
            prev2 = prev1;
            prev1 = current;
        }
        
        return prev1;
    }
};
