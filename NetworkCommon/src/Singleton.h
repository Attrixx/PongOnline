#pragma once

#define I(className) className::GetInstance()

template <typename T>
class Singleton
{
protected:
	Singleton();
	virtual ~Singleton();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T* sInstance;

public:

	static T* GetInstance();

};

template<typename T>
T* Singleton<T>::sInstance = nullptr;

template<typename T>
inline Singleton<T>::Singleton()
{
}

template<typename T>
inline Singleton<T>::~Singleton()
{
}

template<typename T>
inline T* Singleton<T>::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new T();
	}
	return sInstance;
}