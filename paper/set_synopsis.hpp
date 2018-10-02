namespace std {

struct sorted_t { unspecified };
struct sorted_unique_t { unspecified };
inline constexpr sorted_t sorted { unspecified };
inline constexpr sorted_unique_t sorted_unique { unspecified };

template <class Key, class Compare = less<Key>, class Container = vector<Key>>
class flat_set {
public:
    // types:
    using key_type                  = Key;
    using key_compare               = Compare;
    using value_type                = Key;
    using value_compare             = Compare;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using size_type                 = std::size_t;
    using difference_type           = std::ptrdiff_t;
    using iterator                  = implementation-defined;
    using const_iterator            = implementation-defined;
    using reverse_iterator          = implementation-defined;
    using const_reverse_iterator    = implementation-defined;
    using container_type            = Container;

    // construct/copy/destroy:
    flat_set();

    flat_set(container_type);
    template <class Alloc>
      flat_set(container_type, const Alloc&);

    flat_set(sorted_unique_t, container_type);
    template <class Alloc>
      flat_set(sorted_unique_t, container_type, const Alloc&);

    explicit flat_set(const key_compare& comp);
    template <class Alloc>
      flat_set(const key_compare& comp, const Alloc&);
    template <class Alloc>
      flat_set(const Alloc&);

    template <class InputIterator>
      flat_set(InputIterator first, InputIterator last,
               const key_compare& comp = key_compare());
    template <class InputIterator, class Alloc>
      flat_set(InputIterator first, InputIterator last,
               const key_compare& comp, const Alloc&);
    template <class InputIterator, class Alloc>
      flat_set(InputIterator first, InputIterator last,
               const Alloc& a)
        : flat_set(first, last, key_compare(), a) { }

    template <class InputIterator>
      flat_set(sorted_unique_t, InputIterator first, InputIterator last,
               const key_compare& comp = key_compare());
    template <class InputIterator, class Alloc>
      flat_set(sorted_unique_t, InputIterator first, InputIterator last,
               const key_compare& comp, const Alloc&);
    template <class InputIterator, class Alloc>
      flat_set(sorted_unique_t t, InputIterator first, InputIterator last,
               const Alloc& a)
        : flat_set(t, first, last, key_compare(), a) { }

    template <class Alloc>
      flat_set(flat_set, const Alloc&);

    flat_set(initializer_list<key_type>, const key_compare& = key_compare());
    template <class Alloc>
      flat_set(initializer_list<key_type>,
               const key_compare&, const Alloc&);
    template <class Alloc>
      flat_set(initializer_list<key_type> il, const Alloc& a)
        : flat_set(il, key_compare(), a) { }

    flat_set(sorted_unique_t, initializer_list<key_type>,
             const key_compare& = key_compare());
    template <class Alloc>
      flat_set(sorted_unique_t, initializer_list<key_type>,
               const key_compare&, const Alloc&);
    template <class Alloc>
      flat_set(sorted_unique_t t, initializer_list<key_type> il,
               const Alloc& a)
        : flat_set(t, il, key_compare(), a) { }

    flat_set& operator=(initializer_list<key_type>);

    // iterators:
    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    iterator                end() noexcept;
    const_iterator          end() const noexcept;

    reverse_iterator        rbegin() noexcept;
    const_reverse_iterator  rbegin() const noexcept;
    reverse_iterator        rend() noexcept;
    const_reverse_iterator  rend() const noexcept;

    const_iterator          cbegin() const noexcept;
    const_iterator          cend() const noexcept;
    const_reverse_iterator  crbegin() const noexcept;
    const_reverse_iterator  crend() const noexcept;

    // size:
    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // modifiers:
    template <class... Args> pair<iterator, bool> emplace(Args&&... args);
    template <class... Args>
      iterator emplace_hint(const_iterator position, Args&&... args);
    pair<iterator, bool> insert(const value_type& x);
    pair<iterator, bool> insert(value_type&& x);
    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, value_type&& x);
    template <class InputIterator>
      void insert(InputIterator first, InputIterator last);
    template <class InputIterator>
      void insert(sorted_unique_t, InputIterator first, InputIterator last);
    void insert(initializer_list<key_type>);
    void insert(sorted_unique_t, initializer_list<key_type>);

    container_type extract() &&;
    void replace(container_type&&);

    iterator erase(iterator position);
    iterator erase(const_iterator position);
    size_type erase(const key_type& x);
    iterator erase(const_iterator first, const_iterator last);

    void swap(flat_set& fs) noexcept(
        is_nothrow_swappable_v<container_type> && is_nothrow_swappable_v<key_compare>
    );
    void clear() noexcept;

    template<class C2>
      void merge(flat_set<key_type, C2, container_type>& source);
    template<class C2>
      void merge(flat_set<key_type, C2, container_type>&& source);
    template<class C2>
      void merge(flat_multiset<key_type, C2, container_type>& source);
    template<class C2>
      void merge(flat_multiset<key_type, C2, container_type>&& source);

    // observers:
    key_compare key_comp() const;
    value_compare value_comp() const;

    // set operations:
    iterator       find(const key_type& x);
    const_iterator find(const key_type& x) const;
    template <class K> iterator       find(const K& x);
    template <class K> const_iterator find(const K& x) const;

    size_type count(const key_type& x) const;
    template <class K> size_type count(const K& x) const;

    bool contains(const key_type& x) const;
    template <class K> bool contains(const K& x) const;

    iterator       lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    template <class K> iterator lower_bound(const K& x);
    template <class K> const_iterator lower_bound(const K& x) const;

    iterator       upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    template <class K> iterator       upper_bound(const K& x);
    template <class K> const_iterator upper_bound(const K& x) const;

    pair<iterator, iterator>               equal_range(const key_type& x);
    pair<const_iterator, const_iterator>   equal_range(const key_type& x) const;
    template <class K>
      pair<iterator, iterator>             equal_range(const K& x);
    template <class K>
      pair<const_iterator, const_iterator> equal_range(const K& x) const;
};

