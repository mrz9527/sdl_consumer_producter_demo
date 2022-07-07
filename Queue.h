// author : zhoukang
// date   : 2022-06-14 16:53:53

#ifndef SDL_CONSUMER_PRODUCTER_DEMO_QUEUE_H
#define SDL_CONSUMER_PRODUCTER_DEMO_QUEUE_H

// 先进先出
template<typename T>
class Queue {
private:
    int capacity;   // 容量
    int size;       // 当前队列元素个数
    int head;       // 队头
    int tail;       // 队尾
    T* elements;
public:

    explicit Queue(int capacity = 10);
    bool Push(T task);
    T Pop();
    void Clear();
    inline bool Empty() {
        return size == 0;
    }

    inline bool Full() {
        return size == capacity;
    }

    inline int Size() {
        return size;
    }
    inline int Capacity() {
        return capacity;
    }

    ~Queue() {
        delete []elements;
    };

private:
    void Reset();
};


template<typename T>
Queue<T>::Queue(int c):capacity(c), size(0), head(0), tail(0)
{
    elements = new T[capacity];
}

template<typename T>
void Queue<T>::Clear()
{
    Reset();
}

template<typename T>
void Queue<T>::Reset()
{
    size = 0;
    head = 0;
    tail = 0;
}

template<typename T>
bool Queue<T>::Push(T task)
{
    if (size < capacity) {
        elements[tail] = task;
        tail = (++tail) % capacity;
        ++size;
        return true;
    }
    return false;
}

template<typename T>
T Queue<T>::Pop()
{
    --size;
    T elem = elements[head];
    head = (++head) % capacity;
    return elem;
}


#endif //SDL_CONSUMER_PRODUCTER_DEMO_QUEUE_H
