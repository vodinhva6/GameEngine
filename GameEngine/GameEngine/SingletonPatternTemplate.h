#pragma once
template <class T>
class SingletonPatternTemplate
{
public:
	static T* get()
	{
		if (instance_ == nullptr)
			instance_ = new T;
		return instance_;
	}

private:
	SingletonPatternTemplate(SingletonPatternTemplate const&) {};
	SingletonPatternTemplate& operator=(SingletonPatternTemplate const&) {};
protected:
	SingletonPatternTemplate() {}
	static T* instance_;
};
template<typename T>
typename T* SingletonPatternTemplate<T>::instance_ = 0;