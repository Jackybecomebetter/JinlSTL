#include "deque.h"

namespace Jinl{

	template <class T>
	void deque_iterator<T>::swap(deque_iterator& it)
	{
		swap(mapIndex_, it.mapIndex_);
		swap(cur_, it.cur_);
	}

	template <class T>
	void swap(deque_iterator<T>& lhs, deque_iterator<T>& rhs)
	{
		lhs.swap(rhs);
	}

	template <class T>
	deque_iterator<T>& deque_iterator<T>::operator++(){
		if (cur_ != getBuckTail(mapIndex_))
			++cur_;
		else if (mapIndex_ + 1 < container_->mapSize_){
			++mapSize_;
			cur_ = getBuckHead(mapIndex_);
		}
		else{
			mapIndex_ = container_->mapSize_;
			cur_ = container_->map_[mapIndex_];
		}
		return *this;
	}

	template <class T>
	deque_iterator<T>& deque_iterator<T>::operator++(int){
		auto res = *this;
		++(*this);
		return res;
	}
	template<class T>
	deque_iterator<T>& deque_iterator<T>::operator--(){
		if (cur_ != getBuckHead(mapIndex_))
			--cur_;
		else if (mapIndex_ - 1 >= 0){
			--mapIndex_;
			cur_ = getBuckTail(mapIndex_);
		}
		else {
			mapIndex_ = 0;
			cur_ = container_->map_[mapIndex_];
		}
		return *this;
	}

	template<class T>
	deque_iterator<T>& deque_iterator<T>::operator--(int)
	{
		auto res = *this;
		--(*this);
		return res;
	}

	template<class T>
	bool deque_iterator<T>::operator==(const deque_iterator<T>& it) const {
		return ((mapSize_ == it.mapIndex_) && (cur_ == it.cur_) && (container_ == it.container_));
	}

	template<class T>
	bool deque_iterator<T>::operator!=(const deque_iterator<T>& it) const{
		return !(*this == it);
	}

	template<class T>
	deque_iterator<T>& deque_iterator<T>::operator= (const deque_iterator<T>& it){
		if (*this != it){
			this->mapIndex_ = it.mapIndex_;
			this->cur_ = it.cur_;
			this->container_ = it.container_;
		}
		return *this;
	}

	template <class T>
	T* deque_iterator<T>::getBuckHead(size_t mapIndex) const {
		return container_->map_[mapIndex_];
	}

	template <class T>
	T* deque_iterator<T>::getBuckTail(size_t mapIndex) const {
		return container_->map_[mapIndex_] + (container_->getBuckSize() - 1);
	}

	template<class T>
	size_t deque_iterator<T>::getBuckSize()const{
		return container_->getBuckSize();
	}

