#include <iostream>
#include <secret.hpp>

int main()
{
    constexpr auto a = secret::arr({1, 2, 3});

    for (auto &pt : *a) {
        std::cout << pt << std::endl;
    }

    return 0;
}
