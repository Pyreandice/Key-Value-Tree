#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace nwacc
{
	template<typename T, typename K>
	class avl_tree
	{
	private:
		/**
		 * Determines white space for printing.
		 */
		int indent = 0;

		/**
		 * Represents the number of items in the tree.
		 */
		int tree_size = 0;

		/**
		 * Construct the node with all of the necessary components.
		 * Such as the key, value, left and right values and the height.
		 * @param element
		 * @param key
		 * @param left
		 * @param right
		 * @param height
		 * @reutrn the completed node struct
		 */
		struct node
		{
			T element;
			K key;
			node *parent;
			node *left;
			node *right;
			int height;

			node(const T &the_element, const K &the_key, node *the_parent, node *the_left, node *the_right, int the_height = 0)
				: element{ the_element }, key{ the_key }, parent{ the_parent }, left{ the_left },
				right{ the_right }, height{ the_height } {}

			node(const T && the_element, const K &&the_key, node *the_parent, node *the_left, node *the_right, int the_height = 0)
				: element{ std::move(the_element) }, key{ the_key }, parent{ the_parent }, left{ the_left },
				right{ the_right }, height{ the_height } {}
		};

		/**
		 * The root value of the tree.
		 * @value root
		 */
		node *root;

	public:

		/**
		 * Create the root with a null pointer.
		 */
		avl_tree() : root { nullptr } {}
				
		/**
		 * Create a clone of the right hand side of the root with a null pointer.
		 * @return true if the clone of the rhs of the root is a null pointer.
		 */
		avl_tree(const avl_tree &rhs) : root{ nullptr }
		{
			this->root = this->clone(rhs.root);
		}

		/**
		 * Create the right hand side of the root with a null pointer.
		 * @return true if the rhs of the root is equal to a null pointer.
		 */
		avl_tree(avl_tree &&rhs) : root { rhs.root }
		{
			rhs.root = nullptr;
		}

		/**
		 * Destruct the empty constructor.
		 */
		~avl_tree()
		{
			this->empty();
		}

		/**
		 * 
		 */
		avl_tree &operator=(const avl_tree &rhs)
		{
			auto copy = rhs;
			std::swap(*this, copy);
			return *this;
		}

		/**
		 *
		 */
		avl_tree &operator=(avl_tree &&rhs)
		{
			std::swap(this->root, rhs.root);
			return *this;
		}

		/**
		 * Determine if the root is equal to a null pointer.
		 * @return true if the root is equal to a null pointer.
		 */
		bool is_empty() const
		{
			return this->root == nullptr;
		}

		/**
		 * Set the root equal to empty.
		 */
		void empty()
		{
			this->empty(this->root);
		}

		/**
		 * Determine if the root contains data.
		 * @return true if the root contains data.
		 */
		bool contains(const T &value) const
		{
			return this->contains(value, this->root);
		}

		/**
		 * Remove the value at the root.
		 * @param value
		 */
		void remove(const T &value)
		{
			this->remove(value, this->root);
		}

		/**
		 * Get the value associated with a key.
		 * If the key does not exist in the tree throw an exception.
		 * @param key
		 */
		T get(const K &key) const
		{
			return this->get(key, this->root);
		}

#pragma region const_iterator
		class const_iterator
		{
		public:

			/**
			 * Construct the const_iterator iterator with a null pointer.
			 * @return const_iterator
			 */
			const_iterator() : current{ nullptr } {}

			/**
			 * Overload the pointer operator.
			 * @return const_iterator
			 */
			const T &operator*() const
			{
				return this->retrieve();
			}

			/**
			 * prefix ++ operator
			 * @return const_iterator
			 */
			const_iterator &operator++()
			{
				if (this->current->right != nullptr)
				{
					this->current = this->find_first(this->current->right);
				}
				else
				{
					while (this->current->parent != nullptr && this->current == this->current->parent->right)
					{
						this->current->right = this->current->parent;
					}
					this->current = this->current->parent;
				}
				return *this;
			}

			/**
			 * This is the postfix operator.
			 * @return const_iterator
			 */
			const_iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			/**
			 * This is the prefix operator.
			 * @return const_iterator
			 */
			const_iterator &operator--()
			{
				if (this->current->left != nullptr)
				{
					this->current = this->find_max(this->current->left);
				}
				else
				{
					while (this->current->parent != nullptr && this->current == this->current->parent->left)
					{
						this->current->left = this->current->parent;
					}
					this->current = this->current->parent;
				}
				return *this;
			}

			/**
			 * This is the postfix operator.
			 * @return const_iterator
			 */
			const_iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * of the current node are equal to each other.
			 * @return const_iterator
			 */
			bool operator== (const const_iterator &rhs) const
			{
				return this->current == rhs.current;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * node is not equal to each other.
			 * @return const_iterator
			 */
			bool operator!= (const const_iterator &rhs) const
			{
				return !(*this == rhs);
			}

			/**
			* Find the left most node in the tree. This should represent the
			* lowest value in the tree.
			* @param current
			*/
			node *find_first(node *current)
			{
				if (current == nullptr)
				{
					return nullptr;
				} // else, current is not null, do_nothing();

				if (current->left == nullptr)
				{
					return current;
				} // else, we still have more children do_nothing();

				return this->find_first(current->left);
			}

			/**
			 * Find the right most node in the tree. This should represent the
			 * largest value in the tree.
			 * @param current
			 */
			node *find_last(node *current)
			{
				if (current == nullptr)
				{
					return nullptr;
				} // else, current is not null, do_nothing();

				if (current->right == nullptr)
				{
					return current;
				} // else, we still have children do_nothing();

				return this->find_last(current->right);
			}

			/**
			 * Finds the next node in the tree set.
			 * If the right child of the current node is equal to a null pointer,
			 * then return the value of the current node as the smallest value in the tree set.
			 * Else the current node is a child. Move up the tree.
			 * @param current
			 */
			node *find_next_node(node *current) const
			{
				if (current->right != nullptr)
				{
					return find_min(current->right);
				}
				else
				{
					while (current->parent != nullptr && current == current->parent->right)
					{
						current = current->parent;
					}
					return current->parent;
				}
			}

		protected:
			node *current;

			/**
			 * Retrieve the data that is stored in the current node.
			 * @return const_iterator
			 */
			T &retrieve() const
			{
				return this->current->element;
			}

			/**
			 * Construct the const_iterator iterator with the value at current.
			 * @return the current data
			 */
			const_iterator(node *current) : current{ current } {}

			friend class avl_tree<T, K>;
		};
#pragma endregion
#pragma region iterator
		class iterator : public const_iterator
		{
		public:
			/**
			 * Construct the iterator iterator with a null pointer.
			 * @return iterator
			 */
			iterator() : current{ nullptr } {}

			/**
			 * Overload the pointer operator.
			 * @return iterator
			 */
			T &operator*()
			{
				return this->retrieve();
			}

			/**
			 * Overload the ++ prefix operator.
			 * @return iterator
			 */
			iterator &operator++()
			{
				if (this->current->right != nullptr)
				{
					this->current = this->find_first(this->current->right);
				}
				else
				{
					while (this->current->parent != nullptr && this->current == this->current->parent->right)
					{
						this->current->right = this->current->parent;
					}
					this->current = this->current->parent;
				}
				return *this;
			}

			/**
			* This is the ++ postfix operator.
			* @return iterator
			*/
			iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			/**
			 * Overload the prefix -- operator.
			 * @return iterator
			 */
			iterator &operator--()
			{
				if (this->current->left != nullptr)
				{
					this->current = this->find_last(this->current->left);
				}
				else
				{
					while (this->current->parent != nullptr && this->current == this->current->parent->left)
					{
						this->current->left = this->current->parent;
					}
					this->current = this->current->parent;
				}
				return *this;
			}

			/**
			 * Overload the postfix -- operator.
			 * @return iterator
			 */
			iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			/**
			* Return the comparison of the current node vs the rhs
			* of the current node are equal to each other.
			* @return iterator
			*/
			bool operator== (const iterator &rhs)
			{
				return this->current == rhs.current;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * node is not equal to each other.
			 */
			bool operator!= (const iterator &rhs)
			{
				return !(*this == rhs);
			}

			/**
			* Find the left most node in the tree. This should represent the
			* lowest value in the tree.
			* @param current
			*/
			node *find_first(node *current)
			{
				if (current == nullptr)
				{
					return nullptr;
				} // else, current is not null, do_nothing();

				if (current->left == nullptr)
				{
					return current;
				} // else, we still have more children do_nothing();

				return this->find_first(current->left);
			}

			/**
			 * Find the right most node in the tree. This should represent the
			 * largest value in the tree.
			 * @param current
			 */
			node *find_last(node *current)
			{
				if (current == nullptr)
				{
					return nullptr;
				} // else, current is not null, do_nothing();

				if (current->right == nullptr)
				{
					return current;
				} // else, we still have children do_nothing();

				return this->find_last(current->right);
			}

			/**
			 * Finds the next node in the tree set.
			 * If the right child of the current node is equal to a null pointer,
			 * then return the value of the current node as the smallest value in the tree set.
			 * Else the current node is a child. Move up the tree.
			 * @param current
			 */
			node *find_next_node(node *current)
			{
				if (current->right != nullptr)
				{
					return find_first(current->right);
				}
				else
				{
					while (current->parent != nullptr && current == current->parent->right)
					{
						current = current->parent;
					}
					return current->parent;
				}
			}

			/**
			* Insert a value at the root.
			* @param value
			* @param key
			*/
			iterator insert(const T &value, const K &key)
			{
				return this->insert(value, nullptr, key, this->root);
			}

			/**
			 * Insert a value at the root with move semantics.
			 * @param value
			 * @param key
			 */
			iterator insert(T &&value, const K &&key)
			{
				return this->insert(std::move(value), nullptr, std::move(key), this->root);
			}

			/**
			* Return the current key.
			* @param key
			* @return current key.
			*/
		    K& get_key()
			{
				return current->key;
			}

		private:
			node *current;

			/**
			 * Retrieve the data stored within the current node.
			 * @return iterator
			 */
			T &retrieve()
			{
				return this->current->element;
			}

			/**
			 * Construct the iterator iterator with the data at the current node.
			 * @return iterator
			 */
			iterator(node *current) : current{ current } {}

			friend class avl_tree<T, K>;
			friend class const_iterator;
		};
#pragma endregion

	private:
		/** 
		* Finds the lowest valued key value pair.
	    * @param current
		* @return the lowest value in the tree.
		*/
		node *find_min(node *current) const // last
		{
			if (current == nullptr)
			{
				return nullptr;
			} // else, we are at the end, do_nothing();

			if (current->left == nullptr)
			{
				return current;
			} // else, we have found the smallest, do_nothing();

			return this->find_min(current->left);
		}

		/**
		 * Finds the highest valued key value pair.
		 * @param current
		 * @return the maximum value in the tree.
		 */
		node *find_max(node *current) const // first
		{
			if (current != nullptr)
			{
				while (current->right != nullptr)
				{
					current = current->right;
				}
			} // else, do_nothing();
			return current;
		}

	public:
		iterator first_element() const
		{
			return iterator(find_min(this->root));
		}

		iterator last_element() const
		{
			return iterator(find_max(this->root));
		}

		iterator begin() const
		{
			return iterator(nullptr);
		}

		iterator end() const
		{
			return iterator(nullptr);
		}

		/**
		* Insert a value at the root.
		* @param value
		* @param key
		*/
		iterator insert(const T &value, const K &key)
		{
			return this->insert(value, key, nullptr, this->root);
		}

		/**
		 * Insert a value at the root with move semantics.
		 * @param value
		 * @param key
		 */
		void insert(T &&value, const K &&key)
		{
			this->insert(std::move(value), std::move(key), nullptr, this->root);
		}

		T &operator[](K key)
		{
			return this->operator[](key, this->root);
		}

		/**
		 * Overload the ostream operator to print the tree
		 * forwards and backwards.
		 * @param out the value to be printed to the console.
		 * @param rhs the right hand side
		 */
		friend std::ostream &operator<<(std::ostream &out, const avl_tree &rhs)
		{
			for (iterator item = rhs.first_element(); item != rhs.end(); item++)
			{
				out << item.get_key() << std::endl; // TODO: make print key
			}

			for (iterator item = rhs.last_element(); item != rhs.begin(); item--)
			{
				out << item.get_key() << std::endl; // TODO: make print key
			}
			return out;
		}

		void diplay_tree()
		{
			this->postorder(this->root);
		}

	private:

		void postorder(node* p, int indent=0)
		{
			if (p != nullptr) {
				if (p->right) {
					postorder(p->right, indent + 4);
				}
				if (indent) {
					std::cout << std::setw(indent) << ' ';
				}
				if (p->right) std::cout << " /\n" << std::setw(indent) << ' ';
				if (p->parent != nullptr) {
					std::cout << p->key << "--" << p->parent->key << "\n ";
				}else
				{
					std::cout << p->key << "--" << "null parent" << "\n ";
				}
				if (p->left) {
					std::cout << std::setw(indent) << ' ' << " \\\n";
					postorder(p->left, indent + 4);
				}
			}
		}

		/**
		 * Check to see if the current key contains a nullptr.
		 * @param current
		 */
		void empty(node *&current)
		{
			if (current != nullptr)
			{
				this->empty(current->left);
				this->empty(current->right);
				delete current;
			} // else, current is null, do_nothing();

			current = nullptr;
		}

		/**
		 * Cone the current node and all of its necessary components.
		 * @param current
		 * @return a copy of the current node.
		 */
		node *clone(node *current) const
		{
			if (current == nullptr)
				return nullptr;
			else
				return new node{ current->element, clone->key, clone->parent,
								 clone(current->left), clone(current->right), 
								 current->height };
		}

		/**
		 * Insert a key, value, parent, left, right, and height
		 * into the current node.
		 * @param value
		 * @param parent
		 * @param key
		 * @param current
		 * @return iterator
		 */
		iterator insert(const T &value, const K &key, node *parent,  node *&current)
		{
			if (current == nullptr)
			{
				if (parent == nullptr)
				{
					current = new node{ value, key, nullptr, nullptr, nullptr };
					tree_size += 1;
					root = current;
					return iterator(current);
				}
				else
				{
					current = new node{ value, key, parent, nullptr, nullptr };
					tree_size += 1;
					return iterator(current);
				}
			}
			else if (key < current->key)
			{
				this->insert(value, key, parent, current->left);
			}
			else if (current->key < value)
			{
				this->insert(value, key, parent, current->right);
			}
			else
			{
				current->element = value;
			}

			tree_size += 1;
			this->balance(current);
			return iterator(current);
		}

		/**
		 * Insert a value, key, parent, left, right, and height
		 * into the current node using move semantics.
		 * @param value
		 * @param key
		 * @param parent
		 * @param current
		 * @return iterator
		 */
		void insert(const T &&value, const K &&key, node *parent, node *&current)
		{
			if (current == nullptr)
			{
				if (parent == nullptr)
				{
					current = new node{ std::move(value), std::move(key), nullptr, nullptr, nullptr };
					tree_size += 1;
					root = current;
					//return iterator(current);
				}
				else
				{
					current = new node{ std::move(value), std::move(key), parent, nullptr, nullptr };
					tree_size += 1;
					//return iterator(current);
				}
			}
			else if (key < current->key)
			{
				this->insert(std::move(value), std::move(key), current, current->left);
			}
			else if (current->key < key)
			{
				this->insert(std::move(value), std::move(key), current, current->right);
			}
			else
			{
				current->element = value;
			}

			this->balance(current);
			//return iterator(current);
		}

		/**
		 * Remove the value referenced by the key.
		 * This DOES NOT delete the KEY.
		 * ONLY the VALUE inside the KEY.
		 * @param key the current key
		 * @param current the current value
		 */
		void remove(const T &key, node *&current)
		{
			if (current == nullptr)
			{
				// we did not find the item to remove. 
				return; 
			} // else, we found the item do_nothing();

			if (key < current->key)
			{
				this->remove(key, current->key->left);
			}
			else if (current->key < key)
			{
				this->remove(key, current->key->right);
			}
			else if (current->left != nullptr && current->right != nullptr)
			{
				// here we have two children
				current->key = this->find_min(current->right)->key;
				this->remove(current->key, current->right);
			}
			else
			{
				// here we have no children :( or one child. 
				node *old_node = current;
				current = (current->left != nullptr) ? current->left : current->right;
				delete old_node;
			}

			this->balance(current);
		}

		/**
		 * Determine if the current key is not a null pointer.
		 * @param key
		 * @param current
		 * @return true if the current key is not a null pointer.
		 */
		bool contains_key(K key, node *current)
		{
			if (current == nullptr)
			{
				return false;
			}
			else if (key < current->key)
			{
				return this->contains_key(key, current->left);
			}
			else if (current->key < key)
			{
				return this->contains_key(key, current->right);
			}
			else
			{
				return true;
			}
		}

		/**
		 * Determine if the current value is not a null pointer.
		 * @param value
		 * @param current
		 * @reutrn true if the current value is not a null pointer.
		 */
		bool contains_value(T value, node *current)
		{
			if (current == nullptr)
			{
				return false;
			}
			else if (value < current->element)
			{
				return this->contains_value (value, current->left);
			}
			else if (current->element < value)
			{
				return this->contains_value(value, current->right);
			}
			else
			{
				return true;
			}
		}

		/**
		 * Get the value associated with a key.
		 * If the key does not exist in the tree throw an exception.
		 * @param key
		 * @param current
		 */
		T &get(K key, node *&current)
		{
			if (current == nullptr)
			{
				throw std::length_error("Data not Found....");
			}
			else if (key < current->key)
			{
				return this->get(key, current->left);
			}
			else if (current->key < key)
			{
				return this->get(key, current->right);
			}
			return current->element;
		}

		/**
		 * Private facing subscript operator overload
		 * @param key
		 * @param current
		 * @return the current node.
		 */
		T &subscript_overload(K key, node *&current)
		{
			if (current == nullptr)
			{
				return{};
			}
			else if (key < current->key)
			{
				this->operator[](tree_size, current->left);
			}
			else if (current->key < key)
			{
				this->operator[](tree_size, current->right);
			}
			return current->element;
		}

		/**
		 * Finds the height of the current tree.
		 * @param current
		 * @return the height of the current tree.
		 */
		int height(node *current) const
		{
			return current == nullptr ? -1 : current->height;
		}

		/**
		 * Rotate binary tree node with left child. This is a 
		 * single rotation.
		 * @param current 
		 */
		void rotate_with_left_child(node *&current)
		{
			auto *temp = current->left;
			current->left = temp->right; // current->left->right;
			temp->right = current;
			temp->parent = current->parent;
			current->parent = temp;

			if (current->right != nullptr && current->right->parent != current)
			{
				current->right->parent = current;
			} // else, do_nothing();
			if (current->left != nullptr && current->left->parent != current)
			{
				current->left->parent = current;
			} // else, do_nothing();

			current->height = std::max(this->height(current->left), this->height(current->right)) + 1;
			temp->height = std::max(this->height(temp->left), current->height) + 1;
			
			current = temp;
		}

		/**
		 * Rotate binary tree node with the right child. This is a
		 * single rotation.
		 * @param current
		 */
		void rotate_with_right_child(node *&current)
		{
			auto *temp = current->right;
			current->right = temp->left;
			temp->left = current;
			temp->parent = current->parent;
			current->parent = temp;

			if(current->right != nullptr && current->right->parent != current)
			{
				current->right->parent = current;
			} // else, do_nothing();

			if (current->left != nullptr && current->left->parent != current)
			{
				current->left->parent = current;
			} // else, do_nothing();

			current->height = std::max(this->height(current->left), this->height(current->right)) + 1;
			temp->height = std::max(this->height(temp->right), current->height) + 1;

			current = temp;
		}

		/**
		 * Double rotate binary tree node - first rotate the left child
		 * with its right child; then with the new left child. 
		 * @param current
		 */
		void double_rotate_with_left_child(node *&current)
		{
			this->rotate_with_right_child(current->left);
			this->rotate_with_left_child(current);
		}

		/**
		 * Double rotate binary tree node - first rotate with right child
		 * with its left child; then with the new left child.
		 * @param current
		 */
		void double_rotate_with_right_child(node *&current)
		{
			this->rotate_with_left_child(current->right);
			this->rotate_with_right_child(current);
		}

		/**
		 * Balance the tree "branches", effectively making them
		 * the same height as one another.
		 * @param current
		 */
		void balance(node *&current)
		{
			if (current == nullptr)
			{
				return;
			} // else, we have a valid node do_nothing();

			if (this->height(current->left) - 
				this->height(current->right) > 1)
			{
				// left side has a greater height
				if (this->height(current->left->left) >=
					this->height(current->left->right))
				{
					this->rotate_with_left_child(current);
				}
				else
				{
					this->double_rotate_with_left_child(current);
				}
			}
			else if (this->height(current->right) - 
				     this->height(current->left) > 1) 
			{
				// right side has a greater height
				if (this->height(current->right->right) >= 
					this->height(current->right->left))
				{
					this->rotate_with_right_child(current);
				}
				else
				{
					this->double_rotate_with_right_child(current);
				}
			} // else, the nodes are balanced within 1, do_nothing();

			current->height = std::max(
				this->height(current->left),
				this->height(current->right)) + 1;
		}
	};
}

#endif // AVL_TREE_H_