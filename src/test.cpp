#include <iostream>
#include <string>
#include <list>

void test(std::list<int> l) {
    for(auto i : l){
        std::cout << i << std::endl;        
    }
}

int main()
{
  test( {1, 3, 2} );
}