	template <class T>
	deque_iterator<T> operator+(const deque_iterator<T>& it, typename deque_iterator<T>::difference_type n){
		deque_iterator<T> res(it);
		auto m = res.getBuckTail(res.mapIndex_) - res.cur_;
		if (n <= m){
			return res.cur_ + n;
		}
		else{
			n = n - m;
			res.mapIndex_ += (n / it.getBuckSize() + 1);
			auto p = res.getBuckHead(res.mapIndex_);
			auto x = n%it.getBuckSize() - 1;
			res.cur_ = p + x;
		}
		return res;
	}
	template <class T>
	deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n, const deque_iterator<T>& it){
		return it + n;
	}

	template <class T>
	deque_iterator operator-(const deque_iterator<T>& it, typename deque_iterator<T>::difference_type n){
		deque_iterator<T> res(it);
		auto m = res.cur_ - res.getBuckHead(res.mapIndex_);
		if (n <= m)
			res.cur_ -= n;
		else{
			n = n - m;
			res.mapIndex_ -= (n / res.getBuckSize() + 1);
			res.cur_ = res.getBuckTail(res.mapIndex_) - (n%res.getBuckSize() - 1);
		}
		return res;
	}
	template <class T>
	deque_iterator operator-(typename deque_iterator<T>::difference_type n, const deque_iterator<T>& it){
		return it - n;
	}

	template <class T>
	typename deque_iterator<T>::difference_type operator-(const deque_iterator<T>& it1, const deque_iterator<T>& it2)
	{
		if (it1.container_ == it2.container_ && it1.container_ == 0)
			return 0;
		return typename deque_iterator<T>::difference_type(it1.getBuckSize())*(it1.mapIndex_ - it2.mapIndex_ - 1)
			+ (it1.cur_ - it1.getBuckHead(it1.mapIndex_)) + (it2.getBuckTail(it2.mapIndex_) - it2.cur_) + 1;
	}

	template <class T>
	void swap(deque_iterator<T>& lhs, deque_iterator<T>& rhs){
		lhs.swap(rhs);
	}

	template<class T,class Alloc>
	bool deque<T, Alloc>::back_full() const {
		return map_[mapSize_ - 1] && (map_[mapSize_]) == end().cur_;
	}

	template<class T, class Alloc>
	bool deque<T, Alloc>::front_full() const {
		return map_[0] && map_[0] == begin().cur_;
	}

	template<class T, class Alloc>
	bool deque<T, Alloc>::deque_aux(size_t n, const value_type& val, std::true_type)
	{
		int i = 0;
		for (; i != n / 2; i++)
			(*this).push_front(val);
		for (; i != n; i++)
			(*this).push_back(val);
	}

	template<class T,class Alloc,class Iterator>
	bool deque<T, Alloc>::deque_aux(Iterator first, Iterator last, std::false_type)
	{
		difference_type mid = (last - first) / 2;
		for (auto it = first + mid; it != first - 1; --it)
			(*this).push_front(*it);
		for (auto it = first + mid + 1; it != last; ++it)
			(*this).push_back(*it);
	}

	template<class T,class Alloc>
	void deque<T>::init(){
		mapSize_ = 2;
		map_ = getANewMap(mapSize_);
		beg_.container_ = end_.container_ = this;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ - 1;
		beg_.cur_ = end_.cur_ = map_[mapSize_ - 1];
	}

	template <class T,class Alloc>
	T* deque<T, Alloc>::getANewBuck(){
		return dataAllocator::allocate(getBuckSize());
	}

	template<class T,class Alloc>
	T** deque<T, Alloc>::getANewMap(const size_t size) {
		T** map = new T*[size];
		for (int i = 0; i != size; i++)
			map[i] = getANewBuck();
		return map;
	}

	template<class T,class Alloc>
	size_t deque<T, Alloc>::getANewMapSize(const size_t size){
		return (size == 0 ? 2 : size * 2);
	}

	template<class T,class Alloc>
	size_t deque<T, Alloc>::getBuckSize()const{
		return size_t(EBucksSize)::BUCKSIZE;
	}

	template<class T,class Alloc>
	void deque<T, Alloc>::clear(){
		for (auto i = 0; i != mapSize_; i++){
			for (auto p = map_[i] + 0; !p&&p != map_[i] + getBuckSize(); p++)
				dataAllocator::destroy(p);
		}
		mapSize_ = 0;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ / 2;
		beg_.cur_ = end_.cur_ = map_[mapSize_ / 2];
	}

	template<class T,class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::operator[](size_type n){
		return*(begin() + n);
	}

	template <class T,class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::front (){
		return *begin();
	}
	
	template <class T,class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::back(){
		return *(end()-1);
	}

	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::operator[] (size_type n) const{
		return *(begin() + n);
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::front() const{
		return *begin();
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::back() const{
		return *(end() - 1);
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::begin(){ return beg_; }
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::end(){ return end_; }
	//迭代器设计失误，会使下面两个函数丧失const特性，
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::begin()const{ return beg_; }
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::end()const{ return end_; }

	template<class T,class Alloc>
	deque<T, Alloc>::~deque(){
		for (int i = 0; i != mapSize_; i++){
			for (auto p = map_[i] + 0; !p**p != map_[i] + getBuckSize(); ++p)
				dataAllocator::destroy(p);
			if (!map_[i])
				dataAllocator::deallocate(map_[i], getBuckSize());
		}

		delete[] map_;
	}

	template<class T,class Alloc>
	deque<T, Alloc>::deque() :mapSize_(0), map_(nullptr){}
	
	template<class T,class Alloc>
	deque<T, Alloc>::deque(size_type n, const value_type& val = value_type()){
		deque();
		deque_aux(n, val, typename std::is_integral<size_type>::type());
	}

	template<class T,class Alloc,class InputIterator>
	deque<T, Alloc>::deque(InputIterator first, InputIterator last){
		deque();
		deque_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template <class T,class Alloc>
	deque<T, Alloc>::deque(const deque& x){
		this->mapSize_ = x.mapSize_;
		this->map_ = getANewMap(mapSize_);
		for (int i = 0; i + x.beg_.mapIndex_ != x.mapSize_; ++i){
			for (int j = 0; j != getBuckSize(); ++j)
				map_[x.beg_.mapIndex_ + i][j] = x.map_[x.beg_.mapIndex_ + i][j];
			beg_.mapIndex_ = x.beg_.mapIndex_;
			end_.mapIndex_ = x.end_.mapIndex_;
			beg_.cur_ = map_[beg_.mapIndex_] + (x.beg_.cur_ - x.map_[x.beg_.mapIndex_]);
			end_.cur_ = map_[end_.mapIndex_] + (x.end_.cur_ - x.map_[x.end_.mapIndex_]);
			beg_.container_ = end.container_ = this;

		}
	}
	template<class T,class Alloc>
	void deque<T, Alloc>::reallocateAndCopy(){
		auto newMapSize = getANewMapSize(mapSize_);
		T** newMap = getANewMap(newMapSize);
		size_t startIndex = newMapSize / 4;
		for (int i = 0; i + beg_.mapIndex_ != mapSize_; ++i)
			for (int j = 0; j != getBuckSize(); j++)
				newMap[startIndex + i][j] = map_[beg_.mapIndex_ + i][j];

		size_t n = beg_.cur_ - map_[beg_.mapIndex_];
		auto size = this->size();
		auto b = beg_, e = end_;
		clear();
		mapSize_ = newMapSize;
		map = newMap;
		beg_ = iterator(startIndex, newMap[startIndex] + n, this);
		end_ = beg_ + size;
	}

	template<class T,class Alloc>
	void deque<T, Alloc>::push_back(const value_type& val){
		if (empty())
			init();
		else if (back_full())
			reallocateAndCopy();
		Jinl::construct(end_.cur_, val);
		++end_;
	}

	template<class T,class Alloc>
	void deque<T, Alloc>::push_front(const value_type& val){
		if (empty())
			init();
		else if (front_full())
			reallocateAndCopy();
		--beg_;
		Jinl::construct(beg_.cur_, val_);
	}

	template<class T,class Alloc>
	void deque<T, Alloc>::pop_back(){
		--end_;
		dataAllocator.destroy(end_.cur_);
	}
	template<class T,class Alloc>
	void deque<T, Alloc>::pop_front(){
		dataAllocator.destroy(beg_.cur_);
		++beg_;
	}

	template<class T,class Alloc>
	void deque<T, Alloc>::swap(deque<T, Alloc>& x){
		Jinl::swap(mapSize_, x.mapSize_);
		Jinl::swap(map_, x.map_);
		beg_.swap(x.beg_);
		end_.swap(x.end_);
	}

	template<class T,class Alloc>
	bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs){
		auto cit1 = lhs.begin(), cit2 = rhs.begin();
		for (; cit1 != lhs.end() && cit2 != rhs.end(); cit1++, cit2++){
			if (*cit1 != *cit2)
				return false;
		}
		if (cit1 == lhs.end() && cit2 == rhs.end())
			return true;
		return false;
	}

	template<class T,class Alloc>
	bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs){
		return !(lhs == rhs);
	}

	template<class T,class Alloc>
	void swap(deque_iterator<T>& lhs, deque_iterator<T>& rhs){
		lhs.swap(rhs);
	}

}