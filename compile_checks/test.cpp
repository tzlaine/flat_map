#include <utility>
#include <functional>
#include <vector>
#include <type_traits>

namespace std {

// TODO: Remove this once there is a multimap_synopsis.hpp.
template <class Key, class T, class Compare = less<Key>,
          class KeyContainer = vector<Key>,
          class MappedContainer = vector<T>>
class flat_multimap
{};

template <class Alloc>
using alloc_key_t = typename Alloc::value_type::type; // exposition only

template <class Alloc>
using alloc_val_t = typename Alloc::value_type::type; // exposition only

template<class InputIterator>
using iter_key_t = remove_const_t<
    typename iterator_traits<InputIterator>::value_type::first_type>; // exposition only

template<class InputIterator>
using iter_val_t = typename iterator_traits<InputIterator>::value_type::second_type; // exposition only

template<class InputIterator>
using iter_to_alloc_t = pair<add_const_t<typename iterator_traits<InputIterator>::value_type::first_type>,
                             typename iterator_traits<InputIterator>::value_type::second_type>; // exposition only

}

#include "map_synopsis.hpp"

#define USE_BOOST_STATIC_VECTOR 0
#if USE_BOOST_STATIC_VECTOR
#include <boost/container/static_vector.hpp>
#endif

#define DEFINE_SOME_MEMBERS 0 // Use requires a member "Container c;" to be added to the synopsis.
#if DEFINE_SOME_MEMBERS
namespace std {

    template <class Key, class T, class Compare, class Container>
    bool flat_map<Key, T, Compare, Container>::empty() const noexcept
    {
        return c.empty();
    }

    template <class Key, class T, class Compare, class Container>
    typename flat_map<Key, T, Compare, Container>::size_type
    flat_map<Key, T, Compare, Container>::size() const noexcept
    {
        return c.size();
    }

    template <class Key, class T, class Compare, class Container>
    typename flat_map<Key, T, Compare, Container>::size_type
    flat_map<Key, T, Compare, Container>::max_size() const noexcept
    {
        return c.max_size();
    }

}
#endif


int main()
{
#if USE_BOOST_STATIC_VECTOR
    using container_t = boost::container::static_vector<std::pair<char const *, double>, 10u>;
    using fm_t = std::flat_map<char const *, double, std::less<char const *>, container_t>;
#else
    using container_t = std::vector<std::pair<char const *, double> >;
    using fm_t = std::flat_map<char const *, double>;
#endif

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
        fm_t fm_from_container_copy_and_alloc(container, alloc, alloc);
        fm_t fm_from_container_move(std::move(container));
        fm_t fm_from_container_move_and_alloc(std::move(container), alloc, alloc);

        fm_t fm_from_comp(comp);
        fm_t fm_from_comp_and_alloc(comp, alloc);
        fm_t fm_from_range(container.begin(), container.end());
        fm_t fm_from_range_and_comp(container.begin(), container.end(), comp);
        fm_t fm_from_range_and_comp_and_alloc(container.begin(), container.end(), comp, alloc, alloc);
        fm_t fm_from_range_and_alloc(container.begin(), container.end(), alloc, alloc);

        fm_t fm_from_ordered_range(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end());
        fm_t fm_from_ordered_range_and_comp(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), comp);
        fm_t fm_from_ordered_range_and_comp_and_alloc(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), comp, alloc, alloc);
        fm_t fm_from_ordered_range_and_alloc(
            std::ordered_unique_sequence_tag{},
            container.begin(), container.end(), alloc, alloc);

        fm_t fm_from_other_fm_copy_and_alloc(other_fm, alloc);
        fm_t fm_from_other_fm_move_and_alloc(std::move(other_fm), alloc);

        fm_t fm_from_initializer({ {"foo", 1.0}, {"bar", 42.0} });
        fm_t fm_from_initializer_and_comp({ {"foo", 1.0}, {"bar", 42.0} }, comp);
        fm_t fm_from_initializer_and_comp_and_alloc({ {"foo", 1.0}, {"bar", 42.0} }, comp, alloc, alloc);
        fm_t fm_from_initializer_and_alloc({ {"foo", 1.0}, {"bar", 42.0} }, alloc, alloc);
        fm_from_initializer = { {"foo", 1.0}, {"bar", 42.0} };
    }

    // deduction:
    {
        auto p = std::pair{"foo", 1.0};
        std::flat_map m = { std::pair{"foo", 1.0}, std::pair{"bar", 42.0} };
    }

    // modifiers:
    {
        fm_t fm;
        fm.swap(other_fm);

        std::swap(fm, other_fm);
    }

    return 0;
}
