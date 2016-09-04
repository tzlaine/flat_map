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
    SortedVector,
    SortedVectorCustomPair
};

template <typename T, typename U>
struct sorted_vec_map
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

    void erase(T const & t)
    {
        value_type value{t, U()};
        auto it = std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
        if (it != v.end())
            v.erase(it);
    }

    std::vector<value_type> v;
};

template <typename T, typename U>
typename std::vector<std::pair<T, U>>::const_iterator begin(sorted_vec_map<T, U> const & c)
{ return c.v.begin(); }
template <typename T, typename U>
typename std::vector<std::pair<T, U>>::const_iterator end(sorted_vec_map<T, U> const & c)
{ return c.v.end(); }

template <typename T, typename U>
struct custom_pair
{
    T first;
    U second;
};

template <typename T, typename U>
struct sorted_vec_map_custom_pair
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

    void erase(T const & t)
    {
        value_type value{t, U()};
        auto it = std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
        if (it != v.end())
            v.erase(it);
    }

    std::vector<value_type> v;
};

template <typename T, typename U>
typename std::vector<custom_pair<T, U>>::const_iterator begin(sorted_vec_map_custom_pair<T, U> const & c)
{ return c.v.begin(); }
template <typename T, typename U>
typename std::vector<custom_pair<T, U>>::const_iterator end(sorted_vec_map_custom_pair<T, U> const & c)
{ return c.v.end(); }

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
struct map_impl<T, SortedVector>
{
    using type = sorted_vec_map<int, T>;
};

template <typename T>
struct map_impl<T, SortedVectorCustomPair>
{
    using type = sorted_vec_map_custom_pair<int, T>;
};

template <typename T, MapImplKind MapImpl>
using map_impl_t = typename map_impl<T, MapImpl>::type;

template <typename T, MapImplKind MapImpl, typename Rand>
auto make_map(std::vector<int> const & v, Rand const & rand) -> map_impl_t<T, MapImpl>
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

template <typename T, MapImplKind MapImpl, typename RandFn>
void test_map_type(int iterations, std::string kind_name, std::vector<int> const & v, RandFn Rand)
{
    using dur = std::chrono::duration<double>;

    std::chrono::time_point<std::chrono::system_clock> t_prev;
    std::chrono::time_point<std::chrono::system_clock> t_now;

    std::vector<map_impl_t<T, MapImpl>> maps(iterations);

    kind_name += ':';
    kind_name += std::string(40 - kind_name.size(), ' ');

    t_prev = std::chrono::system_clock::now();
    for (auto & map : maps) {
        map = make_map<T, MapImpl>(v, Rand);
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  " << kind_name << (dur(t_now - t_prev).count() * 1000) << " ms insert\n";

    t_prev = std::chrono::system_clock::now();
    int key_sum = 0; // To ensure the optimizer does not remove the loops below altogether, take a sum.
    for (auto const & map : maps) {
        for (auto const & element : map) {
            key_sum += element.first;
        }
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  " << kind_name << (dur(t_now - t_prev).count() * 1000) << " ms iterate\n";
    if (key_sum == 2)
        std::cout << "  SURPRISE! key_sum=" << key_sum << "\n";

    t_prev = std::chrono::system_clock::now();
    for (auto & map : maps) {
        for (auto e : v) {
            map.erase(e);
        }
    }
    t_now = std::chrono::system_clock::now();
    std::cout << "  " << kind_name << (dur(t_now - t_prev).count() * 1000) << " ms erase\n";
}

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

    int const iterations = 5;

    using dur = std::chrono::duration<double>;

    std::chrono::time_point<std::chrono::system_clock> t_prev;
    std::chrono::time_point<std::chrono::system_clock> t_now;

    test_map_type<T, FlatMap>(iterations, "boost flat_map", v, rand);
    test_map_type<T, NodeBased>(iterations, "std::map", v, rand);
    test_map_type<T, SortedVector>(iterations, "vector", v, rand);
    test_map_type<T, SortedVectorCustomPair>(iterations, "vector (custom-pair)", v, rand);

    std::cout << "\n";
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
    TEST(int, 100000u);
#if 0
    TEST(int, 1000000u);
    TEST(int, 10000000u);
    TEST(int, 100000000u);
#endif

    TEST(largish_struct, 10u);
    TEST(largish_struct, 100u);
    TEST(largish_struct, 1000u);
    TEST(largish_struct, 10000u);
    TEST(largish_struct, 100000u);
#if 0
    TEST(largish_struct, 1000000u);
    TEST(largish_struct, 10000000u);
    TEST(largish_struct, 100000000u);
#endif

    return 0;
}
