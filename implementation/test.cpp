#include "flat_map"

#include <gtest/gtest.h>

#include <string>

// Test instantiations.
template class std::flat_map<std::string, int>;

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

    fmap_t const map_from_const_map = map_from_c;
    fmap_t const map_from_map_rvalue = fmap_t(map_from_c);

    EXPECT_EQ(map_from_const_map, map_from_vec);
    EXPECT_FALSE(map_from_map_rvalue != map_from_vec);

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
        fmap_t map(vec.begin(), vec.end());
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
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, vec.begin(), vec.end());
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, vec);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        std::flat_map<std::string, int, std::greater<std::string>> map;
        map.insert(vec.begin(), vec.end());
        EXPECT_TRUE(std::equal(
            map.rbegin(),
            map.rend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
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

TEST(std_flat_map, ctors_allocators_iterators)
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

    auto allocator = c.keys.get_allocator();

    fmap_t const map_from_c(c.keys, c.values, allocator);
    fmap_t const map_from_vec(vec, allocator);

    EXPECT_EQ(map_from_c, map_from_vec);
    EXPECT_FALSE(map_from_c != map_from_vec);


    fmap_t const map_from_const_map(map_from_c, allocator);
    fmap_t const map_from_map_rvalue(fmap_t(map_from_c), allocator);

    EXPECT_EQ(map_from_const_map, map_from_vec);
    EXPECT_FALSE(map_from_map_rvalue != map_from_vec);


    {
        fmap_t map(allocator);
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0u);
        EXPECT_EQ(map.begin(), map.end());
    }

    {
        fmap_t map(c.keys, c.values, allocator);
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
        fmap_t map(vec.begin(), vec.end(), allocator);
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
        fmap_t map(vec, allocator);
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
        std::flat_map<std::string, int, std::greater<std::string>> map(vec, allocator);
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
        fmap_t map(std::sorted_unique, c.keys, c.values, allocator);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, vec.begin(), vec.end(), allocator);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, vec, allocator);
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        std::flat_map<std::string, int, std::greater<std::string>> map(
            allocator);
        map.insert(vec.begin(), vec.end());
        EXPECT_TRUE(std::equal(
            map.rbegin(),
            map.rend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        fmap_t map(init_list, allocator);
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        // Broken contract, as above.
        fmap_t map(std::sorted_unique, init_list, allocator);
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        fmap_t map(allocator);
        map = {pair_t{"key1", 1}, pair_t{"key2", 2}, pair_t{"key0", 0}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }
}

TEST(std_flat_map, index_at)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key-1", -1}};
    std::vector<pair_t> sorted_vec = vec;
    std::sort(sorted_vec.begin(), sorted_vec.end());

    {
        fmap_t map(vec);
        std::string key_1("key1");
        EXPECT_EQ(map["key-1"], -1);
        EXPECT_EQ(map["key2"], 2);
        EXPECT_EQ(map[key_1], 1);
        EXPECT_EQ(map["foo"], 0);

        map["foo"] = 8;

        EXPECT_EQ(map.at("key-1"), -1);
        EXPECT_EQ(map.at("key2"), 2);
        EXPECT_EQ(map.at("key1"), 1);
        EXPECT_EQ(map.at("foo"), 8);
        EXPECT_THROW(map.at("bar"), std::out_of_range);

        fmap_t const & cmap = map;
        EXPECT_THROW(cmap.at("bar"), std::out_of_range);
    }
}

TEST(std_flat_map, emplace_insert)
{
    using fmap_t = std::flat_map<std::string, int>;
    using rev_fmap_t =
        std::flat_map<std::string, int, std::greater<std::string>>;
    using pair_t = std::pair<std::string, int>;

    auto pair_cmp = [](auto lhs, auto rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    };

    fmap_t::containers const c = {{"key1", "key2", "key0"}, {1, 2, 0}};
    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key0", 0}};
    std::vector<pair_t> sorted_vec = vec;
    std::sort(sorted_vec.begin(), sorted_vec.end());

    pair_t const pair1("q", 3);
    pair_t const pair2("w", 4);

    using foreign_pair_t = std::pair<char const *, short>;
    foreign_pair_t foreign_pair("e", 5);

    {
        // NOTE: These calls excercise all emplaces and inserts before the range
        // inserts.

        fmap_t map;
        map.insert(pair1);
        map.insert(pair_t("r", 6));
        map.insert(map.begin(), pair2);
        map.insert(map.begin(), pair_t("t", 7));
        map.insert(foreign_pair);
        map.insert(foreign_pair_t("y", 8));

        std::vector<pair_t> const qwerty_vec = {
            {"e", 5}, {"q", 3}, {"r", 6}, {"t", 7}, {"w", 4}, {"y", 8}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            qwerty_vec.begin(),
            qwerty_vec.end(),
            pair_cmp));
    }

    // NOTE: Already exercised by ctor test:
    // template<class InputIterator>
    // void insert(InputIterator first, InputIterator last);
    // template<class InputIterator>
    // void insert(sorted_unique_t, InputIterator first, InputIterator last);

    {
        fmap_t map;
        map.insert({pair_t{"key1", 1}, pair_t{"key2", 2}, pair_t{"key0", 0}});
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
        rev_fmap_t map;
        map.insert({pair_t{"key1", 1}, pair_t{"key2", 2}, pair_t{"key0", 0}});
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.rbegin(),
            map.rend(),
            sorted_vec.begin(),
            sorted_vec.end(),
            pair_cmp));
    }

    {
        // This breaks the ctor's contract, since c.keys are not insorted
        // order, but it's useful to verify that the resulting contents were
        // not touched.
        fmap_t map;
        map.insert(
            std::sorted_unique,
            {pair_t{"key1", 1}, pair_t{"key2", 2}, pair_t{"key0", 0}});
        EXPECT_EQ(map.size(), 3u);
        EXPECT_FALSE(map.empty());
        EXPECT_TRUE(std::equal(
            map.begin(), map.end(), vec.begin(), vec.end(), pair_cmp));
    }

    {
        // NOTE: These calls excercise all the try_emplaces.

        fmap_t map;
        map.try_emplace(map.begin(), pair1.first, pair1.second);
        map.try_emplace(map.begin(), std::string("lucky"), 13);

        std::vector<pair_t> const local_vec = {{"lucky", 13}, {"q", 3}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            local_vec.begin(),
            local_vec.end(),
            pair_cmp));
    }

    {
        // NOTE: These calls excercise all the try_emplaces.

        fmap_t map;
        map.insert_or_assign(map.begin(), pair1.first, pair1.second);
        map.insert_or_assign(
            map.begin(), std::string(pair1.first), pair1.second);

        std::vector<pair_t> const local_vec = {{"q", 3}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            local_vec.begin(),
            local_vec.end(),
            pair_cmp));
    }
}

