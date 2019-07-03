#include "flat_map"

#include <gtest/gtest.h>

#include <string>


TEST(std_flat_map, ctors)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    auto pair_cmp = [](auto lhs, auto rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    };

    {
        std::vector<pair_t> const input_vec = {{"key1", 1}, {"key0", 0}};

        std::vector<pair_t> sorted_vec = input_vec;
        std::sort(sorted_vec.begin(), sorted_vec.end());

        fmap_t map1(input_vec);

        EXPECT_FALSE(std::equal(
            map1.begin(),
            map1.end(),
            input_vec.begin(),
            input_vec.end(),
            pair_cmp));
        EXPECT_TRUE(std::equal(
            map1.begin(),
            map1.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));

        fmap_t map2({pair_t{"key1", 1}, pair_t{"key0", 0}});

        EXPECT_TRUE(std::equal(
            map1.begin(), map1.end(), map2.begin(), map2.end(), pair_cmp));
    }
}
