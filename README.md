# SecretData
Compile time concealment of critical data.

### Sample:
```c++
#include <iostream>

#include <secret.hpp>


int main() {
    // Build a new string that is hidden at compile time.
    constexpr auto s = secret::str("Hello, World!");

    // Use the dereference operator to retrieve a std::basic_string containing a copy of
    // the plaintext data
    std::cout << *s << std::endl;
    return 0;
}
```

