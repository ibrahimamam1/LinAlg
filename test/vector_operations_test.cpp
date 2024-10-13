#include "../src/vector.h"
#include <iostream>
#include <vector>

bool failed = false;

void testing_scale(){
  int n = 3;
  int scalar = 2;
  std::vector<int>ar= {1 , 2 , 3};

  VectorND<int>v(n , ar);

  v.scale(scalar);
  for(int i=0; i<n; i++){
    if(v[i] != ar[i]*scalar){
      std::cout<< "Test 2.1 Failed : At position : "<< i << v[i] << " != " << ar[i] * scalar << "\n";
      failed = true;
      return;
    }
  }
  std::cout<< "Test 2.1 Passed\n";
}

int main (int argc, char *argv[]) {
  //Test 2 : Testing Vector Ops
  //
  std::cout<<"Test 2 : Testing Vector Ops\n";

  //Test 2.1
  testing_scale();

  if(!failed) std::cout << "All Tests passed\n";
  return 0;
}
