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

TEST(std_flat_map, ctors_iterators)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    auto pair_cmp = [](auto lhs, auto rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    };

    std::initializer_list<pair_t> init_list = {
        {"key1", 1}, {"key2", 2}, {"key0", 0}};
    fmap_t::containers const c = {{"key1", "key2", "key0"}, {1, 2, 0}};
    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key0", 0}};
    std::vector<pair_t> sorted_vec = vec;
    std::sort(sorted_vec.begin(), sorted_vec.end());

    fmap_t const map_from_c(c.keys, c.values);
    fmap_t const map_from_vec(vec);

    EXPECT_EQ(map_from_c, map_from_vec);
    EXPECT_FALSE(map_from_c != map_from_vec);

    {
        fmap_t map;
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0u);
        EXPECT_EQ(map.begin(), map.end());
    }

    {
        fmap_t map(c.keys, c.values);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        // NOTE: This also exercises:
        // template<class InputIterator>
        // flat_map(
        //     InputIterator first,
        //     InputIterator last,
        //     const key_compare & comp = key_compare());
        
        fmap_t map(vec);
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));

        EXPECT_TRUE(std::equal(
            map.cbegin(),
            map.cend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        std::flat_map<std::string, int, std::greater<std::string>> map(vec);
        EXPECT_TRUE(std::equal(
            map.rbegin(),
            map.rend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));

        EXPECT_TRUE(std::equal(
            map.crbegin(),
            map.crend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        // This breaks the ctor's contract, since c.keys are not insorted
        // order, but it's useful to verify that the resulting contents were
        // not touched.
        fmap_t map(std::sorted_unique, c.keys, c.values);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        // NOTE: This also exercises:
        // template<class InputIterator>
        // flat_map(
        //     sorted_unique_t s,
        //     InputIterator first,
        //     InputIterator last,
        //     const key_compare & comp = key_compare());
        
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, vec);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        std::flat_map<std::string, int, std::greater<std::string>> map;
        // TODO
    }

    {
        fmap_t map(init_list);
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, init_list);
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        fmap_t map;
        map = {pair_t{"key1", 1}, pair_t{"key2", 2}, pair_t{"key0", 0}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }
}
