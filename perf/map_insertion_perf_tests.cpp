#include <boost/container/flat_map.hpp>

#include <algorithm>
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
    split_map,

    num_map_impl_kinds
};

template <typename T, typename U, typename ValueType = std::pair<T, U>>
struct split_map_t
{
    using const_iterator = typename std::vector<U>::const_iterator;

    size_t size() const
    { return values.size(); }

    typename std::vector<U>::iterator lower_bound(T const & t)
    {
        auto const keys_it = std::lower_bound(keys.begin(), keys.end(), t);
        return values.begin() + (keys_it - keys.begin());
    }

    typename std::vector<U>::iterator find(T const & t)
    {
        auto const it = lower_bound(t);
        if (it != values.end() && *it == t)
            return it;
        return values.end();
    }

    U& operator[](T const & t)
    {
        auto const values_it = lower_bound(t);
        if (values_it != values.end() && *values_it == t)
            return *values_it;
        auto const keys_it = keys.begin() + (values_it - values.begin());
        keys.insert(keys_it, t);
        return *values.insert(values_it, U());
    }

    void erase(T const & t)
    {
        auto values_it = find(t);
        if (values_it != values.end()) {
            auto const keys_it = keys.begin() + (values_it - values.begin());
            keys.erase(keys_it);
            values.erase(values_it);
        }
    }

    std::vector<U> keys;
    std::vector<U> values;
};

template <typename T, typename U>
typename split_map_t<T, U>::const_iterator begin(split_map_t<T, U> const & c)
{ return c.values.begin(); }
template <typename T, typename U>
typename split_map_t<T, U>::const_iterator end(split_map_t<T, U> const & c)
{ return c.values.end(); }

template <typename KeyType, typename ValueType, map_impl_kind MapImpl>
struct map_impl
{
    using type = std::map<KeyType, ValueType>;
};

template <typename KeyType, typename ValueType>
struct map_impl<KeyType, ValueType, boost_flat_map>
{
    using type = boost::container::flat_map<KeyType, ValueType>;
};

template <typename KeyType, typename ValueType>
struct map_impl<KeyType, ValueType, split_map>
{
    using type = split_map_t<KeyType, ValueType>;
};

template <typename KeyType, typename ValueType, map_impl_kind MapImpl>
using map_impl_t = typename map_impl<KeyType, ValueType, MapImpl>::type;

template <typename KeyType>
KeyType make_key(int x)
{ return x; }

template <>
std::string make_key(int x)
{ return std::to_string(x); }

template <typename ValueType>
ValueType make_value()
{ return ValueType(); }

template <typename T>
auto value_of(T const & x)
{ return x; }

template <typename T, typename U>
auto value_of(std::pair<T, U> const & x)
{ return x.second; }

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

