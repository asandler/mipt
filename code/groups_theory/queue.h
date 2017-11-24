#pragma once

template<class T>
class Queue
{
	public:
		Queue(size_t);
		void push(const T&);
		T pop();
		bool empty() const;
		T front() const;
		T back() const;
	private:
		size_t start;
		size_t length;
		size_t n;
		T *data;
};

template<class T>
Queue<T>::Queue(size_t _n)
{
	n = _n;
	data = new T[n];
	start = 0;
	length = 0;
}

template<class T>
void Queue<T>::push(const T& v)
{
	data[start + length++] = v;
}

template<class T>
T Queue<T>::pop()
{
	if (empty())
	{
		throw "";
	}
	--length;
	return data[start++];
}

template<class T>
bool Queue<T>::empty() const
{
	return length == 0;
}

template<class T>
T Queue<T>::front() const
{
	if (empty())
	{
		throw "";
	}
	return data[start];
}

template<class T>
T Queue<T>::back() const
{
	if (empty())
	{
		throw "";
	}
	return data[start + length - 1];
}
