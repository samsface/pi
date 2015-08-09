#include <vector>

namespace kissvec 
{
   std::vector<float> mult(const std::vector<float>& x, const std::vector<std::vector<float>>& y) {
      std::vector<float> res;

      res = { x[0]*y[0][0] + x[1]*y[1][0] + x[2]*y[2][0],
              x[0]*y[0][1] + x[1]*y[1][1] + x[2]*y[2][1],
              x[0]*y[0][2] + x[1]*y[1][2] + x[2]*y[2][2] }; 
 
     return res;
   } 
}
