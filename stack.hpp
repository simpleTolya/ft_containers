/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 18:46:47 by marmand           #+#    #+#             */
/*   Updated: 2022/03/26 20:04:51 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STACK_HPP
# define FT_STACK_HPP

#include "vector.hpp"
#include "utility.hpp"

namespace ft
{

template< class T, class Container = ft::vector<T> >
class stack
{
protected:
	Container	__container;
public:
	typedef Container							container_type;
	typedef typename Container::value_type		value_type;
	typedef typename Container::size_type		size_type;
	typedef typename Container::reference		reference;
	typedef typename Container::const_reference	const_reference;
	
	explicit stack( const Container& cont = Container() ) :
		__container(cont) {};
	
	stack( const stack& other ) : __container(other.__container) {};
	~stack() {};
	stack& operator=( const stack& other )
	{
		__container = other.__container;
		return *this;
	}

	reference top()
	{ return __container.back(); }
	
	const_reference top() const
	{ return __container.back(); }
	
	bool empty() const
	{ return __container.empty(); }
	
	size_type size() const
	{ return __container.size(); }
	
	void push( const value_type& value )
	{ return __container.push_back(value); }
	
	void pop()
	{ __container.pop_back(); }


	friend bool operator==( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container == rhs.__container;
	}

	friend bool operator!=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container != rhs.__container;
	}

	friend bool operator<( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container < rhs.__container;
	}

	friend bool operator<=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container <= rhs.__container;
	}

	friend bool operator>( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container > rhs.__container;
	}

	friend bool operator>=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
	{
		return lhs.__container >= rhs.__container;
	}
};


}

#endif // FT_STACK_HPP