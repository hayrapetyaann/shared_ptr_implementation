#include "shared.hpp"

int main() {
    shared_ptr<int> shared1(new int(90));
    
    shared_ptr<int> shared2(shared1); // copy ctor

    shared_ptr<int> shared3(std::move(shared2)); // move ctor

    shared_ptr<int> shared4(shared1);

    std::cout << shared1.use_count() << std::endl; // 3

    weak_ptr<int> weak = shared1;
    weak_ptr<int> weak2 = weak;

    if (auto tmp = weak.lock()) { // "shared still in memory"
        std::cout << "shared still in memory " << std::endl;
    } else {
        std::cout << "shared ptr is not in memory " << std::endl;
    }

    weak_ptr<int> weak3 = shared2; // shared2 was moved (std::move)

    if (auto tmp = weak3.lock()) { // "shared still in memory"
        std::cout << "shared still in memory " << std::endl;
    } else {
        std::cout << "shared ptr is not in memory " << std::endl;
    }
}
