# SecretData
Compile time concealment of critical data.

### Sample:
```c++
#include <iostream>

#include <secret.hpp>

int main() {
    constexpr auto s = secret::str("Hello, World!");

    std::cout << *s << std::endl;
    return 0;
}
```