template <class Container>
  using cont-value-type = typename Container::value_type; // exposition only

template <class Container>
  flat_set(Container)
    -> flat_set<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container>
  flat_set(Container)
    -> flat_set<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container, class Alloc>
  flat_set(Container, Alloc)
    -> flat_set<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container, class Alloc>
  flat_set(Container, Alloc)
    -> flat_set<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container>
  flat_set(sorted_unique_t, Container)
    -> flat_set<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container>
  flat_set(sorted_unique_t, Container)
    -> flat_set<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container, class Alloc>
  flat_set(sorted_unique_t, Container, Alloc)
    -> flat_set<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container, class Alloc>
  flat_set(sorted_unique_t, Container, Alloc)
    -> flat_set<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
  flat_set(InputIterator, InputIterator, Compare = Compare())
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
  flat_set(InputIterator, InputIterator, Compare, Alloc)
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>, Compare,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Alloc>
  flat_set(InputIterator, InputIterator, Alloc)
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
  flat_set(sorted_unique_t, InputIterator, InputIterator, Compare = Compare())
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
  flat_set(sorted_unique_t, InputIterator, InputIterator, Compare, Alloc)
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>, Compare,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Alloc>
  flat_set(sorted_unique_t, InputIterator, InputIterator, Alloc)
    -> flat_set<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class Key, class Compare = less<Key>>
  flat_set(initializer_list<key_type>, Compare = Compare())
    -> flat_set<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Compare, class Alloc>
  flat_set(initializer_list<key_type>, Compare, Alloc)
    -> flat_set<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Alloc>
  flat_set(initializer_list<key_type>, Alloc)
    -> flat_set<Key, less<Key>, vector<Key>, vector<T>>;

template<class Key, class Compare = less<Key>>
flat_set(sorted_unique_t, initializer_list<key_type>, Compare = Compare())
    -> flat_set<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Compare, class Alloc>
  flat_set(sorted_unique_t, initializer_list<key_type>, Compare, Alloc)
    -> flat_set<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Alloc>
  flat_set(sorted_unique_t, initializer_list<key_type>, Alloc)
    -> flat_set<Key, less<Key>, vector<Key>, vector<T>>;

