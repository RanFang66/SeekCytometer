#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QVector>
#include <QMutex>
#include <QWaitCondition>


template <typename T>
class RingBuffer
{
public:
    explicit RingBuffer(int size = DefaultCapacity)
        : m_capacity(size), m_size(0), m_writeIndex(0), m_readIndex(0)
    {
        m_buffer.reserve(size);
    }

    void write(const T&data);
    void writeMultiple(const QVector<T> &data);
    int  writeMultipleNoWait(const QVector<T> &data);
    T read();
    QVector<T> readMultiple(int count);
    QVector<T> readMultipleNoWait(int maxCount);
    bool isEmpty() const { return m_size == 0; }
    bool isFull() const { return m_size == m_capacity; }
    int  avaiable() const { return m_size; }
    int  remainingCapacity() const { return m_capacity - m_size; }
    void reset();
    void init(T data);


private:
    static constexpr int DefaultCapacity = 10000;
    QVector<T> m_buffer;
    int m_capacity;
    int m_size;
    int m_writeIndex;
    int m_readIndex;
    mutable QMutex m_mutex;
    QWaitCondition m_notEmpty;
    QWaitCondition m_notFull;
};

template <typename T>
void RingBuffer<T>::writeMultiple(const QVector<T> &data)
{
    if (data.size() <= 0) return;
    QMutexLocker locker(&m_mutex);
    int remaining = data.size();
    while (remaining > 0) {
        while (m_size == m_capacity) {
            m_notFull.wait(&m_mutex);
        }
        int actualCount = qMin(remaining, m_capacity - m_size);
        for (int i = 0; i < actualCount; i++) {
            m_buffer[m_writeIndex] = data.at(data.size() - remaining + i);
            m_writeIndex = (m_writeIndex + 1) % m_capacity;
        }
        m_size += actualCount;
        remaining -= actualCount;
        m_notEmpty.wakeAll();
    }
}

template<typename T>
inline int RingBuffer<T>::writeMultipleNoWait(const QVector<T> &data)
{
    if (data.size() <= 0) return 0;
    QMutexLocker locker(&m_mutex);
    int actualCount = qMin(data.size(), m_capacity - m_size);
    for (int i = 0; i < actualCount; i++) {
        m_buffer[m_writeIndex] = data.at(i);
        m_writeIndex = (m_writeIndex + 1) % m_capacity;
    }
    m_size += actualCount;
    m_notEmpty.wakeAll();

    return actualCount;
}


template <typename T>
void RingBuffer<T>::write(const T &data)
{
    QMutexLocker locker(&m_mutex);
    while (m_size == m_capacity) {
        m_notFull.wait(&m_mutex);
    }

    m_buffer[m_writeIndex] = data;
    m_writeIndex = (m_writeIndex + 1) % m_capacity;
    m_size++;
    m_notEmpty.wakeAll();
}

template <typename T>
T RingBuffer<T>::read()
{
    QMutexLocker locker(&m_mutex);
    while (m_size == 0) {
        m_notEmpty.wait(&m_mutex);
    }

    T data = m_buffer[m_readIndex];
    m_readIndex = (m_readIndex + 1) % m_capacity;
    m_size--;
    m_notFull.wakeAll();
    return data;
}

template<typename T>
inline QVector<T> RingBuffer<T>::readMultiple(int count)
{
    QVector<T> result;
    if (count <= 0) return result;
    QMutexLocker locker(&m_mutex);
    while (count > 0) {
        while (m_size == 0) {
            m_notEmpty.wait(&m_mutex);
        }
        int readable = qMin(count, m_size);
        int firstPart = qMin(readable, m_capacity - m_readIndex);
        result.append(m_buffer.mid(m_readIndex, firstPart));
        if (firstPart < readable) {
            result.append(m_buffer.mid(0, readable - firstPart));
        }
        m_readIndex = (m_readIndex + readable) % m_capacity;
        m_size -= readable;
        count -= readable;
        m_notFull.wakeAll();
    }
    return result;
}

template<typename T>
inline QVector<T> RingBuffer<T>::readMultipleNoWait(int maxCount)
{
    QVector<T> result;
    if (maxCount <= 0) return result;
    QMutexLocker locker(&m_mutex);
    int actualCount = qMin(maxCount, m_size);
    int firstPart = qMin(actualCount, m_capacity - m_readIndex);
    result.append(m_buffer.mid(m_readIndex, firstPart));
    if (firstPart < actualCount) {
        result.append(m_buffer.mid(0, actualCount - firstPart));
    }
    m_readIndex = (m_readIndex + actualCount) % m_capacity;
    m_size -= actualCount;
    m_notFull.wakeAll();
    return result;
}

template<typename T>
inline void RingBuffer<T>::reset()
{
    QMutexLocker locker(&m_mutex);
    m_size = 0;
    m_writeIndex = 0;
    m_readIndex = 0;
    m_buffer.clear();
    m_buffer.reserve(m_capacity);
    m_notFull.wakeAll();
}

template<typename T>
inline void RingBuffer<T>::init(T data)
{
    QMutexLocker locker(&m_mutex);
    m_buffer.clear();
    m_buffer.fill(data, m_capacity);
    m_size = 0;
    m_writeIndex = 0;
    m_readIndex = 0;
    m_notEmpty.wakeAll();
}


#endif // RINGBUFFER_H
