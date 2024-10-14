#include "../src/vector.h"
#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

bool failed = false;

void testing_scale(){
  std::cout << "Test 2.1 : Scale\n";
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

void testing_length(){
  std::cout << "Test 2.2 : Testing Length\n";
  int n = 3;
  std::vector<int>arr = {1 , 2 , 3};

  VectorND<int>v(n , arr);
  double length = v.length();

  //int test
  if(length != sqrt(14)) {
    std::cout << "Test 2.2 Failed : length of ";
    v.print();
    std::cout<< "is " << length << "!= " << sqrt(14) <<"\n" ;
    failed = true;
    return;
  }
  //decimal test
  std::vector<double>arr2 = {1.1 , 2.2 , 3.3};
  VectorND<double>v2(n , arr2);

  length = v2.length();

  if( !(length > 4.115 && length < 4.116) ) {
    std::cout << "Test 2.2 Failed : length of ";
    v2.print();
    std::cout<< "is "<< length << "!= 4.11582\n" ;
    failed = true;
    return;
  }

  std::cout << "All Test Passed\n";
}

void testing_add(){
  std::cout << "Test 2.3 : Testing Add\n";

  std::vector<int>ar = {1 , 2 , 3};
  std::vector<int>ar2 = {2 , 4 , 6};
  std::vector<int>ar3 = {1 , 2 , 3, 4};


  VectorND<int>v(3 , ar);
  VectorND<int>v2(3 , ar2);
  VectorND<int>v3(4 , ar3);

  try{
    v.add(v2);
    for(int i=0; i<3; i++){
      if(v[i] != ar[i] + ar2[i]){
        std::cout << "Test 2.3 Failed : " << v[i] << "!=" << ar[i] + ar2[i] << "\n";
        failed = true;
        return;
      }
    }

  }catch(std::exception e){
     std::cout << "Test 2.3 Failed : Unexpected Exception\n";
        failed = true;
        return;
  }
 try{
    v.add(v3);
    std::cout << "Test 2.3 Failed : Did not throw exception\n";
    failed = true;
    return;

  }catch(std::exception e){
     std::cout << "Test 2.3 Passed\n";
  }



}

void test_dot_product() {
    std::cout << "Test 2.4 : Testing Dot Product\n";
    
    // Test case 1: Integer vectors
    std::vector<int> ar1 = {1, 2, 3};
    std::vector<int> ar2 = {4, 5, 6};
    VectorND<int> v1(3, ar1);
    VectorND<int> v2(3, ar2);
    
    double result = v1.dotProduct(v2);
    double expected = 32; // 1*4 + 2*5 + 3*6
    
    if (std::abs(result - expected) > 1e-6) {
        std::cout << "Test 2.4 Failed: Integer vectors dot product " << result << " != " << expected << "\n";
        failed = true;
        return;
    }
    
    // Test case 2: Double vectors
    std::vector<double> ar3 = {1.5, 2.5, 3.5};
    std::vector<double> ar4 = {2.5, 3.5, 4.5};
    VectorND<double> v3(3, ar3);
    VectorND<double> v4(3, ar4);
    
    result = v3.dotProduct(v4);
    expected = 1.5*2.5 + 2.5*3.5 + 3.5*4.5;
    
    if (std::abs(result - expected) > 1e-6) {
        std::cout << "Test 2.4 Failed: Double vectors dot product " << result << " != " << expected << "\n";
        failed = true;
        return;
    }
    
    // Test case 3: Mismatched dimensions
    std::vector<int> ar5 = {1, 2, 3, 4};
    VectorND<int> v5(4, ar5);
    
    try {
        v1.dotProduct(v5);
        std::cout << "Test 2.4 Failed: Did not throw exception for mismatched dimensions\n";
        failed = true;
        return;
    } catch (const std::invalid_argument& e) {
        // Expected exception
    }
    
    std::cout << "Test 2.4 Passed\n";
}

void test_angle_with_vec() {
    std::cout << "Test 2.5 : Testing Angle With Vector\n";
    
    // Test case 1: Orthogonal vectors
    std::vector<double> ar1 = {1, 0, 0};
    std::vector<double> ar2 = {0, 1, 0};
    VectorND<double> v1(3, ar1);
    VectorND<double> v2(3, ar2);
    
    double result = v1.angleWithVec(v2);
    double expected = M_PI / 2; // 90 degrees
    
    if (std::abs(result - expected) > 1e-6) {
        std::cout << "Test 2.5 Failed: Orthogonal vectors angle " << result << " != " << expected << "\n";
        failed = true;
        return;
    }
    
    // Test case 2: Same direction vectors
    std::vector<double> ar3 = {2, 2, 2};
    std::vector<double> ar4 = {4, 4, 4};
    VectorND<double> v3(3, ar3);
    VectorND<double> v4(3, ar4);
    
    result = v3.angleWithVec(v4);
    expected = 0; // 0 degrees
    
    if (std::abs(result - expected) > 1e-6) {
        std::cout << "Test 2.5 Failed: Same direction vectors angle " << result << " != " << expected << "\n";
        failed = true;
        return;
    }
    
    // Test case 3: Opposite direction vectors
    std::vector<double> ar5 = {1, 1, 1};
    std::vector<double> ar6 = {-1, -1, -1};
    VectorND<double> v5(3, ar5);
    VectorND<double> v6(3, ar6);
    
    result = v5.angleWithVec(v6);
    expected = M_PI; // 180 degrees
    
    if (std::abs(result - expected) > 1e-6) {
        std::cout << "Test 2.5 Failed: Opposite direction vectors angle " << result << " != " << expected << "\n";
        failed = true;
        return;
    }
    
    std::cout << "Test 2.5 Passed\n";
}
int main (int argc, char *argv[]) {
  //Test 2 : Testing Vector Ops
  //
  std::cout<<"Test 2 : Testing Vector Ops\n";

  //Test 2.1
  testing_scale();
  
  //Test 2.2
  testing_length();
  
  //Test 2.3
  testing_add();

  //Test 2.4
  test_dot_product();
  
  //Test 2.5
  test_angle_with_vec();
  if(!failed) std::cout << "All Tests passed\n";
  return 0;
}