template <typename KeyType, typename ValueType, map_impl_kind MapImpl, int Iterations>
void test_map_type(std::string kind_name, std::vector<int> const & v, output_files_t & output_files)
{
    using dur = std::chrono::duration<double>;

    using map_t = map_impl_t<KeyType, ValueType, MapImpl>;

    std::vector<map_t> maps(Iterations);

    int const other_map_factor = 64;
    std::vector<map_t> other_maps_were_not_measuring(other_map_factor * Iterations);

    output_files.ofs[MapImpl] << "    {'size': " << v.size() << ", ";

    kind_name += ':';
    kind_name += std::string(40 - kind_name.size(), ' ');

    {
        std::vector<double> times;
        for (int i = 0, size = (int)maps.size(); i < size; ++i) {
            map_t & map = maps[i];
            double time = 0.0;
            for (auto e : v)
            {
                // allocate a bunch of nodes of the same size to fragment
                // memory.
                for (int j = 0; j < other_map_factor; ++j) {
                    map_t & other_map =
                        other_maps_were_not_measuring[other_map_factor * i + j];
                    auto const key = make_key<KeyType>(other_map_factor * e + j);
                    other_map[key] = make_value<ValueType>();
                }
                auto const key = make_key<KeyType>(e);
                auto start = std::chrono::high_resolution_clock::now();
                map[key] = make_value<ValueType>();
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
        int copy_count = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
        for (auto const & map : maps) {
            std::vector<ValueType> values(map.size());
            auto start = std::chrono::high_resolution_clock::now();
            std::transform(
                begin(map), end(map), begin(values),
                [](auto const & elem){ return value_of(elem); }
            );
            auto stop = std::chrono::high_resolution_clock::now();
            times.push_back(dur(stop - start).count() * 1000);
            for (auto x : values) {
                ++copy_count;
            }
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'iterate': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms iterate\n";
        if (copy_count == 2)
            std::cout << "  SURPRISE! copy_count=" << copy_count << "\n";
    }

    {
        std::vector<double> times;
        int key_count = 0; // To ensure the optimizer does not remove the loops below altogether, do some work.
        for (auto & map : maps) {
            auto const end_ = end(map);
            double time = 0.0;
            for (auto e : v) {
                auto const key = make_key<KeyType>(e);
                auto start = std::chrono::high_resolution_clock::now();
                auto const it = map.find(key);
                if (it != end_)
                    ++key_count;
                auto stop = std::chrono::high_resolution_clock::now();
                time += dur(stop - start).count() * 1000;
            }
            times.push_back(time);
        }
        auto const elapsed = single_elapsed_value(times);
        output_files.ofs[MapImpl] << "'find': " << elapsed << ",";
        std::cout << "  " << kind_name << elapsed << " ms find\n";
        if (key_count == 2)
            std::cout << "  SURPRISE! key_count=" << key_count << "\n";
    }

    output_files.ofs[MapImpl] << "},\n";
}

template <typename KeyType, typename ValueType>
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

    test_map_type<KeyType, ValueType, boost_flat_map, iterations>("boost flat_map", v, output_files);
    test_map_type<KeyType, ValueType, std_map, iterations>("std::map", v, output_files);
    test_map_type<KeyType, ValueType, split_map, iterations>("split_map", v, output_files);

    std::cout << std::endl;
}

#define TEST(key_t, value_t, size)                              \
    std::cout << "<" << #key_t << ", " << #value_t << ">, "     \
              << (size) << " elements:\n";                      \
    test<key_t, value_t>((size), output_files)

int main()
{
    output_files_t output_files;
    output_files.ofs[boost_flat_map].open("boost_flat_map.py");
    output_files.ofs[std_map].open("std_map.py");
    output_files.ofs[split_map].open("split_map.py");

    for (auto & of : output_files.ofs) {
        of << "int_timings = [\n";
    }

#if 1
    TEST(int, int, 8u);
    TEST(int, int, 8u << 1);
    TEST(int, int, 8u << 2);
    TEST(int, int, 8u << 3);
    TEST(int, int, 8u << 4);
    TEST(int, int, 8u << 5);
    TEST(int, int, 8u << 6);
    TEST(int, int, 8u << 7);
    TEST(int, int, 8u << 8);
    TEST(int, int, 8u << 9);
    TEST(int, int, 8u << 10);
    TEST(int, int, 8u << 11);
    TEST(int, int, 8u << 12);
#if 0
    TEST(int, int, 8u << 13);
    TEST(int, int, 8u << 14);
    TEST(int, int, 8u << 15);
#endif
#else
    TEST(int, int, 10u);
    TEST(int, int, 100u);
    TEST(int, int, 1000u);
    TEST(int, int, 10000u);
    TEST(int, int, 100000u);
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n\n"
           << "string_timings = [\n";
    }

#if 1
    TEST(std::string, std::string, 8u);
    TEST(std::string, std::string, 8u << 1);
    TEST(std::string, std::string, 8u << 2);
    TEST(std::string, std::string, 8u << 3);
    TEST(std::string, std::string, 8u << 4);
    TEST(std::string, std::string, 8u << 5);
    TEST(std::string, std::string, 8u << 6);
    TEST(std::string, std::string, 8u << 7);
    TEST(std::string, std::string, 8u << 8);
    TEST(std::string, std::string, 8u << 9);
    TEST(std::string, std::string, 8u << 10);
    TEST(std::string, std::string, 8u << 11);
    TEST(std::string, std::string, 8u << 12);
#if 0
    TEST(std::string, std::string, 8u << 13);
    TEST(std::string, std::string, 8u << 14);
    TEST(std::string, std::string, 8u << 15);
#endif
#else
    TEST(std::string, std::string, 10u);
    TEST(std::string, std::string, 100u);
    TEST(std::string, std::string, 1000u);
    TEST(std::string, std::string, 10000u);
    TEST(std::string, std::string, 100000u);
#endif

    for (auto & of : output_files.ofs) {
        of << "]\n";
    }

    return 0;
}
