#include <iostream>
#include "../src/vector.h"

bool failed = false;

void settingDimensionsTest();
void settingComponentsWithConstructor();
void settingComponentsWithSetter();

int main (int argc, char *argv[]) {
  std::cout << "Test 1 : Creating Vector\n";
  
  //test 1.1
  settingDimensionsTest();

   //test 1.2
  settingComponentsWithConstructor();

  //test 1.3
  settingComponentsWithSetter();

  if(!failed)
      std::cout << "\n\n\nAll Test Passed\n";
  return 0;
}

void settingDimensionsTest(){
  int n = 2;
  VectorND<int>v(n);
  
  std::cout << "Test 1.1 : Checking Dimensions After Creation\n";
  if(v.getDimension() != n) {
    std::cout << "Test 1.1 Failed\n";
    failed = true;
    return;
  }
  std::cout << "Test 1.1 Passed\n";
}

void settingComponentsWithConstructor(){
  std::cout << "Test 1.2 : Setting Componets with constructor\n";
  int n = 3;
  VectorND<int>v(n);
  int ar[3] = {1 , 2 , 3};

  for(int i=0; i<n; i++){
    if(ar[i] != v[i]){
      std::cout << ar[i] << "!=" << v[i] <<"\n";
      std::cout << "Test Failed\n";
      failed = true;
      return ;
    }
  }
  std::cout <<"Test 1.2 Passed\n"; 
}


void settingComponentsWithSetter(){
  std::cout << "Test 1.3 : Setting Componets With Setter\n";
  int n = 4;
  VectorND<int>v(n);
  int ar[4] = {1 , 2 , 3 , 4};

  v.setComponents(ar);

  for(int i=0; i<n; i++){
    if(ar[i] != v[i]){
      std::cout << ar[i] << "!=" << v[i] <<"\n";
      std::cout << "Test Failed\n";
      failed = true;
      return ;
    }
  }
  std::cout <<"Test 1.3 Passed\n"; 
} 








