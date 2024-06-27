#include "make_shared.hpp"
#include <iostream>

int main() {
    auto ptr = make_shared<int>(5);
    std::cout << *(ptr.get()) << std::endl;
    return 0;
}
