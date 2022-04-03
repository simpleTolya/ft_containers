/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 18:09:47 by marmand           #+#    #+#             */
/*   Updated: 2022/03/27 19:58:07 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MAP_HPP
# define FT_MAP_HPP

#include "utility.hpp"
#include "RBTree.hpp"


namespace ft
{

template <class Key, class T, class Compare>
struct PairCompare
{
	bool	operator()(const ft::pair<const Key, T> & first, const ft::pair<const Key, T> & second) const
	{
		return Compare()(first.first, second.first);
	}
};

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T> >
> class map
{
public:
	typedef Key									key_type;
	typedef T									mapped_type;
	typedef ft::pair<const Key, T>				value_type;
	typedef std::size_t							size_type;
	typedef std::ptrdiff_t						difference_type;
	typedef PairCompare<Key, T, Compare>		pair_compare;
	typedef	Compare								key_compare;
	typedef Allocator							allocator_type;
	typedef value_type&							reference;
	typedef const value_type&					const_reference;
	typedef typename Allocator::pointer			pointer;
	typedef typename Allocator::const_pointer	const_pointer;
	
	typedef RBTreeIterator<ft::pair<const Key, T> >			iterator;
	typedef RBTreeConstIterator<ft::pair<const Key, T> >	const_iterator;
	typedef ft::reverse_iterator<iterator>					reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;

	class value_compare : std::binary_function<value_type, value_type, bool>
	{
		friend class map<key_type, value_type, key_compare, Allocator>;
		
		protected:
			Compare comp;
			value_compare (Compare c) : comp(c) {}
		
		public:
			bool operator() (const value_type& x, const value_type& y) const
			{ return (comp(x.first, y.first)); }
	};
private:
	RBTree<ft::pair<const Key, T>, pair_compare, Allocator>		__tree;
public:
	
	map() : __tree()
	{}

	explicit map( const Compare& comp,
			const Allocator& alloc = Allocator() )
			: __tree(comp, alloc)
	{}
	
	template< class InputIt >
	map( InputIt first, InputIt last,
		const Compare& comp = Compare(),
		const Allocator& alloc = Allocator() )
	{
		for (; first != last; ++first)
			__tree.insert(*first);
	}

	map( const map& other ) : __tree(other.__tree)
	{}

	~map()
	{}

	map& operator=( const map& other )
	{
		__tree = other.__tree;
		return *this;
	}
	
	allocator_type get_allocator() const
	{ return __tree.get_allocator(); }

	T& at( const Key& key )
	{
		iterator	it = find(key);
		if (it == end())
			throw std::out_of_range("map");
		return	it->second;
	}
	
	const T& at( const Key& key ) const
	{
		const_iterator	it = find(key);
		if (it == end())
			throw std::out_of_range("map");
		return	it->second;
	}

	T& operator[]( const Key& key )
	{
		ft::pair<iterator, bool>	it = insert(ft::make_pair(key, T()));
		return	(it.first)->second;
	}

	bool		empty() const
	{ return __tree.empty(); }

	size_type	size() const
	{ return __tree.size(); }

	size_type	max_size() const
	{ return __tree.max_size(); }

	iterator	begin()
	{ return __tree.begin(); }

	const_iterator	begin() const
	{ return __tree.begin(); }

	iterator	end()
	{ return __tree.end(); }

	const_iterator	end() const
	{ return __tree.end(); }

	reverse_iterator		rbegin()
	{ return __tree.rbegin(); }
	
	const_reverse_iterator	rbegin() const
	{ return __tree.rbegin(); }
	
	reverse_iterator		rend()
	{ return __tree.rend(); }
	
	const_reverse_iterator	rend() const
	{ return __tree.rend(); }

	void clear()
	{ __tree.clear(); }

	ft::pair<iterator,bool> insert( const value_type& value )
	{ return __tree.insert(value); }

	iterator insert( iterator hint, const value_type& value )
	{ return __tree.insert(hint, value); }

	template< class InputIt >
	void insert( InputIt first, InputIt last )
	{ __tree.insert(first, last); }

	void erase( iterator pos )
	{ __tree.erase(pos); }

	void erase( iterator first, iterator last )
	{ __tree.erase(first, last); }

	size_type erase( const Key& key )
	{ return __tree.erase(ft::make_pair(key, T())); }

	void	swap(map & other)
	{ __tree.swap(other.__tree); }

	size_type count( const Key& key ) const
	{ return __tree.find(ft::make_pair(key, T())) == __tree.end() ? 0 : 1; }

	iterator find( const Key& key )
	{ return __tree.find(ft::make_pair(key, T())); }

	const_iterator find( const Key& key ) const
	{ return __tree.find(ft::make_pair(key, T())); }

	ft::pair<iterator,iterator> equal_range( const Key& key )
	{ return __tree.equal_range(ft::make_pair(key, T())); }

	ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
	{ return __tree.equal_range(ft::make_pair(key, T())); }

	iterator lower_bound( const Key& key )
	{ return __tree.lower_bound(ft::make_pair(key, T())); }

	const_iterator lower_bound( const Key& key ) const
	{ return __tree.lower_bound(ft::make_pair(key, T())); }

	iterator upper_bound( const Key& key )
	{ return __tree.upper_bound(ft::make_pair(key, T())); }

	const_iterator upper_bound( const Key& key ) const
	{ return __tree.upper_bound(ft::make_pair(key, T())); }

	key_compare key_comp() const
	{ return key_compare(); }

	value_compare value_comp() const
	{ return value_compare(key_compare()); }
};

template< class Key, class T, class Compare, class Allocator >
bool	operator==(const map<Key, T, Compare, Allocator> & lhs,
					const map<Key, T, Compare, Allocator> & rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template< class Key, class T, class Compare, class Allocator >
bool	operator!=(const map<Key, T, Compare, Allocator> & lhs,
					const map<Key, T, Compare, Allocator> & rhs)
{
	return !(lhs == rhs);
}

template< class Key, class T, class Compare, class Allocator >
bool	operator<(const map<Key, T, Compare, Allocator>& lhs,
					const map<Key, T, Compare, Allocator>& rhs)
{
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end()
										));
}

template< class Key, class T, class Compare, class Allocator >
bool	operator<=(const map<Key, T, Compare, Allocator>& lhs,
					const map<Key, T, Compare, Allocator>& rhs)
{
	return (!(rhs < lhs));
}

template< class Key, class T, class Compare, class Allocator >
bool	operator>(const map<Key, T, Compare, Allocator>& lhs,
					const map<Key, T, Compare, Allocator>& rhs)
{
	return (rhs < lhs);
}

template< class Key, class T, class Compare, class Allocator >
bool	operator>=(const map<Key, T, Compare, Allocator>& lhs,
					const map<Key, T, Compare, Allocator>& rhs)
{
	return (!(lhs < rhs));
}


} // namespace ft

namespace std
{
	template< class Key, class T, class Compare, class Allocator >
	void	swap( ft::map<Key, T, Compare, Allocator> & lhs,
				ft::map<Key, T, Compare, Allocator> & rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // FT_MAP_HPP