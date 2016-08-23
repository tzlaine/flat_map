#include <utility>
#include <functional>
#include <vector>

#if __cplusplus <= 201402L
namespace std {
    template<class T>
    using default_order_t = std::less<T>;
}
#endif

#include "map_synopsis.hpp"


int main()
{
    using container_t = std::vector<std::pair<char const * const, double> >;
    using fm_t = std::flat_map<char const *, double>;

    static_assert(std::is_same<container_t, fm_t::container_type>::value, "");

    fm_t other_fm;
    container_t container;
    auto const & alloc = container.get_allocator();
    fm_t::key_compare comp;

    // construct/copy/destroy:
    {
        // default
        fm_t fm_default;

        // rule of five
        fm_default.~flat_map();
        fm_t fm_from_copy(other_fm);
        fm_from_copy = other_fm;
        fm_t fm_from_move(std::move(other_fm));
        fm_from_move = std::move(other_fm);

        fm_t fm_from_container_copy(container);
        fm_t fm_from_container_copy_and_alloc(container, alloc);
        fm_t fm_from_container_move(std::move(container));
        fm_t fm_from_container_move_and_alloc(std::move(container), alloc);

        fm_t fm_from_comp(comp);
        fm_t fm_from_comp_and_alloc(comp, alloc);
        fm_t fm_from_range(container.begin(), container.end());
        fm_t fm_from_range_and_comp(container.begin(), container.end(), comp);
        fm_t fm_from_range_and_comp_and_alloc(container.begin(), container.end(), comp, alloc);
        fm_t fm_from_range_and_alloc(container.begin(), container.end(), alloc);

        fm_t fm_from_ordered_range(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end());
        fm_t fm_from_ordered_range_and_comp(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), comp);
        fm_t fm_from_ordered_range_and_comp_and_alloc(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), comp, alloc);
        fm_t fm_from_ordered_range_and_alloc(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), alloc);

        fm_t fm_from_other_fm_copy_and_alloc(other_fm, alloc);
        fm_t fm_from_other_fm_move_and_alloc(std::move(other_fm), alloc);

        fm_t fm_from_initializer({ {"foo", 1.0}, {"bar", 42.0} });
        fm_t fm_from_initializer_and_comp({ {"foo", 1.0}, {"bar", 42.0} }, comp);
        fm_t fm_from_initializer_and_comp_and_alloc({ {"foo", 1.0}, {"bar", 42.0} }, comp, alloc);
        fm_t fm_from_initializer_and_alloc({ {"foo", 1.0}, {"bar", 42.0} }, alloc);
        fm_from_initializer = { {"foo", 1.0}, {"bar", 42.0} };
    }

    {
        fm_t fm;
        fm.swap(other_fm);

        std::swap(fm, other_fm);
    }

    return 0;
}
