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

/**
 * @brief Simple allocator that zero's out memory before deallocation.
 */ 
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

/**
 * @brief Secret string data, capable of holding binary data. Should not be used directly.
 */
template <std::size_t N>
class String
{
public:
    /**
     * @brief Plaintext type. Zero's out memory on destruction.
     */
    using pt = std::basic_string<char, std::char_traits<char>, ZeroAlloc<char>>;

    /**
     * @brief Builds a new hidden string out of a string literal
     */
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

    /**
     * @brief Syntactic sugar for to_pt().
     */
    pt operator*() const {
        return to_pt();
    }

    /**
     * @brief Returns a new plaintext container string.
     */
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

template<typename T, std::size_t N>
class Array
{
public:
    class pt : public std::array<T, N> {
    public:
        ~pt() {
            std::memset(this->data(), 0, sizeof(T) * N);
        }

    private:
        friend class Array;
        pt() = default;
    };

    constexpr Array(T const * const l)
        : _ct() {
        for (size_t i = 0; i < N; ++i) {
            _ct[i] = l[i];
        }
        conceal();
    }

    pt operator*() const {
        return to_pt();
    }

    pt to_pt() const {
        pt _pt;

        size_t j = 0;

        for (size_t i = 0; i < N; ++i) {
            T t = 0;
            for (size_t k = 0; k < sizeof(T); ++k) {
                if (j % OBFKEY_LEN == 0) {
                    j = 0;
                }

                t |= (OBFKEY[j] << ((sizeof(T) - k) * 8));

                ++j;
            }

            _pt[i] = _ct[i] ^ t;
        }

        return _pt;
    }

public:
    constexpr void conceal()
    {
        size_t j = 0;

        for (auto &at : _ct) {
            T t = 0;
            for (size_t k = 0; k < sizeof(T); ++k) {
                if (j % OBFKEY_LEN == 0) {
                    j = 0;
                }

                t |= (OBFKEY[j] << ((sizeof(T) - k) * 8) - 1);

                ++j;
            }

            at = at ^ t;
        }
    }

    std::array<T, N> _ct;
};

/**
 * @brief Builds a secret string as a constexpr. Resolved at compile time and should
 * prevent the plaintext string from being contained in the binary. Should double
 * check... compilers are finicky.
 *
 * @param l A string literal to be concealed.
 * @return A new ObfStr container with the concealed string.
 */
template <std::size_t N>
constexpr auto str(const char (&l)[N])
{
    return String<N>(l);
}

template <typename T, std::size_t N>
constexpr auto arr(const T (&l)[N])
{
    return Array<T, N>(l);
}

}

#endif // OBFSTR_HPP
