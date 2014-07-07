#pragma once

template <class T>
class ISingleton
{
public:
	virtual ~ISingleton()
	{
		CleanUp();
	}

	static void CleanUp()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	static T* Get()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T;
		}
		return mInstance;
	}

private:
	static T* mInstance;
};

template <class T> T* ISingleton<T>::mInstance = nullptr;