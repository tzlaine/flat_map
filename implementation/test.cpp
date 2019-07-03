#include "flat_map"

#include <gtest/gtest.h>

#include <string>


TEST(std_flat_map, iterator)
{
    using fmap_t = std::flat_map<std::string, int>;

    {
        fmap_t map;
        fmap_t::iterator mutable_first = map.begin();
        fmap_t::const_iterator const_first = mutable_first;

        EXPECT_EQ(mutable_first, const_first);
    }

    {
        fmap_t::containers c = {{"key0", "key1", "key2"}, {0, 1, 2}};

        fmap_t::iterator const first(c.keys.begin(), c.values.begin());
        fmap_t::iterator const last(c.keys.end(), c.values.end());

        EXPECT_EQ(first + 3, last);
        EXPECT_EQ(first, last - 3);

        EXPECT_EQ(first[1].first, "key1");
        EXPECT_EQ(last[-3].second, 0);

        EXPECT_EQ((*(first + 1)).first, "key1");
        EXPECT_EQ((*(last - 3)).second, 0);

        EXPECT_EQ((first + 1)->first, "key1");
        EXPECT_EQ((last - 3)->second, 0);

        EXPECT_EQ(first - last, -3);
        EXPECT_EQ(last - first, 3);

        {
            auto first_copy = first;
            auto last_copy = last;

            first_copy += 3;
            last_copy -= 3;

            EXPECT_EQ(first_copy, last);
            EXPECT_EQ(last_copy, first);
        }

        {
            auto first_copy = first;
            auto last_copy = last;

            EXPECT_EQ(first_copy++, first);
            EXPECT_EQ(first_copy++, first + 1);
            EXPECT_EQ(first_copy++, first + 2);
            EXPECT_EQ(first_copy, last);

            EXPECT_EQ(last_copy--, last);
            EXPECT_EQ(last_copy--, last - 1);
            EXPECT_EQ(last_copy--, last - 2);
            EXPECT_EQ(last_copy, first);
        }

        {
            auto first_copy = first;
            auto last_copy = last;

            EXPECT_EQ(++first_copy, first + 1);
            EXPECT_EQ(++first_copy, first + 2);
            EXPECT_EQ(++first_copy, last);
            EXPECT_EQ(first_copy, last);

            EXPECT_EQ(--last_copy, last - 1);
            EXPECT_EQ(--last_copy, last - 2);
            EXPECT_EQ(--last_copy, first);
            EXPECT_EQ(last_copy, first);
        }

        EXPECT_EQ(first, first);
        EXPECT_NE(first, last);

        EXPECT_LT(first, last);
        EXPECT_LE(first, last);
        EXPECT_LE(first, first);
        EXPECT_GE(last, first);
        EXPECT_GE(first, first);
        EXPECT_GT(last, first);
    }

    {
        fmap_t::containers c = {{"key0", "key1", "key2"}, {0, 1, 2}};

        fmap_t::reverse_iterator const first(c.keys.rbegin(), c.values.rbegin());
        fmap_t::reverse_iterator const last(c.keys.rend(), c.values.rend());

        EXPECT_EQ(first + 3, last);
        EXPECT_EQ(first, last - 3);

        EXPECT_EQ(first[1].first, "key1");
        EXPECT_EQ(last[-3].second, 2);

        EXPECT_EQ((*(first + 1)).first, "key1");
        EXPECT_EQ((*(last - 3)).second, 2);

        EXPECT_EQ((first + 1)->first, "key1");
        EXPECT_EQ((last - 3)->second, 2);

        EXPECT_EQ(first - last, -3);
        EXPECT_EQ(last - first, 3);

        {
            auto first_copy = first;
            auto last_copy = last;

            first_copy += 3;
            last_copy -= 3;

            EXPECT_EQ(first_copy, last);
            EXPECT_EQ(last_copy, first);
        }

        {
            auto first_copy = first;
            auto last_copy = last;

            EXPECT_EQ(first_copy++, first);
            EXPECT_EQ(first_copy++, first + 1);
            EXPECT_EQ(first_copy++, first + 2);
            EXPECT_EQ(first_copy, last);

            EXPECT_EQ(last_copy--, last);
            EXPECT_EQ(last_copy--, last - 1);
            EXPECT_EQ(last_copy--, last - 2);
            EXPECT_EQ(last_copy, first);
        }

        {
            auto first_copy = first;
            auto last_copy = last;

            EXPECT_EQ(++first_copy, first + 1);
            EXPECT_EQ(++first_copy, first + 2);
            EXPECT_EQ(++first_copy, last);
            EXPECT_EQ(first_copy, last);

            EXPECT_EQ(--last_copy, last - 1);
            EXPECT_EQ(--last_copy, last - 2);
            EXPECT_EQ(--last_copy, first);
            EXPECT_EQ(last_copy, first);
        }

        EXPECT_EQ(first, first);
        EXPECT_NE(first, last);

        EXPECT_LT(first, last);
        EXPECT_LE(first, last);
        EXPECT_LE(first, first);
        EXPECT_GE(last, first);
        EXPECT_GE(first, first);
        EXPECT_GT(last, first);
    }
}

TEST(std_flat_map, ctors)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    auto pair_cmp = [](auto lhs, auto rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    };

    {
        fmap_t map;
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.begin(), map.end());
    }

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
