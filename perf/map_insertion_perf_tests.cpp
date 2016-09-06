#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
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

template <typename T, typename U, typename ValueType = std::pair<T, U>>
struct sorted_vec_map
{
    using value_type = ValueType;

    size_t size() const
    { return v.size(); }

    void reserve(size_t size)
    { v.reserve(size); }

    typename std::vector<value_type>::iterator lower_bound(T const & t)
    {
        value_type value{t, U()};
        return std::lower_bound(
            v.begin(), v.end(),
            value,
            [](value_type const & lhs, value_type const & rhs){
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

template <typename Map>
void reserve(Map & m, size_t size)
{ m.reserve(size); }

template <typename ...T>
void reserve(std::map<T...> &, size_t) {}

template <typename T, typename U>
typename std::vector<std::pair<T, U>>::const_iterator begin(sorted_vec_map<T, U> const & c)
{ return c.v.begin(); }
template <typename T, typename U>
typename std::vector<std::pair<T, U>>::const_iterator end(sorted_vec_map<T, U> const & c)
{ return c.v.end(); }

template <typename T, typename U>
struct custom_pair
{
#if 0 // Set to nonzero to see the performance of this custom type match that of std::pair.
    custom_pair() :
        first{T()}, second{U()} {}

    custom_pair(T const & t, U const & u) :
        first{t}, second{u} {}

    custom_pair(custom_pair const & rhs) noexcept(false) :
        first{rhs.first}, second{rhs.second} {}
    custom_pair(custom_pair && rhs) noexcept(false) :
        first{std::move(rhs.first)}, second{std::move(rhs.second)} {}

    custom_pair& operator=(custom_pair const & rhs) noexcept(false)
    { first = rhs.first; second = rhs.second; return *this; }
    custom_pair& operator=(custom_pair && rhs) noexcept(false)
    { first = std::move(rhs.first); second = std::move(rhs.second); return *this; }
#endif

    T first;
    U second;
};

template <typename T, typename U>
using sorted_vec_map_custom_pair = sorted_vec_map<T, U, custom_pair<T, U>>;

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

struct largish_struct
{
    largish_struct() {}
    largish_struct(int k) : key{k} {}
    operator int() {return key;}
    int key;
    std::array<double, 5> data;
};

struct output_files_t
{
    std::ofstream ofs[num_map_impl_kinds];
};

double single_elapsed_value(std::vector<double> & times)
{
    // Take the mean after throwing out the smallest and largest values.
    std::sort(times.begin(), times.end());
    return std::accumulate(times.begin() + 1, times.end() - 1, 0.0) / (times.size() - 2);
}

template <typename T, map_impl_kind MapImpl, int Iterations>
void test_map_type(std::string kind_name, std::vector<int> const & v, output_files_t & output_files)
{
    using dur = std::chrono::duration<double>;

    using map_t = map_impl_t<T, MapImpl>;

    std::vector<map_t> maps(Iterations);

    output_files.ofs[MapImpl] << "    {'size': " << v.size() << ", ";

    kind_name += ':';
    kind_name += std::string(40 - kind_name.size(), ' ');

#if 0
    std::cout << "********************************************************************************\n";
    std::cout << std::chrono::high_resolution_clock::period().num
              << " / "
              << std::chrono::high_resolution_clock::period().den
              << "\n";
#endif

    {
        std::vector<double> times;
        for (auto & map : maps) {
#if 0
            reserve(map, v.size());
#endif
            double time = 0.0;
            for (auto e : v)
            {
                auto start = std::chrono::high_resolution_clock::now();
                map[e] = T(0);
                auto stop = std::chrono::high_resolution_clock::now();
                time += dur(stop - start).count() * 1000;
            }
            times.push_back(time);
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'insert': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms insert\n";
    }

    {
        std::vector<double> times;
        int copy_sum = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
        for (auto const & map : maps) {
            std::vector<T> values(map.size());
            using value_type = typename map_t::value_type;
            auto start = std::chrono::high_resolution_clock::now();
            std::transform(
                begin(map), end(map), begin(values),
                [](value_type const & elem){ return elem.second; }
            );
            auto stop = std::chrono::high_resolution_clock::now();
            times.push_back(dur(stop - start).count() * 1000);
            for (auto x : values) {
                copy_sum += static_cast<int>(x);
            }
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'iterate': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms iterate\n";
        if (copy_sum == 2)
            std::cout << "  SURPRISE! copy_sum=" << copy_sum << "\n";
    }

    {
        std::vector<double> times;
        int key_sum = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
        for (auto & map : maps) {
            auto const end_ = end(map);
            double time = 0.0;
            for (auto e : v) {
                auto start = std::chrono::high_resolution_clock::now();
                auto const it = map.lower_bound(e);
                if (it != end_)
                    key_sum += it->first;
                auto stop = std::chrono::high_resolution_clock::now();
                time += dur(stop - start).count() * 1000;
            }
            times.push_back(time);
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'lower bound': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms lower bound\n";
        if (key_sum == 2)
            std::cout << "  SURPRISE! key_sum=" << key_sum << "\n";
    }

    {
        std::vector<double> times;
        for (auto & map : maps) {
            double time = 0.0;
            for (auto e : v) {
                auto start = std::chrono::high_resolution_clock::now();
                map.erase(e);
                auto stop = std::chrono::high_resolution_clock::now();
                time += dur(stop - start).count() * 1000;
            }
            times.push_back(time);
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'erase': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms erase\n";
    }

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

    int const iterations = 7;

    test_map_type<T, boost_flat_map, iterations>("boost flat_map", v, output_files);
    test_map_type<T, std_map, iterations>("std::map", v, output_files);
    test_map_type<T, sorted_vector, iterations>("vector", v, output_files);
    test_map_type<T, sorted_vector_custom_pair, iterations>("vector (custom-pair)", v, output_files);

    std::cout << std::endl;
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
    TEST(int, 8u << 1);
    TEST(int, 8u << 2);
    TEST(int, 8u << 3);
    TEST(int, 8u << 4);
    TEST(int, 8u << 5);
    TEST(int, 8u << 6);
    TEST(int, 8u << 7);
    TEST(int, 8u << 8);
    TEST(int, 8u << 9);
    TEST(int, 8u << 10);
    TEST(int, 8u << 11);
#if 0
    TEST(int, 8u << 12);
    TEST(int, 8u << 13);
    TEST(int, 8u << 14);
    TEST(int, 8u << 15);
    TEST(int, 8u << 16);
    TEST(int, 8u << 17);
    TEST(int, 8u << 18);
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n\n"
           << "struct_timings = [\n";
    }

#if 0
    TEST(largish_struct, 8u);
    TEST(largish_struct, 8u << 1);
    TEST(largish_struct, 8u << 2);
    TEST(largish_struct, 8u << 3);
    TEST(largish_struct, 8u << 4);
    TEST(largish_struct, 8u << 5);
    TEST(largish_struct, 8u << 6);
    TEST(largish_struct, 8u << 7);
    TEST(largish_struct, 8u << 8);
    TEST(largish_struct, 8u << 9);
    TEST(largish_struct, 8u << 10);
    TEST(largish_struct, 8u << 11);
#if 0
    TEST(largish_struct, 8u << 12);
    TEST(largish_struct, 8u << 13);
    TEST(largish_struct, 8u << 14);
    TEST(largish_struct, 8u << 15);
    TEST(largish_struct, 8u << 16);
    TEST(largish_struct, 8u << 17);
    TEST(largish_struct, 8u << 18);
#endif
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n";
    }

    return 0;
}
