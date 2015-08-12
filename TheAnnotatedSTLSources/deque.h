#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "Iterator.h"
#include "ReverseIterator.h"
#include "Utility.h"
#include "allocator.h"

namespace Jinl{

	template <class T,class Alloc=allocator<T>>
	class deque;

	/*定义Deque的迭代器，必须能够指出分段连续空间在哪里，
	 *必须能够判断自己是否已经处于其所在缓冲区的边缘。如果是，
	 一旦前进或后退时就必须跳跃至下一个或上一个缓冲区。*/
	template <class T>
	class deque_iterator :public Iterator<BidirectionIterator_tag, T>{
	private:
		template<class T,class Alloc>
		friend class ::Jinl::deque;

		typedef const ::Jinl::deque<T>* cntrPtr;
		size_t mapIndex_;
		T* cur_;
		cntrPtr container_;
	public:
		deque_iterator() :mapIndex_(-1), cur_(nullptr), container_(nullptr)
		{

		}

		deque_iterator(size_t index, T* ptr, cntrPtr cter) :
			mapIndex_(index), cur_(ptr), container_(cter)
		{

		}
		deque_iterator(const deque_iterator& it) :
			mapIndex_(it.mapIndex_), cur_(it.cur_), container_(it.container)
		{

		}

		deque_iterator& operator=(const deque_iterator& x)
		{
			this->mapIndex_ = x.mapIndex_;
			this->cur_ = x.cur_;
			this->container_ = x.container_;
			return *this;
		}

		void swap(deque_iterator& it);

		reference operator*(){ return *cur_; }
		const reference operator*() const { return *cur_; }

		pointer operator->(){ return &(operator*()); }
		const pointer operator->()const{ return &(operator*()); }

		deque_iterator& operator++();
		deque_iterator  operator++(int);

		deque_iterator& operaot--();
		deque_iterator  operator(int); 

		bool operator==(const deque_iterator& it) const;
		bool operator!=(const deque_iterator& it) const;

		T* getBuckTail(size_t mapIndex) const;
		T* getBuckHead(size_t mapIndex) const;
		size_t getBuckSize() const;

	public:
		template<class T>
		deque_iterator<T> operator+(const deque_iterator<T>& it, typename deque_iterator<T>::difference_type n);
		template<class T>
		deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n, const deque_iterator<T>& it);

		template<class T>
		deque_iterator<T> operator-(const deque_iterator<T>& it, typename deque_iterator<T>::difference_type n);
		template<class T>
		deque_iterator<T> operator-(typename deque_iterator<T>::difference_type n, const deque_iterator<T>& it);

		template <class T>
		typename deque_iterator<T>::difference_type operator-(const deque_iterator<T>& it1, const deque_iterator<T>& it2);

		template <class T>
		void swap(deque_iterator<T>& it1, deque_iterator<T>& it2);
	
	};

	template <class T,class Alloc>
	class deque{
	private:
		template<class T>
		class ::Jinl::deque_iterator;

	public:

		typedef T                       value_type;
		typedef size_t                  size_type;
		typedef Alloc                   allocator_type;
		typedef T*                      pointer;
		typedef ptrdiff_t               difference_type;
		typedef T&                      reference;
		typedef const reference         const_reference;
		typedef deque_iterator<T>       iterator;
		typedef deque_iterator<const T> const_iterator;
	private:
		typedef Alloc dataAllocator;
		enum class EBucksSize{BUCKSIZE = 64};
		
	private:
		iterator beg_,end_;
		size_type mapSize_;
		T** map_;

	public:
		deque() :mapSize_(0), map_(nullptr){}
		explicit deque(size_type n, const value_type& val = value_type());
		template <class InputIterator>
		deque(InputIterator first, InputIterator last);
		deque(const deque& x);
		
		//赋值拷贝和移动拷贝
		deque& operator=(const deque& x);
		deque& operator=(deque&& x);

		~deque();
		
		iterator begin(){ return first_; }
		iterator end(){ return last_; }
		iterator begin() const { return first_; }
		iterator end()const { return last; }

	public:
		size_type size() const { return end() - begin(); }
		bool empty() const { return begin() == end(); }

		reference operator[](size_type n);
		reference front();
		reference back();
		const_reference operator[](size_type n) const;
		const_reference front() const;
		const_reference back() const;

		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back();
		void pop_front();
		void swap(deque& x);
		void clear();

	private:

		T* getANewBuck();
		T** getANewMap(const size_t size);
		size_t getANewMapSize(const size_t size);
		size_t getBuckSize() const; 
		
		void init();
		bool back_full()const;
		bool front_full() const;
		void deque_aux(size_t n, const value_type& val, std::true_type);
		template<class Iterator>
		void deque_aux(Iterator first, Iterator last, std::false_type);
		void reallocateAndCopy();

	public:

		template<class T,class Alloc>
		bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

		template<class T,class Alloc>
		bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

		template<class T,class Alloc>
		void swap(deque<T, Alloc>& x, deque<T, Alloc>& y);

	};

}



#endif