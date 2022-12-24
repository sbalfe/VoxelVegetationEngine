#include <iostream>
#include <fmt/format.h>
#include <cstdlib>
#include <vector>

template<typename Container> struct GenericStruct {
 public:
  using size_type = Container::size_type;

};

int main(){
  GenericStruct<std::vector<uint32_t>> test_struct;
}