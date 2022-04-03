/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RBTree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marmand <marmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 21:56:37 by marmand           #+#    #+#             */
/*   Updated: 2022/03/28 13:04:47 by marmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_RBTREE
# define FT_RBTREE

#include "utility.hpp"
#include <algorithm>

namespace ft
{

template <class T>
class	RBTreeIterator;

template <class T>
class	RBTreeConstIterator;

template <class T>
struct Node
{
	Node	*left;
	Node	*right;
	Node	*parent;
	bool	is_red;
	T		value;
	Node(const T& value, Node *left = NULL, Node *right = NULL,
			Node *parent = NULL, bool is_red = true) :
			left(left), right(right), parent(parent),
			is_red(is_red), value(value)
	{}
};

template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
class RBTree
{
public:
	typedef	T						value_type;
	typedef unsigned long			size_type;
	typedef Allocator				allocator_type;
	typedef Compare					comparator_type;

	typedef Node<T>					Node;
	typedef RBTreeIterator<T>		iterator;
	typedef RBTreeConstIterator<T>	const_iterator;

	typedef ft::reverse_iterator<iterator>			reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	
	typedef typename allocator_type::template rebind<Node>::other	node_allocator_type;
private:
	Node *			__root;
	size_type		__size;
	allocator_type	__alloc;
	comparator_type	__comp;
public:
	RBTree() : __root(NULL), __size(0), __alloc()
	{}

	explicit RBTree( const Compare& comp,
						const Allocator& alloc = Allocator() )
			: __comp(comp), __alloc(alloc), __root(NULL), __size(0)
	{}

	RBTree( const RBTree& other );

	~RBTree()
	{_clear_tree(__root); };

	RBTree& operator=( const RBTree& other );

	allocator_type get_allocator() const
	{ return __alloc; }

	bool		empty() const
	{ return __root == NULL; }

	size_type	size() const
	{ return __size; }

	size_type	max_size() const
	{ return (node_allocator_type().max_size()); }

	iterator		begin()
	{
		Node    *min = __root;
		if (min)
		{
			while (min->left)
				min = min->left;
		}
		return iterator(min, __root);
	}

	const_iterator	begin() const
	{
		Node    *min = __root;
		if (min)
		{
			while (min->left)
				min = min->left;
		}
		return const_iterator(min, __root);
	}

	iterator		end()
	{ return iterator(NULL, __root); }

	const_iterator	end() const
	{ return const_iterator(NULL, __root); }

	reverse_iterator		rbegin()
	{ return reverse_iterator(end()); }
	
	const_reverse_iterator	rbegin() const
	{ return const_reverse_iterator(end()); }
	
	reverse_iterator		rend()
	{ return reverse_iterator(begin()); }
	
	const_reverse_iterator	rend() const
	{ return const_reverse_iterator(begin()); }

	ft::pair<iterator,bool> insert( const value_type& value );

	iterator insert( iterator hint, const value_type& value );

	template< class InputIt >
	void insert( InputIt first, InputIt last );

	void erase( iterator pos );
	void erase( iterator first, iterator last );
	size_type erase( const value_type& key );

	iterator find( const value_type& key );
	const_iterator find( const value_type& key ) const;

	ft::pair<iterator,iterator> equal_range( const value_type& key );
	ft::pair<const_iterator,const_iterator> equal_range( const value_type& key ) const;

	iterator lower_bound( const value_type& key );
	const_iterator lower_bound( const value_type& key ) const;

	iterator upper_bound( const value_type& key );
	const_iterator upper_bound( const value_type& key ) const;

	void	swap(RBTree& other)
	{
		ft::swap(other.__root, __root);
		ft::swap(other.__size, __size);
	}

	void clear()
	{ _clear_tree(__root); __root = NULL; }
private:

	Node *	_get_new_node(const T& value, Node *parent = NULL, Node *left = NULL,
							Node *right = NULL, bool is_red = true)
	{
		Node	* tmp = node_allocator_type().allocate(1);
		node_allocator_type().construct(
				tmp, Node(value, left, right, parent, is_red));
		__size++;
		return tmp;
	}

	void	_remove_node(Node	*node)
	{
		node_allocator_type().destroy(node);
		node_allocator_type().deallocate(node, 1);
		__size--;
	}

	bool	_is_red(Node *node)
	{ return node == NULL ? false : node->is_red; }

	Node *	_find_node(const T & key) const;
	ft::pair<Node*, bool>	_insert_to_tree(Node **tree, const T & key);
	void	_fix_node_after_inserting(Node *node);

	void	_copy_tree(Node *&current, Node *curr_parent, Node *other_node);
	void	_clear_tree(Node *current);

	void	_rotate_left(Node * node);
	void	_rotate_right(Node * node);

	void	_erase_node(Node *node);
	void	_swap_nodes(Node *nd1, Node *nd2);
	void	_fix_node_after_erasing(Node **removed, Node *parent);
};

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_copy_tree(Node *&current, Node *curr_parent, Node *other_node)
{
	if (other_node == NULL)
		return ;
	current = _get_new_node(other_node->value,
							curr_parent);
	_copy_tree(current->left, current, other_node->left);
	_copy_tree(current->right, current, other_node->right);
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_clear_tree(Node *current)
{
	if (current == NULL)
		return ;
	_clear_tree(current->left);
	_clear_tree(current->right);
	_remove_node(current);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::Node *	RBTree<T, Compare, Allocator>::_find_node(const T & key) const
{
	Node	* tmp = __root;

	while (tmp != NULL)
	{
		if (__comp(key, tmp->value))
			tmp = tmp->left;
		else if (__comp(tmp->value, key))
			tmp = tmp->right;
		else
			return tmp;
	}
	return NULL;
}

template <class T, class Compare, class Allocator>
ft::pair<typename RBTree<T, Compare, Allocator>::Node *, bool>	RBTree<T, Compare, Allocator>::_insert_to_tree(Node **tree, const T & key)
{
	Node	*parent = *tree == NULL ? NULL : (*tree)->parent;

	while (*tree != NULL)
	{
		if (__comp(key, (*tree)->value))
		{
			parent = *tree;
			tree = &((*tree)->left);
		}
		else if (__comp((*tree)->value, key))
		{
			parent = *tree;
			tree = &((*tree)->right);
		}
		else
			return ft::make_pair(*tree, false);
	}
	*tree = _get_new_node(key, parent);
	_fix_node_after_inserting(*tree);
	return ft::make_pair(*tree, true);
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_fix_node_after_inserting(Node *node)
{
	while (_is_red(node->parent))
	{
		if (node == node->parent->left)
		{
			if (_is_red(node->parent->parent->right)) // red uncle
			{
				node->parent->is_red = false;
				node->parent->parent->right->is_red = false;
				node->parent->parent->is_red = true;
				node = node->parent->parent;
				continue;
			}
			else // black uncle
			{
				node->parent->is_red = false;
				node->parent->parent->is_red = true;
				_rotate_right(node->parent->parent);
				break ;
			}
		}
		else
		{
			if (_is_red(node->parent->parent->left)) // red uncle
			{
				node->parent->is_red = false;
				node->parent->parent->left->is_red = false;
				node->parent->parent->is_red = true;
				node = node->parent->parent;
				continue;
			}
			else // black uncle
			{
				node->parent->is_red = false;
				node->parent->parent->is_red = true;
				_rotate_left(node->parent->parent);
				break ;
			}
		}
	}
	__root->is_red = false;
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_swap_nodes(Node *nd1, Node *nd2)
{
	ft::swap(nd1->is_red, nd2->is_red);
	if (nd1->left)
		nd1->left->parent = nd2;
	if (nd2->left)
		nd2->left->parent = nd1;
	ft::swap(nd1->left, nd2->left);

	if (nd1->right)
		nd1->right->parent = nd2;
	if (nd2->right)
		nd2->right->parent = nd1;
	ft::swap(nd1->right, nd2->right);

	if (nd1->parent)
	{
		if (nd1->parent->left == nd1)
			nd1->parent->left = nd2;
		else
			nd1->parent->right = nd2;
	}
	else
		__root = nd2;
	if (nd2->parent)
	{
		if (nd2->parent->left == nd2)
			nd2->parent->left = nd1;
		else
			nd2->parent->right = nd1;
	}
	else
		__root = nd1;
	ft::swap(nd1->parent, nd2->parent);
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_erase_node(Node *node)
{
	Node	*to_del = node;

	if (node->left and node->right)
	{
		to_del = node->right;
		while (to_del->left)
			to_del = to_del->left;
		_swap_nodes(node, to_del);
		_erase_node(node);
	}
	else if (node->left)
	{
		node->left->is_red = false;
		node->left->parent = node->parent;
		if (node->parent)
		{
			if (node->parent->left == node)
				node->parent->left = node->left;
			else
				node->parent->right = node->left;
		}
		else
			__root = node->left;
		_remove_node(to_del);
		return ;
	}
	else if (node->right)
	{
		node->right->is_red = false;
		node->right->parent = node->parent;
		if (node->parent)
		{
			if (node->parent->right == node)
				node->parent->right = node->right;
			else
				node->parent->left = node->right;
		}
		else
			__root = node->right;
		_remove_node(to_del);
		return ;
	}
	else
	{
		if (node->is_red)
		{
			Node	**tmp;
			if (node->parent->right == node)
				tmp = &node->parent->right;
			else
				tmp = &node->parent->left;
			_remove_node(*tmp);
			*tmp = NULL;
		}
		else
		{
			Node	**tmp;
			Node    *parent = node->parent;
			if (node->parent)
			{
				if (node->parent->right == node)
					tmp = &node->parent->right;
				else
					tmp = &node->parent->left;
			}
			else
				tmp = &__root;
			_remove_node(*tmp);
			*tmp = NULL;
			_fix_node_after_erasing(tmp, parent);
		}
	}
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_fix_node_after_erasing(Node **removed, Node *parent)
{
	while (parent != NULL)
	{
		if (&parent->right == removed) // right removed
		{
			if (_is_red(parent)) // red parent
			{
				if (_is_red(parent->left->right)) // maybe
				{
					parent->is_red = false;
					_rotate_left(parent->left);
				}
				_rotate_right(parent);
				return ;
			}
			else // black parent
			{
				if (_is_red(parent->left)) // red brother
				{
					if (_is_red(parent->left->right->left))
					{
						parent->left->right->left->is_red = false;
						_rotate_left(parent->left);
						_rotate_right(parent);
					}
					else if (_is_red(parent->left->right->right))
					{
						ft::swap(parent->left->right->is_red,
									parent->left->right->right->is_red);
						_rotate_left(parent->left->right);
						parent->left->right->left->is_red = false;
						_rotate_left(parent->left);
						_rotate_right(parent);
					}
					else
					{
						parent->left->is_red = false;
						parent->left->right->is_red = true;
						_rotate_right(parent);
					}
					return;
				}
				else // black brother
				{
					if (_is_red(parent->left->right))
					{
						parent->left->right->is_red = false;
						_rotate_left(parent->left);
						_rotate_right(parent);
						return ;
					}
					else if (_is_red(parent->left->left))
					{
						parent->left->left->is_red = false;
						_rotate_right(parent);
						return ;
					}
					parent->left->is_red = true;
					if (parent->parent)
					{
						if (parent->parent->left == parent)
							removed = &parent->parent->left;
						else
							removed = &parent->parent->right;
					}
					parent = parent->parent;
				}
			}
		}
		else // left removed
		{
			if (_is_red(parent)) // red parent
			{
				if (_is_red(parent->right->left)) // maybe
				{
					parent->is_red = false;
					_rotate_right(parent->right);
				}
				_rotate_left(parent);
				return ;
			}
			else // black parent
			{
				if (_is_red(parent->right)) // red brother
				{
					if (_is_red(parent->right->left->right))
					{
						parent->right->left->right->is_red = false;
						_rotate_right(parent->right);
						_rotate_left(parent);
					}
					else if (_is_red(parent->right->left->left))
					{
						ft::swap(parent->right->left->is_red,
									parent->right->left->left->is_red);
						_rotate_right(parent->right->left);
						parent->right->left->right->is_red = false;
						_rotate_right(parent->right);
						_rotate_left(parent);
					}
					else
					{
						parent->right->is_red = false;
						parent->right->left->is_red = true;
						_rotate_left(parent);
					}
					return;
				}
				else // black brother
				{
					if (_is_red(parent->right->left))
					{
						parent->right->left->is_red = false;
						_rotate_right(parent->right);
						_rotate_left(parent);
						return ;
					}
					else if (_is_red(parent->right->right))
					{
						parent->right->right->is_red = false;
						_rotate_left(parent);
						return ;
					}
					parent->right->is_red = true;
					if (parent->parent)
					{
						if (parent->parent->right == parent)
							removed = &parent->parent->right;
						else
							removed = &parent->parent->left;
					}
					parent = parent->parent;
				}
			}
		}
	}
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_rotate_left(Node * node)
{
	Node	*tmp = node->right;

	node->right = tmp->left;
	if (tmp->left)
		tmp->left->parent = node;
	tmp->left = node;
	tmp->parent = node->parent;
	node->parent = tmp;
	if (tmp->parent)
	{
		if (tmp->parent->right == tmp->left)
			tmp->parent->right = tmp;
		else
			tmp->parent->left = tmp;
	}
	else
		__root = tmp;
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::_rotate_right(Node * node)
{
	Node	*tmp = node->left;

	node->left = tmp->right;
	if (tmp->right)
		tmp->right->parent = node;
	tmp->right = node;
	tmp->parent = node->parent;
	node->parent = tmp;
	if (tmp->parent)
	{
		if (tmp->parent->left == tmp->right)
			tmp->parent->left = tmp;
		else
			tmp->parent->right = tmp;
	}
	else
		__root = tmp;
}

// public methods
template <class T, class Compare, class Allocator>
RBTree<T, Compare, Allocator>::RBTree( const RBTree& other ) :
		__alloc(other.__alloc), __comp(other.__comp), __root(NULL)
{
	_copy_tree(__root, NULL, other.__root);
	__size = other.__size;
}

template <class T, class Compare, class Allocator>
RBTree<T, Compare, Allocator>&	RBTree<T, Compare, Allocator>::operator=( const RBTree& other )
{
	Node	*tmp_tree = NULL;

	_copy_tree(tmp_tree, NULL, other.__root);
	_clear_tree(__root);
	__root = tmp_tree;
	__size = other.__size;
	return *this;
}

template <class T>
class	RBTreeIterator :
		public ft::iterator<std::bidirectional_iterator_tag, T>
{
public:
	typedef	Node<T>	Node;

	Node	*node;
	Node	*root;
	RBTreeIterator(Node * node, Node *root) : node(node), root(root) {};
public:
	RBTreeIterator() {}
	RBTreeIterator(const RBTreeIterator & other) : node(other.node), root(other.root) {};

	RBTreeIterator &	operator=(const RBTreeIterator & other) {
		node = other.node;
		return *this;
	}
	T &	operator*() {
		return node->value;
	}
	T *	operator->() {
		return &(node->value);
	}
	RBTreeIterator &	operator++() {
		if (node == NULL)
		{
			if (root == NULL)
				return *this;
			node = root;
			while (node->left)
				node = node->left;
			return *this;
		}
		if (node->right)
		{
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else
		{
			while (node->parent and node->parent->right == node)
				node = node->parent;
			node = node->parent;
		}
		return *this;
	}
	RBTreeIterator	operator++(int) {
		RBTreeIterator tmp(*this);
		++(*this);
		return tmp;
	}
	RBTreeIterator &	operator--() {
		if (node == NULL)
		{
			if (root == NULL)
				return *this;
			node = root;
			while (node->right)
				node = node->right;
			return *this;
		}
		if (node->left)
		{
			node = node->left;
			while (node->right)
				node = node->right;
		}
		else
		{
			while (node->parent and node->parent->left == node)
				node = node->parent;
			node = node->parent;
		}
		return *this;
	}
	RBTreeIterator	operator--(int) {
		RBTreeIterator tmp(*this);
		--(*this);
		return tmp;
	}
	operator RBTreeConstIterator<T>() const {
		return RBTreeConstIterator<T>(node, root);
	}
	friend bool operator==(
			RBTreeIterator lhs,
			RBTreeIterator rhs)
	{
		return lhs.node == rhs.node;
	}

	friend bool operator!=(
			RBTreeIterator lhs,
			RBTreeIterator rhs)
	{
		return lhs.node != rhs.node;
	}
};




template <class T>
class	RBTreeConstIterator :
		public ft::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
public:
	typedef	Node<T>	Node;

	const Node	*node;
	const Node	*root;
	RBTreeConstIterator(Node * node, Node *root) : node(node), root(root) {};
public:
	RBTreeConstIterator() {};
	RBTreeConstIterator(const RBTreeConstIterator & other) : node(other.node), root(other.root) {};
	RBTreeConstIterator &	operator=(const RBTreeConstIterator & other) {
		node = other.node;
		return *this;
	}
	const T &	operator*() const {
		return node->value;
	}
	const T *	operator->() const {
		return &(node->value);
	}
	RBTreeConstIterator &	operator++() {
		if (node == NULL)
		{
			if (root == NULL)
				return *this;
			node = root;
			while (node->left)
				node = node->left;
			return *this;
		}
		if (node->right)
		{
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else
		{
			while (node->parent and node->parent->right == node)
				node = node->parent;
			node = node->parent;
		}
		return *this;
	}
	RBTreeConstIterator	operator++(int) {
		RBTreeConstIterator tmp(*this);
		++(*this);
		return tmp;
	}
	RBTreeConstIterator &	operator--() {
		if (node == NULL)
		{
			if (root == NULL)
				return *this;
			node = root;
			while (node->right)
				node = node->right;
			return *this;
		}
		if (node->left)
		{
			node = node->left;
			while (node->right)
				node = node->right;
		}
		else
		{
			while (node->parent and node->parent->left == node)
				node = node->parent;
			node = node->parent;
		}
		return *this;
	}
	RBTreeConstIterator	operator--(int) {
		RBTreeConstIterator tmp(*this);
		--(*this);
		return tmp;
	}

	friend bool operator==(
			RBTreeConstIterator lhs,
			RBTreeConstIterator rhs)
	{
		return lhs.node == rhs.node;
	}

	friend bool operator!=(
			RBTreeConstIterator lhs,
			RBTreeConstIterator rhs)
	{
		return lhs.node != rhs.node;
	}
};




template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::iterator RBTree<T, Compare, Allocator>::insert(
		typename RBTree<T, Compare, Allocator>::iterator hint,
		const T& value )
{
	// what about hint?
	ft::pair<Node *, bool>	tmp = _insert_to_tree(&__root, value);
	return iterator(tmp.first, __root);
}

template <class T, class Compare, class Allocator>
ft::pair<typename RBTree<T, Compare, Allocator>::iterator, bool>
RBTree<T, Compare, Allocator>::insert( const T& value )
{
	ft::pair<Node *, bool>	tmp = _insert_to_tree(&__root, value);
	return ft::make_pair(iterator(tmp.first, __root), tmp.second);
}

template <class T, class Compare, class Allocator>
template< class InputIt >
void	RBTree<T, Compare, Allocator>::insert( InputIt first, InputIt last )
{
	for (InputIt it = first; it != last; ++it)
		insert(*it);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::iterator
RBTree<T, Compare, Allocator>::find( const T& key )
{
	return iterator(_find_node(key), __root);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::const_iterator
RBTree<T, Compare, Allocator>::find( const T& key ) const
{
	return const_iterator(_find_node(key), __root);
}


template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::erase( typename RBTree<T, Compare, Allocator>::iterator pos )
{
	_erase_node(pos.node);
}

template <class T, class Compare, class Allocator>
void	RBTree<T, Compare, Allocator>::erase( iterator first, iterator last )
{
	for (; first != last; )
		_erase_node((first++).node);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::size_type	RBTree<T, Compare, Allocator>::erase( const value_type& key )
{
	Node	*node = _find_node(key);
	if (node)
	{
		_erase_node(node);
		return (1);
	}
	return (0);
}


template <class T, class Compare, class Allocator>
ft::pair<
		typename RBTree<T, Compare, Allocator>::iterator,
		typename RBTree<T, Compare, Allocator>::iterator
>	RBTree<T, Compare, Allocator>::equal_range( const T& key )
{
	return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
}

template <class T, class Compare, class Allocator>
ft::pair<
		typename RBTree<T, Compare, Allocator>::const_iterator,
		typename RBTree<T, Compare, Allocator>::const_iterator
>	RBTree<T, Compare, Allocator>::equal_range( const T& key ) const
{
	return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::iterator	RBTree<T, Compare, Allocator>::lower_bound( const T& key )
{
	// iterator beg = this->begin();
	// iterator end = this->end();

	// while (beg != end)
	// {
	// 	if (__comp((*beg), key) == false)
	// 		break;
	// 	beg++;
	// }
	// return (beg);
	Node *curr = __root;
	Node *res = NULL;

	while (curr)
	{
		if (__comp(key, curr->value))
		{
			res = curr;
			curr = curr->left;
		} 
		else if (not __comp(curr->value, key)) // equals
		{
			res = curr;
			curr = curr->left;
		}
		else
			curr = curr->right;
	}
	return iterator(res, __root);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::const_iterator	RBTree<T, Compare, Allocator>::lower_bound( const T& key ) const
{
	Node *curr = __root;
	Node *res = NULL;

	while (curr)
	{
		if (__comp(key, curr->value))
		{
			res = curr;
			curr = curr->left;
		} 
		else if (not __comp(curr->value, key)) // equals
		{
			res = curr;
			curr = curr->left;
		}
		else
			curr = curr->right;
	}
	return const_iterator(res, __root);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::iterator	RBTree<T, Compare, Allocator>::upper_bound( const T& key )
{
	Node *curr = __root;
	Node *res = NULL;

	while (curr)
	{
		if (__comp(key, curr->value))
		{
			res = curr;
			curr = curr->left;
		}
		else
			curr = curr->right;
	}
	return iterator(res, __root);
}

template <class T, class Compare, class Allocator>
typename RBTree<T, Compare, Allocator>::const_iterator	RBTree<T, Compare, Allocator>::upper_bound( const T& key ) const
{
	Node *curr = __root;
	Node *res = NULL;

	while (curr)
	{
		if (__comp(key, curr->value))
		{
			res = curr;
			curr = curr->left;
		}
		else
			curr = curr->right;
	}
	return const_iterator(res, __root);
}



} // namespace ft

#endif