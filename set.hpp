/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 16:04:21 by marmand           #+#    #+#             */
/*   Updated: 2022/03/27 17:34:30 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	FT_SET_HPP
# define FT_SET_HPP

#include "utility.hpp"
#include "RBTree.hpp"

namespace ft
{

template <class Key,
    	class Compare = std::less<Key>,
		class Allocator = std::allocator<Key>
		>
class set
{
private:
	ft::RBTree<Key, Compare, Allocator>	__tree;
public:
	typedef	Key									key_type;
	typedef	Key									value_type;
	typedef	std::size_t							size_type;
	typedef	std::ptrdiff_t						difference_type;
	typedef	Compare								key_compare;
	typedef	Allocator							allocator_type;
	typedef	value_type&							reference;
	typedef	const value_type&					const_reference;
	typedef	typename Allocator::pointer			pointer;
	typedef typename Allocator::const_pointer	const_pointer;

	typedef typename ft::RBTree<Key, Compare, Allocator>::iterator					iterator;
	typedef typename ft::RBTree<Key, Compare, Allocator>::const_iterator			const_iterator;
	typedef	typename ft::RBTree<Key, Compare, Allocator>::reverse_iterator			reverse_iterator;
	typedef	typename ft::RBTree<Key, Compare, Allocator>::const_reverse_iterator	const_reverse_iterator;

	class value_compare : std::binary_function<value_type, value_type, bool>
	{
		friend class set<key_type, key_compare, Allocator>;
		
		protected:
			Compare comp;
			value_compare (Compare c) : comp(c) {}
		
		public:
			bool operator() (const value_type& x, const value_type& y) const
			{ return (comp(x.first, y.first)); }
	};

	set() : __tree()
	{}

	explicit set( const Compare& comp,
			const Allocator& alloc = Allocator() )
			: __tree(comp, alloc)
	{}

	set( const set& other ) : __tree(other.__tree)
	{}

	template< class InputIt >
	set( InputIt first, InputIt last,
		const Compare& comp = Compare(),
		const Allocator& alloc = Allocator() )
	{
		for (; first != last; ++first)
			__tree.insert(*first);
	}

	~set()
	{}

	set& operator=( const set& other )
	{
		__tree = other.__tree;
		return *this;
	}

	allocator_type get_allocator() const
	{ return __tree.get_allocator(); }

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

	size_type erase( const value_type& key )
	{ return __tree.erase(key); }

	iterator find( const value_type& key )
	{ return __tree.find(key); }

	const_iterator find( const value_type& key ) const
	{ return __tree.find(key); }

	ft::pair<iterator,iterator> equal_range( const value_type& key )
	{ return __tree.equal_range(key); }

	ft::pair<const_iterator,const_iterator> equal_range( const value_type& key ) const
	{ return __tree.equal_range(key); }

	iterator lower_bound( const value_type& key )
	{ return __tree.lower_bound(key); }

	const_iterator lower_bound( const value_type& key ) const
	{ return __tree.lower_bound(key); }

	iterator upper_bound( const value_type& key )
	{ return __tree.upper_bound(key); }

	const_iterator upper_bound( const value_type& key ) const
	{ return __tree.upper_bound(key); }

	void	swap(set & other)
	{ __tree.swap(other.__tree); }

	size_type count( const Key& key ) const
	{ return __tree.find(key) == __tree.end() ? 0 : 1; }

	void clear()
	{ __tree.clear(); }

	key_compare key_comp() const
	{ return key_compare(); }

	value_compare value_comp() const
	{ return value_compare(key_compare()); }

};


template<class T, class Compare, class Allocator>
bool	operator==(const set<T, Compare, Allocator> & lhs, const set<T, Compare, Allocator> & rhs) {
	if (lhs.size() != rhs.size())
		return false;
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, class Compare, class Allocator>
bool	operator!=(const set<T, Compare, Allocator> & lhs, const set<T, Compare, Allocator> & rhs) {
	return !(lhs == rhs);
}

template<class T, class Compare, class Allocator>
bool	operator<(const set<T, Compare, Allocator>& lhs, const set<T, Compare, Allocator>& rhs) {
	return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template<class T, class Compare, class Allocator>
bool	operator<=(const set<T, Compare, Allocator>& lhs, const set<T, Compare, Allocator>& rhs)
{
	return (!(rhs < lhs));
}

template<class T, class Compare, class Allocator>
bool	operator>(const set<T, Compare, Allocator>& lhs, const set<T, Compare, Allocator>& rhs)
{
	return (rhs < lhs);
}

template<class T, class Compare, class Allocator>
bool	operator>=(const set<T, Compare, Allocator>& lhs, const set<T, Compare, Allocator>& rhs)
{
	return (!(lhs < rhs));
}



} // namespace ft

namespace std
{
	template <class T, class Compare, class Allocator>
	void	swap( ft::set<T, Compare, Allocator> & lhs,
				ft::set<T, Compare, Allocator> & rhs)
	{
		lhs.swap(rhs);
	}
}

#endif // FT_SET_HPP