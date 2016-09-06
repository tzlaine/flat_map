#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <random>


enum map_impl_kind
{
    boost_flat_map,
    std_map,
    sorted_vector,
    sorted_vector_custom_pair,

    num_map_impl_kinds
};

template <typename T, typename U>
struct sorted_vec_map
{
    using value_type = std::pair<T, U>;

    typename std::vector<value_type>::iterator lower_bound(T const & t)
    {
        value_type value{t, U()};
        return std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
    }

    U& operator[](T const & t)
    {
        value_type value{t, U()};
        return v.insert(lower_bound(t), value)->second;
    }

    void erase(T const & t)
    {
        auto it = lower_bound(t);
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

    typename std::vector<value_type>::iterator lower_bound(T const & t)
    {
        value_type value{t, U()};
        return std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type lhs, value_type rhs){
                return lhs.first < rhs.first;
            }
        );
    }

    U& operator[](T const & t)
    {
        value_type value{t, U()};
        return v.insert(lower_bound(t), value)->second;
    }

    void erase(T const & t)
    {
        auto it = lower_bound(t);
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

template <typename T, map_impl_kind MapImpl>
struct map_impl
{
    using type = std::map<int, T>;
};

template <typename T>
struct map_impl<T, boost_flat_map>
{
    using type = boost::container::flat_map<int, T>;
};

template <typename T>
struct map_impl<T, sorted_vector>
{
    using type = sorted_vec_map<int, T>;
};

template <typename T>
struct map_impl<T, sorted_vector_custom_pair>
{
    using type = sorted_vec_map_custom_pair<int, T>;
};

template <typename T, map_impl_kind MapImpl>
using map_impl_t = typename map_impl<T, MapImpl>::type;

template <typename T, map_impl_kind MapImpl, typename Rand>
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

struct output_files_t
{
    std::ofstream ofs[num_map_impl_kinds];
};

template <typename T, map_impl_kind MapImpl, typename RandFn>
void test_map_type(int iterations, std::string kind_name, std::vector<int> const & v, RandFn Rand, output_files_t & output_files)
{
    using dur = std::chrono::duration<double>;

    std::chrono::time_point<std::chrono::system_clock> t_prev;
    std::chrono::time_point<std::chrono::system_clock> t_now;

    std::vector<map_impl_t<T, MapImpl>> maps(iterations);

    output_files.ofs[MapImpl] << "    {'size': " << v.size() << ", ";

    kind_name += ':';
    kind_name += std::string(40 - kind_name.size(), ' ');

    t_prev = std::chrono::system_clock::now();
    for (auto & map : maps) {
        map = make_map<T, MapImpl>(v, Rand);
    }
    t_now = std::chrono::system_clock::now();
    auto elapsed = dur(t_now - t_prev).count() * 1000;
    output_files.ofs[MapImpl] << "'insert': " << elapsed << ",";
    std::cout << "  " << kind_name << elapsed << " ms insert\n";

    t_prev = std::chrono::system_clock::now();
    int key_sum = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
    for (auto const & map : maps) {
        for (auto const & element : map) {
            key_sum += element.first;
        }
    }
    t_now = std::chrono::system_clock::now();
    elapsed = dur(t_now - t_prev).count() * 1000;
    output_files.ofs[MapImpl] << "'iterate': " << elapsed << ",";
    std::cout << "  " << kind_name << elapsed << " ms iterate\n";
    if (key_sum == 2)
        std::cout << "  SURPRISE! key_sum=" << key_sum << "\n";

    t_prev = std::chrono::system_clock::now();
    int find_count = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
    for (auto & map : maps) {
        for (auto e : v) {
            auto const it = map.lower_bound(e);
            find_count += it != end(map);
        }
    }
    t_now = std::chrono::system_clock::now();
    elapsed = dur(t_now - t_prev).count() * 1000;
    output_files.ofs[MapImpl] << "'lower bound': " << elapsed << ",";
    std::cout << "  " << kind_name << elapsed << " ms lower bound\n";
    if (find_count == 2)
        std::cout << "  SURPRISE! key_sum=" << find_count << "\n";

    t_prev = std::chrono::system_clock::now();
    for (auto & map : maps) {
        for (auto e : v) {
            map.erase(e);
        }
    }
    t_now = std::chrono::system_clock::now();
    elapsed = dur(t_now - t_prev).count() * 1000;
    output_files.ofs[MapImpl] << "'erase': " << elapsed << ",";
    std::cout << "  " << kind_name << elapsed << " ms erase\n";

    output_files.ofs[MapImpl] << "},\n";
}

template <typename T>
void test(std::size_t size, output_files_t & output_files)
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

    test_map_type<T, boost_flat_map>(iterations, "boost flat_map", v, rand, output_files);
    test_map_type<T, std_map>(iterations, "std::map", v, rand, output_files);
    test_map_type<T, sorted_vector>(iterations, "vector", v, rand, output_files);
    test_map_type<T, sorted_vector_custom_pair>(iterations, "vector (custom-pair)", v, rand, output_files);

    std::cout << "\n";
}

#define TEST(T, size)                                           \
    std::cout << #T << ", " << (size) << " elements:\n";        \
    test<T>((size), output_files)

int main()
{
    output_files_t output_files;
    output_files.ofs[boost_flat_map].open("boost_flat_map.py");
    output_files.ofs[std_map].open("std_map.py");
    output_files.ofs[sorted_vector].open("vector.py");
    output_files.ofs[sorted_vector_custom_pair].open("vector_custom_pair.py");

    for (auto & of : output_files.ofs) {
        of << "int_timings = [\n";
    }

    TEST(int, 8u);
    TEST(int, 8u << 2);
    TEST(int, 8u << 4);
    TEST(int, 8u << 6);
    TEST(int, 8u << 8);
    TEST(int, 8u << 10);
#if 0
    TEST(int, 8u << 12);
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n\n"
           << "struct_timings = [\n";
    }

    TEST(largish_struct, 8u);
    TEST(largish_struct, 8u << 2);
    TEST(largish_struct, 8u << 4);
    TEST(largish_struct, 8u << 6);
    TEST(largish_struct, 8u << 8);
    TEST(largish_struct, 8u << 10);
#if 0
    TEST(largish_struct, 8u << 12);
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n";
    }

    return 0;
}
