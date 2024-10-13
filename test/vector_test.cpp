#include <exception>
#include <iostream>
#include <vector>
#include "../src/vector.h"

bool failed = false;

void settingDimensionsTest();
void settingComponentsWithConstructor();
void settingComponentsWithSetter();
void checking_dimension_mismatch_with_higher_value();
void checking_dimension_mismatch_with_lower_value();


int main (int argc, char *argv[]) {
  std::cout << "Test 1 : Creating Vector\n";
  
  //test 1.1
  settingDimensionsTest();

   //test 1.2
  settingComponentsWithConstructor();

  //test 1.3
  settingComponentsWithSetter();

  //test 1.4
  checking_dimension_mismatch_with_higher_value();

  //test 1.5
  checking_dimension_mismatch_with_lower_value();

  if(!failed)
      std::cout << "\n\n\nAll Test Passed\n";
  return 0;
}

void settingDimensionsTest(){
  int n = 2;
  VectorND<int>v(n);
  
  std::cout << "Test 1.1 : Checking Dimensions After Creation\n";
  if(v.getDimension() != n) {
    std::cout << "\tTest 1.1 Failed\n";
    failed = true;
    return;
  }
  std::cout << "\tTest 1.1 Passed\n";
}

void settingComponentsWithConstructor(){
  std::cout << "Test 1.2 : Setting Componets with constructor\n";
  int n = 3;
  std::vector<int>ar(n);
  ar = {1 , 2 , 3};
  VectorND<int>v(n , ar);

  for(int i=0; i<n; i++){
    if(ar[i] != v[i]){
      std::cout << ar[i] << "!=" << v[i] <<"\n";
      std::cout << "\tTest 1.2 Failed\n";
      failed = true;
      return ;
    }
  }
  std::cout <<"\tTest 1.2 Passed\n"; 
}


void settingComponentsWithSetter(){
  std::cout << "Test 1.3 : Setting Componets With Setter\n";
  int n = 4;
  VectorND<int>v(n);
  std::vector<int> ar(n);
  ar = {1 , 2 , 3 , 4};

  v.setComponents(ar);

  for(int i=0; i<n; i++){
    if(ar[i] != v[i]){
      std::cout << ar[i] << "!=" << v[i] <<"\n";
      std::cout << "\tTest Failed\n";
      failed = true;
      return ;
    }
  }
  std::cout <<"\tTest 1.3 Passed\n"; 
} 

void checking_dimension_mismatch_with_higher_value(){
  std::cout << "Test 1.4 : Testing Dimension Mistmatch With Higher Value\n";

  int n = 4;
  VectorND<int>v(n);

  std::vector<int>ar(5);
  ar = {1 , 2 , 3 , 4 , 5};
  
  try{
    v.setComponents(ar);
    std::cout << "Test 1.4 failed\n";
    failed = true;
  }catch(std::exception e){
    std::cout << e.what();
    std::cout << "\n\tTest 1.4 Passed\n";
  }
 }

void checking_dimension_mismatch_with_lower_value(){
  std::cout << "Test 1.5 : Testing Dimension Mistmatch With Lower Value\n";

  int n = 7;
  VectorND<int>v(n);

  std::vector<int>ar(5);
  ar = {1 , 2 , 3 , 4 , 5};
  
    v.setComponents(ar);
    for(int i=n-1; i<ar.size(); i++){
     if(v[i] != 0){
      std::cout << "Test 1.5 failed\n At index %d " << i << v[n-1] << "!= 0\n";
      failed = true;
      return;
      }
  }
      std::cout << "\n\tTest 1.5 Passed\n";
  }




