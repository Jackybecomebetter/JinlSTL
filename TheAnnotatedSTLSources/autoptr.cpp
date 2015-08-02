#ifndef _AUTOPTR_
#define _AUTOPTR_

template <class T>
class auto_ptr
{
private:
	T* pointee;
public:
	explicit auto_ptr(T* p = 0) :pointee(p)
	{

	}
	template <class U>
	auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release())
	{

	}
	T* release()
	{
		return this->pointee;
	}
	~auto_ptr(){
		delete pointee;
	}

	template<class U>
	auto_ptr<U>& operator=(auto_ptr<U>& rhs)
	{
		if (this != &rhs) reset(rhs.release());
		return *this;
	}

	T& operator*() const
	{
		return this->*pointee;
	}
	T* operator->() const{
		return pointee;
	}
	T* get() const{
		return this->pointee;
	}
};


#endif