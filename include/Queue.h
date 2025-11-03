#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

template <class T>
class TQueue
{
    T* data;
    int start;
    int end;
    int len;

public:
    TQueue();
    TQueue(int len_);
    TQueue(const TQueue& obj);
    TQueue(TQueue&& obj);
    ~TQueue();

    int GetLen() const;
    int GetStart() const;
    int GetEnd() const;
    int GetCount() const;

    void Resize(int newLen);

    void Push(T value);
    T Pop();
    bool IsEmpty() const;
    bool IsFull() const;
    T FindMin() const;

    TQueue& operator=(const TQueue& obj);
    TQueue& operator=(TQueue&& obj);
    bool operator==(const TQueue& obj) const;
    bool operator!=(const TQueue& obj) const;

    template <class O>
    friend std::ostream& operator<<(std::ostream& o, const TQueue<O>& v);
    template <class I>
    friend std::istream& operator>>(std::istream& i, TQueue<I>& v);

    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);
};

template<class T>
inline TQueue<T>::TQueue() : data(nullptr), start(0), end(0), len(0) {}

template<class T>
inline TQueue<T>::TQueue(int len_) : TQueue<T>::TQueue()
{
    if (len_ < 0)
        throw std::invalid_argument("len < 0");
    if (len_ > 0)
    {
        len = len_;
        data = new T[len];
    }
}

template<class T>
inline TQueue<T>::TQueue(const TQueue& obj)
    : start(obj.start), end(obj.end), len(obj.len)
{
    if (obj.len > 0)
    {
        data = new T[len];
        for (int i = 0; i < obj.GetCount(); i++)
        {
            int index = (obj.start + i) % obj.len;
            data[(start + i) % len] = obj.data[index];
        }
    }
    else
    {
        data = nullptr;
    }
}

template<class T>
inline TQueue<T>::TQueue(TQueue&& obj)
    : data(obj.data), start(obj.start), end(obj.end), len(obj.len)
{
    obj.data = nullptr;
    obj.start = obj.end = obj.len = 0;
}

template<class T>
inline TQueue<T>::~TQueue()
{
    delete[] data;
}

template<class T>
inline int TQueue<T>::GetLen() const { return len; }
template<class T>
inline int TQueue<T>::GetStart() const { return start; }
template<class T>
inline int TQueue<T>::GetEnd() const { return end; }
template<class T>
inline int TQueue<T>::GetCount() const
{
    if (len == 0) return 0;
    return (end >= start) ? (end - start) : (len - start + end);
}

template<class T>
inline void TQueue<T>::Resize(int newLen)
{
    if (newLen < 0)
        throw std::invalid_argument("len < 0");
    if (newLen == len) return;

    int currentCount = GetCount();
    if (newLen < currentCount)
        throw std::logic_error("New size is smaller than current element count in queue");

    if (newLen == 0)
    {
        delete[] data;
        data = nullptr;
        start = end = len = 0;
        return;
    }

    T* newData = new T[newLen];
    for (int i = 0; i < currentCount; i++)
        newData[i] = data[(start + i) % len];

    delete[] data;
    data = newData;
    len = newLen;
    start = 0;
    end = currentCount;
}

template<class T>
inline void TQueue<T>::Push(T value)
{
    if (IsFull())
        throw std::logic_error("Queue is full");
    data[end] = value;
    end = (end + 1) % len;
}

template<class T>
inline T TQueue<T>::Pop()
{
    if (IsEmpty())
        throw std::logic_error("Queue is empty");
    T result = data[start];
    start = (start + 1) % len;
    return result;
}

template<class T>
inline bool TQueue<T>::IsEmpty() const { return start == end; }
template<class T>
inline bool TQueue<T>::IsFull() const { return (end + 1) % len == start; }

template<class T>
inline T TQueue<T>::FindMin() const
{
    if (IsEmpty())
        throw std::logic_error("Cannot find min in empty queue");
    T minValue = data[start];
    for (int i = 1; i < GetCount(); i++)
    {
        int index = (start + i) % len;
        if (data[index] < minValue)
            minValue = data[index];
    }
    return minValue;
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(const TQueue& obj)
{
    if (this == &obj) return *this;

    delete[] data;
    start = obj.start;
    end = obj.end;
    len = obj.len;

    if (obj.len > 0)
    {
        data = new T[len];
        for (int i = 0; i < obj.GetCount(); i++)
            data[(start + i) % len] = obj.data[(obj.start + i) % obj.len];
    }
    else
        data = nullptr;

    return *this;
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(TQueue&& obj)
{
    if (this == &obj) return *this;

    delete[] data;
    data = obj.data;
    start = obj.start;
    end = obj.end;
    len = obj.len;

    obj.data = nullptr;
    obj.start = obj.end = obj.len = 0;
    return *this;
}

template<class T>
inline bool TQueue<T>::operator==(const TQueue& obj) const
{
    if (len != obj.len || GetCount() != obj.GetCount()) return false;
    for (int i = 0; i < GetCount(); i++)
        if (data[(start + i) % len] != obj.data[(obj.start + i) % obj.len])
            return false;
    return true;
}

template<class T>
inline bool TQueue<T>::operator!=(const TQueue& obj) const { return !(*this == obj); }

template<class O>
inline std::ostream& operator<<(std::ostream& o, const TQueue<O>& v)
{
    o << "TQueue[len=" << v.len << ", start=" << v.start << ", end=" << v.end << ", count=" << v.GetCount() << "]\n";
    o << "Data: ";
    if (v.GetCount() > 0)
    {
        for (int i = 0; i < v.GetCount(); i++)
        {
            o << v.data[(v.start + i) % v.len];
            if (i < v.GetCount() - 1) o << ", ";
        }
    }
    else o << "empty";
    o << "\n";
    return o;
}

template<class I>
inline std::istream& operator>>(std::istream& i, TQueue<I>& v)
{
    int elementCount;
    i >> elementCount;
    if (!i.good()) return i;
    if (elementCount < 0) throw std::invalid_argument("element count < 0");

    delete[] v.data;
    if (elementCount == 0)
    {
        v.data = nullptr;
        v.len = 0;
        v.start = v.end = 0;
        return i;
    }

    v.len = elementCount + 1;
    v.data = new I[v.len];
    v.start = v.end = 0;

    for (int j = 0; j < elementCount && i.good(); j++)
    {
        I value;
        i >> value;
        if (i.good()) v.Push(value);
    }

    return i;
}

template<class T>
inline void TQueue<T>::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    int count = GetCount();
    file << count << std::endl;
    for (int i = 0; i < count; i++)
        file << data[(start + i) % len] << std::endl;
}

template<class T>
inline void TQueue<T>::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    delete[] data;
    data = nullptr;
    start = end = len = 0;

    int elementCount;
    file >> elementCount;
    if (!file.good() || elementCount < 0)
        throw std::runtime_error("Error reading element count from file");

    if (elementCount == 0) return;

    len = elementCount + 1;
    data = new T[len];
    start = end = 0;

    for (int i = 0; i < elementCount; i++)
    {
        T value;
        file >> value;
        if (!file.good())
        {
            delete[] data;
            data = nullptr;
            len = start = end = 0;
            throw std::runtime_error("Error reading element from file");
        }
        data[end] = value;
        end = (end + 1) % len;
    }
}