// the comparisons below are for exposition only
template <class Key, class Compare, class Container>
  bool operator==(const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator< (const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator!=(const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator> (const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator>=(const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator<=(const flat_set<Key, Compare, Container>& x,
                  const flat_set<Key, Compare, Container>& y);

// specialized algorithms:
template <class Key, class Compare, class Container>
  void swap(flat_set<Key, Compare, Container>& x,
            flat_set<Key, Compare, Container>& y)
    noexcept(noexcept(x.swap(y)));



template <class Key, class Compare = less<Key>, class Container = vector<Key>>
class flat_multiset {
public:
    // types:
    using key_type                  = Key;
    using key_compare               = Compare;
    using value_type                = Key;
    using value_compare             = Compare;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using size_type                 = std::size_t;
    using difference_type           = std::ptrdiff_t;
    using iterator                  = implementation-defined;
    using const_iterator            = implementation-defined;
    using reverse_iterator          = implementation-defined;
    using const_reverse_iterator    = implementation-defined;
    using container_type            = Container;

    // construct/copy/destroy:
    flat_multiset();

    flat_multiset(container_type);
    template <class Alloc>
      flat_multiset(container_type, const Alloc&);

    flat_multiset(sorted_t, container_type);
    template <class Alloc>
      flat_multiset(sorted_t, container_type, const Alloc&);

    explicit flat_multiset(const key_compare& comp);
    template <class Alloc>
      flat_multiset(const key_compare& comp, const Alloc&);
    template <class Alloc>
      flat_multiset(const Alloc&);

    template <class InputIterator>
      flat_multiset(InputIterator first, InputIterator last,
               const key_compare& comp = key_compare());
    template <class InputIterator, class Alloc>
      flat_multiset(InputIterator first, InputIterator last,
               const key_compare& comp, const Alloc&);
    template <class InputIterator, class Alloc>
      flat_multiset(InputIterator first, InputIterator last,
               const Alloc& a)
        : flat_multiset(first, last, key_compare(), a) { }

    template <class InputIterator>
      flat_multiset(sorted_t, InputIterator first, InputIterator last,
               const key_compare& comp = key_compare());
    template <class InputIterator, class Alloc>
      flat_multiset(sorted_t, InputIterator first, InputIterator last,
               const key_compare& comp, const Alloc&);
    template <class InputIterator, class Alloc>
      flat_multiset(sorted_t t, InputIterator first, InputIterator last,
               const Alloc& a)
        : flat_multiset(t, first, last, key_compare(), a) { }

    template <class Alloc>
      flat_multiset(flat_multiset, const Alloc&);

    flat_multiset(initializer_list<key_type>, const key_compare& = key_compare());
    template <class Alloc>
      flat_multiset(initializer_list<key_type>,
               const key_compare&, const Alloc&);
    template <class Alloc>
      flat_multiset(initializer_list<key_type> il, const Alloc& a)
        : flat_multiset(il, key_compare(), a) { }

    flat_multiset(sorted_t, initializer_list<key_type>,
             const key_compare& = key_compare());
    template <class Alloc>
      flat_multiset(sorted_t, initializer_list<key_type>,
               const key_compare&, const Alloc&);
    template <class Alloc>
      flat_multiset(sorted_t t, initializer_list<key_type> il,
               const Alloc& a)
        : flat_multiset(t, il, key_compare(), a) { }

    flat_multiset& operator=(initializer_list<key_type>);

    // iterators:
    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    iterator                end() noexcept;
    const_iterator          end() const noexcept;

    reverse_iterator        rbegin() noexcept;
    const_reverse_iterator  rbegin() const noexcept;
    reverse_iterator        rend() noexcept;
    const_reverse_iterator  rend() const noexcept;

    const_iterator          cbegin() const noexcept;
    const_iterator          cend() const noexcept;
    const_reverse_iterator  crbegin() const noexcept;
    const_reverse_iterator  crend() const noexcept;

    // size:
    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // modifiers:
    template <class... Args> iterator emplace(Args&&... args);
    template <class... Args>
      iterator emplace_hint(const_iterator position, Args&&... args);
    iterator insert(const value_type& x);
    iterator insert(value_type&& x);
    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, value_type&& x);
    template <class InputIterator>
      void insert(InputIterator first, InputIterator last);
    template <class InputIterator>
      void insert(sorted_t, InputIterator first, InputIterator last);
    void insert(initializer_list<key_type>);
    void insert(sorted_t, initializer_list<key_type>);

    container_type extract() &&;
    void replace(container_type&&);

    iterator erase(iterator position);
    iterator erase(const_iterator position);
    size_type erase(const key_type& x);
    iterator erase(const_iterator first, const_iterator last);

    void swap(flat_multiset& fms) noexcept(
        is_nothrow_swappable_v<container_type> && is_nothrow_swappable_v<key_compare>
    );
    void clear() noexcept;

    template<class C2>
      void merge(flat_set<key_type, C2, container_type>& source);
    template<class C2>
      void merge(flat_set<key_type, C2, container_type>&& source);
    template<class C2>
      void merge(flat_multiset<key_type, C2, container_type>& source);
    template<class C2>
      void merge(flat_multiset<key_type, C2, container_type>&& source);

    // observers:
    key_compare key_comp() const;
    value_compare value_comp() const;

    // set operations:
    iterator       find(const key_type& x);
    const_iterator find(const key_type& x) const;
    template <class K> iterator       find(const K& x);
    template <class K> const_iterator find(const K& x) const;

    size_type count(const key_type& x) const;
    template <class K> size_type count(const K& x) const;

    bool contains(const key_type& x) const;
    template <class K> bool contains(const K& x) const;

    iterator       lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    template <class K> iterator lower_bound(const K& x);
    template <class K> const_iterator lower_bound(const K& x) const;

    iterator       upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    template <class K> iterator       upper_bound(const K& x);
    template <class K> const_iterator upper_bound(const K& x) const;

    pair<iterator, iterator>               equal_range(const key_type& x);
    pair<const_iterator, const_iterator>   equal_range(const key_type& x) const;
    template <class K>
      pair<iterator, iterator>             equal_range(const K& x);
    template <class K>
      pair<const_iterator, const_iterator> equal_range(const K& x) const;
};

template <class Container>
  using cont-value-type = typename Container::value_type; // exposition only

template <class Container>
  flat_multiset(Container)
    -> flat_multiset<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container>
  flat_multiset(Container)
    -> flat_multiset<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container, class Alloc>
  flat_multiset(Container, Alloc)
    -> flat_multiset<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container, class Alloc>
  flat_multiset(Container, Alloc)
    -> flat_multiset<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container>
  flat_multiset(sorted_t, Container)
    -> flat_multiset<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container>
  flat_multiset(sorted_t, Container)
    -> flat_multiset<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class Container, class Alloc>
  flat_multiset(sorted_t, Container, Alloc)
    -> flat_multiset<cont-value-type<Container>,
                less<cont-value-type<Container>>,
                std::vector<cont-value-type<Container>>>;

template <class Container, class Alloc>
  flat_multiset(sorted_t, Container, Alloc)
    -> flat_multiset<typename KeyContainer::value_type,
                less<typename KeyContainer::value_type>,
                Container>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
  flat_multiset(InputIterator, InputIterator, Compare = Compare())
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
  flat_multiset(InputIterator, InputIterator, Compare, Alloc)
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>, Compare,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Alloc>
  flat_multiset(InputIterator, InputIterator, Alloc)
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template <class InputIterator, class Compare = less<iter_key_t<InputIterator>>>
  flat_multiset(sorted_t, InputIterator, InputIterator, Compare = Compare())
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Compare, class Alloc>
  flat_multiset(sorted_t, InputIterator, InputIterator, Compare, Alloc)
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>, Compare,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class InputIterator, class Alloc>
  flat_multiset(sorted_t, InputIterator, InputIterator, Alloc)
    -> flat_multiset<iter_key_t<InputIterator>, iter_val_t<InputIterator>,
                less<iter_key_t<InputIterator>>,
                std::vector<iter_key_t<InputIterator>>,
                std::vector<iter_val_t<InputIterator>>>;

template<class Key, class Compare = less<Key>>
  flat_multiset(initializer_list<key_type>, Compare = Compare())
    -> flat_multiset<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Compare, class Alloc>
  flat_multiset(initializer_list<key_type>, Compare, Alloc)
    -> flat_multiset<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Alloc>
  flat_multiset(initializer_list<key_type>, Alloc)
    -> flat_multiset<Key, less<Key>, vector<Key>, vector<T>>;

template<class Key, class Compare = less<Key>>
flat_multiset(sorted_t, initializer_list<key_type>, Compare = Compare())
    -> flat_multiset<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Compare, class Alloc>
  flat_multiset(sorted_t, initializer_list<key_type>, Compare, Alloc)
    -> flat_multiset<Key, Compare, vector<Key>, vector<T>>;

template<class Key, class Alloc>
  flat_multiset(sorted_t, initializer_list<key_type>, Alloc)
    -> flat_multiset<Key, less<Key>, vector<Key>, vector<T>>;

// the comparisons below are for exposition only
template <class Key, class Compare, class Container>
  bool operator==(const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator< (const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator!=(const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator> (const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator>=(const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);
template <class Key, class Compare, class Container>
  bool operator<=(const flat_multiset<Key, Compare, Container>& x,
                  const flat_multiset<Key, Compare, Container>& y);

// specialized algorithms:
template <class Key, class Compare, class Container>
  void swap(flat_multiset<Key, Compare, Container>& x,
            flat_multiset<Key, Compare, Container>& y)
    noexcept(noexcept(x.swap(y)));

}
