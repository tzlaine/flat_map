#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <vector>
#include <random>


enum MapImplKind
{
    FlatMap,
    NodeBased,
    LogVector,
    LogVectorCustomPair
};

template <typename T, typename U>
struct log_vec_map
{
    using value_type = std::pair<T, U>;

    U& operator[](T const & t)
    {
        value_type value(t, U());
        auto it = std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
        return v.insert(it, value)->second;
    }

    std::vector<value_type> v;
};

template <typename T, typename U>
struct custom_pair
{
    T first;
    U second;
};

template <typename T, typename U>
struct log_vec_map_custom_pair
{
    using value_type = custom_pair<T, U>;

    U& operator[](T const & t)
    {
        value_type value{t, U()};
        auto it = std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
        return v.insert(it, value)->second;
    }

    std::vector<value_type> v;
};

template <typename T, MapImplKind MapImpl>
struct map_impl
{
    using type = std::map<int, T>;
};

template <typename T>
struct map_impl<T, FlatMap>
{
    using type = boost::container::flat_map<int, T>;
};

template <typename T>
struct map_impl<T, LogVector>
{
    using type = log_vec_map<int, T>;
};

template <typename T>
struct map_impl<T, LogVectorCustomPair>
{
    using type = log_vec_map_custom_pair<int, T>;
};

template <typename T, MapImplKind MapImpl>
using map_impl_t = typename map_impl<T, MapImpl>::type;

template <typename T, MapImplKind MapImpl, typename Rand>
auto make_map(std::vector<int> & v, Rand const & rand) -> map_impl_t<T, MapImpl>
{
    map_impl_t<T, MapImpl> m;
    for (auto e : v)
    {
        m[e] = T(0);
    }

    return m;
}

struct largish_struct
{
    largish_struct() {}
    largish_struct(int k) : key{k} {}
    int key;
    std::array<double, 5> data;
};

template <typename T>
void test(std::size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist;

    auto rand = [&]() {
        return dist(gen);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), rand);

    int const iterations = 7;

    using dur = std::chrono::duration<double>;

    std::chrono::time_point<std::chrono::system_clock> t_prev =
        std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> t_now;

    for (int i = 0; i < iterations; ++i) {
        auto flat = make_map<T, FlatMap>(v, rand);
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  boost flat_map:              " << (dur(t_now - t_prev).count() * 1000)<< " ms\n";
    t_prev = t_now;

    for (int i = 0; i < iterations; ++i) {
        auto lumpy = make_map<T, NodeBased>(v, rand);
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  std::map:                    " << (dur(t_now - t_prev).count() * 1000)<< " ms\n";
    t_prev = t_now;

    for (int i = 0; i < iterations; ++i) {
        auto log_vector = make_map<T, LogVector>(v, rand);
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  vector-log:                  " << (dur(t_now - t_prev).count() * 1000)<< " ms\n";
    t_prev = t_now;


    for (int i = 0; i < iterations; ++i) {
        auto log_vector = make_map<T, LogVectorCustomPair>(v, rand);
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  vector-log (custom-pair):    " << (dur(t_now - t_prev).count() * 1000)<< " ms\n";
    t_prev = t_now;

    std::cout << "\n";

#if 0
    for (auto pair : flat) {
        std::cout << pair.first << "\n";
    }
#endif
}

#define TEST(T, size)                                   \
    std::cout << #T << ", " << size << " elements:\n";  \
    test<T>(size)

int main()
{
    TEST(int, 10u);
    TEST(int, 100u);
    TEST(int, 1000u);
    TEST(int, 10000u);
#if 0
    TEST(int, 100000u);
    TEST(int, 1000000u);
    TEST(int, 10000000u);
    TEST(int, 100000000u);
#endif

    TEST(largish_struct, 10u);
    TEST(largish_struct, 100u);
    TEST(largish_struct, 1000u);
    TEST(largish_struct, 10000u);
#if 0
    TEST(largish_struct, 100000u);
    TEST(largish_struct, 1000000u);
    TEST(largish_struct, 10000000u);
    TEST(largish_struct, 100000000u);
#endif

    return 0;
}
