/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 21:21:55 by marmand           #+#    #+#             */
/*   Updated: 2022/03/27 20:21:52 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_UTILS_HPP
# define FT_UTILS_HPP

# include <cstddef>

namespace ft
{

template<class T>
void	swap(T & a, T & b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template <class T>
const T&	min( const T& a, const T& b )
{
	if (a < b)
		return a;
	return b;
}

template <class T>
const T&	max( const T& a, const T& b )
{
	if (a > b)
		return a;
	return b;
}



template <class T1, class T2>
struct pair
{
	typedef T1 first_type;
	typedef T2 second_type;
	
	T1	first;
	T2	second;
	
	pair() : first(), second() {}
	pair( const T1& x, const T2& y ) : first(x), second(y) {}
	
	template< class U1, class U2 >
		pair( const pair<U1, U2>& p ) : first(p.first), second(p.second) {}
};

template< class T1, class T2 >
pair<T1,T2> make_pair( T1 t, T2 u )
{
	return pair<T1, T2>(t, u);	
};

template< class T1, class T2 >
bool operator==(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	if (lhs.first == rhs.first and lhs.second == rhs.second)
		return true;
	return false;
};

template< class T1, class T2 >
bool operator!=(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	if (lhs.first != rhs.first or lhs.second != rhs.second)
		return true;
	return false;
};

template< class T1, class T2 >
bool operator<(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
};

template< class T1, class T2 >
bool operator>(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	return (rhs < lhs);
};

template< class T1, class T2 >
bool operator<=(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	return !(rhs < lhs);
};

template< class T1, class T2 >
bool operator>=(const pair<T1,T2> & lhs, const pair<T1,T2> & rhs)
{
	return !(lhs < rhs);
};



template <class T>
struct remove_const {
	typedef T	type;	
};

template <class T>
struct remove_const<const T> {
	typedef T	type;
};



template <class T>
struct remove_reference {
	typedef T	type;	
};

template <class T>
struct remove_reference<T &> {
	typedef T	type;
};



template <class T, T v = T()>
struct __intergral_helper
{
	static const T value = v;
	int	for_size;
};

template< class T >
struct is_integral
{
private:

	template <class Some>
	static char f(...) {return 0;}
	
	template <class Some>
	static __intergral_helper<Some>	f(int a) {return a;}

public:
	static const bool value = sizeof(f<T>(0)) == sizeof(int);
	bool operator() ()
	{ return value; }
	
	operator bool ()
	{ return value; }
};



template <class T1, class T2>
struct is_same {
	static const bool value = false;	
};

template <class T>
struct is_same <T, T> {
	static const bool value = true;	
};



template <bool B, typename T = void>
struct enable_if
{};

template <typename T>
struct enable_if<true, T>
{
	typedef T	type;
};



template<class InputIt1, class InputIt2>
bool	equal(InputIt1 first1, InputIt1 last1, 
		InputIt2 first2)
{
	for (; first1 != last1; ++first1, ++first2) {
		if (!(*first1 == *first2)) {
			return false;
		}
	}
	return true;
}



template<class InputIt1, class InputIt2>
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1,
							InputIt2 first2, InputIt2 last2)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (*first1 < *first2)
			return true;
        if (*first2 < *first1)
			return false;
    }
    return (first1 == last1) && (first2 != last2);
}


template <class Base, class Derived>
struct __helper_is_base_of__ {
	operator Derived*();
	operator Base *() const;
};

template <class Base, class Derived>
struct is_base_of {
private:
	template <class T>
	static int f(Derived *, T);
 
	static char f(Base *, int);
public:
	static const bool value = sizeof(
			f(__helper_is_base_of__<Base, Derived>(), int())
			) == sizeof(int);	
};



struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template<
    class Category,
    class T,
    class Distance = std::ptrdiff_t,
    class Pointer = T*,
    class Reference = T&>
struct iterator
{
	typedef Category		iterator_category;	
	typedef T				value_type;
	typedef Distance		difference_type;
	typedef Pointer			pointer;
	typedef Reference		reference;
};



template <class T>
struct is_iterator {
private:
	template <class C>
	static typename C::iterator_category f(int a) {return C::iterator_category();};
	
	template <class C>
	static double	f(...) {return 0;};
public:
	static const bool value = sizeof(f<T>(1)) != sizeof(double);
};

template <class T>
struct is_iterator<T *> {
	static const bool value = true;
};



template< class Iter >
struct iterator_traits
{
	typedef Iter	iterator_type;
	typedef typename Iter::difference_type			difference_type;
	typedef typename Iter::value_type				value_type;
	typedef typename Iter::pointer					pointer;
	typedef typename Iter::reference				reference;
	typedef typename Iter::iterator_category		iterator_category;
};

template< class T >
struct iterator_traits<T*>
{
	typedef std::ptrdiff_t				difference_type;
	typedef T							value_type;
	typedef T*							pointer;
	typedef T&							reference;
	typedef random_access_iterator_tag	iterator_category;
};

template< class T >
struct iterator_traits<const T*>
{
	typedef std::ptrdiff_t				difference_type;
	typedef T							value_type;
	typedef const T*					pointer;
	typedef const T&					reference;
	typedef random_access_iterator_tag	iterator_category;
};

template< class Iter, class category = typename iterator_traits<Iter>::iterator_category>
class reverse_iterator
{
public:
	typedef	Iter 	iterator_type;
	typedef	typename iterator_traits<Iter>::iterator_category	iterator_category;
	typedef	typename iterator_traits<Iter>::value_type			value_type;
	typedef	typename iterator_traits<Iter>::difference_type		difference_type;
	typedef	typename iterator_traits<Iter>::pointer				pointer;
	typedef	typename iterator_traits<Iter>::reference			reference;

	reverse_iterator() : __iter() {}

	explicit reverse_iterator (iterator_type it) :
				__iter(it) {}
	
	reverse_iterator(const reverse_iterator & other) :
				__iter(other.__iter) {}
	

	~reverse_iterator() {}

	iterator_type base() const
	{ return (__iter); }

	reference operator*() const
	{
		iterator_type tmp = __iter;
		return (*(--tmp));
	}

	reverse_iterator& operator++()
	{
		--__iter;
		return (*this);
	}
	
	reverse_iterator operator++(int)
	{
		reverse_iterator tmp = *this;
		++(*this);
		return (tmp);
	}

	reverse_iterator& operator--()
	{
		++__iter;
		return (*this);
	}

	reverse_iterator operator--(int)
	{
		reverse_iterator tmp = *this;
		--(*this);
		return (tmp);
	}
	
	pointer operator->() const { return &(operator*()); }
	
	friend	bool operator==(reverse_iterator lhs, reverse_iterator rhs)
	{ return lhs.__iter == rhs.__iter; }
	
	friend	bool operator!=(reverse_iterator lhs, reverse_iterator rhs)
	{ return lhs.__iter != rhs.__iter; }

private:
	iterator_type	__iter;
};

template< class Iter>
class reverse_iterator<Iter, std::random_access_iterator_tag>
{
public:
	typedef	Iter 	iterator_type;
	typedef	typename iterator_traits<Iter>::iterator_category	iterator_category;
	typedef	typename iterator_traits<Iter>::value_type			value_type;
	typedef	typename iterator_traits<Iter>::difference_type		difference_type;
	typedef	typename iterator_traits<Iter>::pointer				pointer;
	typedef	typename iterator_traits<Iter>::reference			reference;

	reverse_iterator() : __iter() {}

	explicit reverse_iterator (iterator_type it) :
				__iter(it) {}
	
	reverse_iterator(const reverse_iterator & other) :
				__iter(other.__iter) {}
	
	~reverse_iterator() {}

	iterator_type base() const
	{ return (__iter); }

	reference operator*() const
	{
		iterator_type tmp = __iter;
		return (*(--tmp));
	}

	reverse_iterator operator+ (difference_type n) const
	{ return (reverse_iterator(__iter - n)); }

	reverse_iterator& operator++()
	{
		--__iter;
		return (*this);
	}
	
	reverse_iterator operator++(int)
	{
		reverse_iterator tmp = *this;
		++(*this);
		return (tmp);
	}

	reverse_iterator& operator+= (difference_type n)
	{
		__iter -= n;
		return (*this);
	}
	
	reverse_iterator operator- (difference_type n) const
	{ return (reverse_iterator(__iter + n)); }

	reverse_iterator& operator--()
	{
		++__iter;
		return (*this);
	}

	reverse_iterator operator--(int)
	{
		reverse_iterator tmp = *this;
		--(*this);
		return (tmp);
	}
	
	reverse_iterator& operator-= (difference_type n)
	{
		__iter += n;
		return (*this);
	}

	pointer operator->() const { return &(operator*()); }
	
	reference operator[] (difference_type n) const
	{ return (this->base()[-n - 1]); }
	
	friend	bool operator==(reverse_iterator lhs, reverse_iterator rhs)
	{ return lhs.__iter == rhs.__iter; }
	
	friend	bool operator!=(reverse_iterator lhs, reverse_iterator rhs)
	{ return lhs.__iter != rhs.__iter; }
private:
	iterator_type	__iter;
};




namespace detail {
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
		__do_distance(It first, It last, ft::input_iterator_tag)
	{
		typename ft::iterator_traits<It>::difference_type result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}
	
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
		__do_distance(It first, It last, ft::random_access_iterator_tag)
	{
		return last - first;
	}

	template<class It>
	typename ft::iterator_traits<It>::difference_type 
		__do_distance(It first, It last, std::input_iterator_tag)
	{
		typename ft::iterator_traits<It>::difference_type result = 0;
		while (first != last) {
			++first;
			++result;
		}
		return result;
	}
	
	template<class It>
	typename ft::iterator_traits<It>::difference_type 
		__do_distance(It first, It last, std::random_access_iterator_tag)
	{
		return last - first;
	}
}
 
 
template<class It>
typename ft::iterator_traits<It>::difference_type 
    distance(It first, It last)
{
    return detail::__do_distance(first, last,
						typename ft::iterator_traits<It>::iterator_category());
}



} // namespace ft


# endif // FT_UTILS_HPP