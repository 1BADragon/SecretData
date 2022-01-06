#ifndef OBFSTR_HPP
#define OBFSTR_HPP

#include <cstddef>
#include <cstring>
#include <string>
#include <memory>
#include <stdexcept>

#ifndef OBFKEY
#define OBFKEY "a"
#define OBFKEY_LEN (1)
#else
#ifndef OBFKEY_LEN
#error "OBFKEY_LEN must be defined if OBFKEY is defined!!!"
#endif
#endif

namespace secret {

template <typename T>
class ZeroAlloc {
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;

    ZeroAlloc() = default;
    ZeroAlloc(const ZeroAlloc &other) :_alloc(other._alloc) {}

    T * allocate(size_t size, const void * = 0)
    {
        return _alloc.allocate(size);
    }

    void deallocate(T * ptr, size_t size)
    {
        std::memset(ptr, 0, sizeof(T) * size);
        _alloc.deallocate(ptr, size);
    }

private:
    std::allocator<T> _alloc;
};

template <std::size_t N>
class String
{
public:
    using pt = std::basic_string<char, std::char_traits<char>, ZeroAlloc<char>>;

    constexpr String(const char (&l)[N])
        : _ct()
    {
        size_t j = 0;
        for (size_t i = 0; i < N; ++i) {
            if (j % OBFKEY_LEN == 0) {
                j = 0;
            }
            _ct[i] = l[i] ^ OBFKEY[j];
            ++j;
        }
    }

    pt operator*() const {
        return to_pt();
    }

    pt to_pt() const {
        pt _pt;

        size_t j = 0;
        for (size_t i = 0; i < N; ++i) {
            if (j % OBFKEY_LEN == 0) {
                j = 0;
            }
            _pt.push_back(_ct[i] ^ OBFKEY[j]);
            ++j;
        }

        return _pt;
    }

private:
    char _ct[N];
};

template <std::size_t N>
constexpr auto str(const char (&l)[N])
{
    return String<N>(l);
}

}

#endif // OBFSTR_HPP
