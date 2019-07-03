#include "flat_map"

#include <string>
#include <cassert>


int main()
{
    using fmap_t = std::flat_map<std::string, int>;
    using pair_t = std::pair<std::string, int>;

    {
        std::vector<pair_t> vec = {{"key1", 1}, {"key0", 0}};
        fmap_t map1(vec);

        assert(!std::equal(
            map1.begin(),
            map1.end(),
            vec.begin(),
            vec.end(),
            [](auto lhs, auto rhs) {
                return lhs.first == rhs.first && lhs.second == rhs.second;
            }));

        fmap_t map2({pair_t{"key1", 1}, pair_t{"key0", 0}});

        assert(std::equal(
            map1.begin(),
            map1.end(),
            map2.begin(),
            map2.end(),
            [](auto lhs, auto rhs) {
                return lhs.first == rhs.first && lhs.second == rhs.second;
            }));
    }

    // TODO
}