TEST(std_flat_map, extract_replace)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    fmap_t::containers const sorted_c = {{"key0", "key1", "key2"}, {0, 1, 2}};
    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key0", 0}};

    fmap_t map1(vec);
    fmap_t const map1_copy = map1;

    fmap_t::containers extracted_c = std::move(map1).extract();
    EXPECT_EQ(extracted_c.keys, sorted_c.keys);
    EXPECT_EQ(extracted_c.values, sorted_c.values);
    EXPECT_TRUE(map1.empty());

    fmap_t map2;
    map2.replace(std::move(extracted_c.keys), std::move(extracted_c.values));
    EXPECT_EQ(map2, map1_copy);
}

TEST(std_flat_map, erase_swap)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    auto pair_cmp = [](auto lhs, auto rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    };

    std::vector<pair_t> const vec = {{"key0", 0}, {"key1", 1}, {"key2", 2}};

    {
        fmap_t map(vec);
        map.erase(map.begin());
        std::vector<pair_t> const local_vec = {{"key1", 1}, {"key2", 2}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            local_vec.begin(),
            local_vec.end(),
            pair_cmp));
    }

    {
        fmap_t map(vec);
        map.erase(map.cbegin());
        std::vector<pair_t> const local_vec = {{"key1", 1}, {"key2", 2}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            local_vec.begin(),
            local_vec.end(),
            pair_cmp));
    }

    {
        fmap_t map(vec);
        map.erase("key0");
        std::vector<pair_t> const local_vec = {{"key1", 1}, {"key2", 2}};
        EXPECT_TRUE(std::equal(
            map.begin(),
            map.end(),
            local_vec.begin(),
            local_vec.end(),
            pair_cmp));
    }

    {
        fmap_t map(vec);
        map.erase(map.begin(), map.end());
        EXPECT_EQ(map.begin(), map.end());
    }

    {
        fmap_t const orig_map(vec);
        fmap_t const orig_empty_map;

        auto map1 = orig_map;
        auto map2 = orig_empty_map;

        std::swap(map1, map2);

        EXPECT_EQ(map1, orig_empty_map);
        EXPECT_EQ(map2, orig_map);

        std::swap(map1, map2);

        EXPECT_EQ(map1, orig_map);
        EXPECT_EQ(map2, orig_empty_map);
    }
}

TEST(std_flat_map, count_contains)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key0", 0}};

    fmap_t const map(vec);
    EXPECT_EQ(map.count("key0"), 1u);
    EXPECT_EQ(map.count("key10"), 0u);
    EXPECT_TRUE(map.count("key0"));
    EXPECT_FALSE(map.count("key10"));
}

TEST(std_flat_map, equal_range)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    std::vector<pair_t> const vec = {{"key1", 1}, {"key2", 2}, {"key0", 0}};

    fmap_t const map(vec);
    auto const eq_range = map.equal_range("key0");
    EXPECT_EQ(eq_range.first, map.begin());
    EXPECT_EQ(eq_range.second, map.begin() + 1);
    auto const empty_eq_range = map.equal_range("");
    EXPECT_EQ(empty_eq_range.first, empty_eq_range.second);
}

TEST(std_flat_map, comparisons)
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    std::vector<pair_t> const vec = {{"key0", 0}, {"key1", 1}, {"key2", 2}};

    fmap_t const map_123(vec);
    fmap_t const map_12(vec.begin(), vec.begin() + 2);

    EXPECT_EQ(map_123, map_123);
    EXPECT_NE(map_123, map_12);

    EXPECT_LT(map_12, map_123);
    EXPECT_LE(map_12, map_123);
    EXPECT_LE(map_123, map_123);
    EXPECT_GE(map_123, map_123);
    EXPECT_GT(map_123, map_12);
}
