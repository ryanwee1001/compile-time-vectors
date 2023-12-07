#ifndef VEC_CONCEPTS_H
#define VEC_CONCEPTS_H

template<class T>
struct is_vector : std::false_type {};

template<class T, class A>
struct is_vector<std::vector<T, A>> : std::true_type {};

template<class T, std::size_t N>
struct is_vector<MyVec<T, N>> : std::true_type {};

// Naive Vector Concept
// Issue: User-defined types that are not CopyConstructible, Erasable, CopyInsertible, 
// EqualityComparable, or Destructible will pass this check.
template <class V>
concept NaiveVector = is_vector<V>::value;


template <class C>
concept allocator_aware = requires (C a) {
    { a.get_allocator() } -> std::same_as<typename C::allocator_type>;
};

template <class T>
struct is_basic_string : std::false_type {};

template <class C, class T, class A>
struct is_basic_string<std::basic_string<C, T, A>> : std::true_type {};

template <class T>
concept String = is_basic_string<T>::value;

template<class T>
concept default_erasable = requires(T* p) { std::destroy_at(p); };

template<class T>
concept default_constructible = requires(T* p, T v) { std::construct_at(p, v); };

template<class C, class T, class A>
concept allocator_erasable = requires(A m, T* p) {
    requires std::same_as<typename C::allocator_type, typename std::allocator_traits<A>::template rebind_alloc<T>>;
    std::allocator_traits<A>::destroy(m, p);
};

template<class C, class T, class A>
concept allocator_constructible = requires(A m, T* p, T v) {
    requires std::same_as<typename C::allocator_type, typename std::allocator_traits<A>::template rebind_alloc<T>>;
    std::allocator_traits<A>::construct(m, p, v);
};

// concept for https://en.cppreference.com/w/cpp/named_req/Erasable
template <class C, class T>
concept Erasable = (String<C> && default_erasable<T>) 
                || (!allocator_aware<C> && default_erasable<T>)
                || (allocator_aware<C> && allocator_erasable<C, T, typename C::allocator_type>);

template <class C, class T>
concept CopyInsertable = (String<C> && default_constructible<T>) 
                || (!allocator_aware<C> && default_constructible<T>)
                || (allocator_aware<C> && allocator_constructible<C, T, typename C::allocator_type>);

// Inspired by https://stackoverflow.com/questions/60449592/how-do-you-define-a-c-concept-for-the-standard-library-containers
template <class C>
concept Container = requires(C a, C b, const C c, C&& rv) {
    // Requirements on C
    requires std::regular<C>;
    requires std::swappable<C>;

    // Requirements on value_type
    requires std::copy_constructible<typename C::value_type>;
    requires std::destructible<typename C::value_type>;
    requires std::equality_comparable<typename C::value_type>;
    requires Erasable<C, typename C::value_type>;
    requires CopyInsertable<C, typename C::value_type>;

    // Requirements on references
    requires std::same_as<typename C::reference, typename C::value_type&>;
    requires std::same_as<typename C::const_reference, const typename C::value_type&>;

    // Requirements on iterators
    requires std::same_as<std::iter_value_t<typename C::iterator>, typename C::value_type>;
    requires std::same_as<std::iter_value_t<typename C::const_iterator>, typename C::value_type>;
    requires std::forward_iterator<typename C::iterator>;
    requires std::forward_iterator<typename C::const_iterator>;

    // Requirements on size_type and difference_type
    requires std::signed_integral<typename C::difference_type>;
    requires std::integral<typename C::size_type>;
    requires std::same_as<typename C::difference_type, typename std::iterator_traits<typename C::iterator>::difference_type>;
    requires std::same_as<typename C::difference_type, typename std::iterator_traits<typename C::const_iterator>::difference_type>;
    
    // Member functions and operators
    { C() } -> std::same_as<C>;
    { C(a) } -> std::same_as<C>;
    { C(rv) } -> std::same_as<C>;
    { a = b } -> std::same_as<C&>;
    { a = rv } -> std::same_as<C&>;
    { a.~C() } -> std::same_as<void>;
    { a.begin() } -> std::same_as<typename C::iterator>;
    { c.begin() } -> std::same_as<typename C::const_iterator>;
    { a.end() } -> std::same_as<typename C::iterator>;
    { c.end() } -> std::same_as<typename C::const_iterator>;
    { a.cbegin() } -> std::same_as<typename C::const_iterator>;
    { a.cend() } -> std::same_as<typename C::const_iterator>;
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
    { a.swap(b) } -> std::same_as<void>;
    { std::swap(a, b) } -> std::same_as<void>;
    { a.size() } -> std::same_as<typename C::size_type>;
    { a.max_size() } -> std::same_as<typename C::size_type>;
    { a.empty() } -> std::convertible_to<bool>;
};

template<class Vec>
concept Vector = requires(
        Vec a, 
        Vec b,
        const Vec c,
        typename Vec::value_type elem,
        const typename Vec::value_type const_elem,
        typename Vec::value_type&& rv_elem) 
{
    // Has to be a container
    requires Container<Vec>;

    // Requirements on iterators
    requires std::contiguous_iterator<typename Vec::iterator>;
    requires std::contiguous_iterator<typename Vec::const_iterator>;
    requires std::same_as<typename Vec::reverse_iterator, std::reverse_iterator<typename Vec::iterator>>;
    requires std::same_as<typename Vec::const_reverse_iterator, std::reverse_iterator<typename Vec::const_iterator>>;

    // Member functiosn and operators (could always add more for tighter requirements)
    { a[0] } -> std::same_as<typename Vec::reference>;
    { a.data() } -> std::same_as<typename Vec::pointer>;
    { a.rbegin() } -> std::same_as<typename Vec::reverse_iterator>;
    { c.rbegin() } -> std::same_as<typename Vec::const_reverse_iterator>;
    { a.rend() } -> std::same_as<typename Vec::reverse_iterator>;
    { c.rend() } -> std::same_as<typename Vec::const_reverse_iterator>;
    { a.crbegin() } -> std::same_as<typename Vec::const_reverse_iterator>;
    { a.crend() } -> std::same_as<typename Vec::const_reverse_iterator>;
    { a.capacity() } -> std::same_as<typename Vec::size_type>;
    { a.clear() } -> std:: same_as<void>;
    { a.insert(a.begin(), const_elem) } -> std::same_as<typename Vec::iterator>;
    { a.insert(a.begin(), elem) } -> std::same_as<typename Vec::iterator>;
    { a.insert(a.begin(), 10, const_elem) } -> std::same_as<typename Vec::iterator>;
    { a.insert(a.begin(), b.begin(), b.end() ) } -> std::same_as<typename Vec::iterator>;
    { a.insert(a.begin(), {elem, elem})} -> std::same_as<typename Vec::iterator>;
    { a.erase(a.begin()) } -> std::same_as<typename Vec::iterator>;
    { a.erase(a.begin(), a.end() )} -> std::same_as<typename Vec::iterator>;
    { a.push_back(const_elem) } -> std::same_as<void>;
    { a.push_back(rv_elem) } -> std::same_as<void>;
    { a.emplace_back(elem) } -> std::same_as<typename Vec::reference>;
    { a.pop_back() } -> std::same_as<void>;
    { a.resize(0) } -> std::same_as<void>;
    { a.swap(b) } -> std::same_as<void>;
    { a <=> b };
    { std::swap(a, b) } -> std::same_as<void>;
};

#endif // VEC_CONCEPTS_H