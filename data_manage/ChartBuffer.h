#ifndef CHARTBUFFER_H
#define CHARTBUFFER_H

#include <QVector>
#include <QMutex>

template <typename T>
class ChartBuffer
{
public:
    explicit ChartBuffer(int size = DefaultSize)
        : m_capacity(size), m_size(0), m_writeIndex(0)
    {
        m_buffer.reserve(size);
        T data;
        m_buffer.fill(data, size);
    }

    void write(const T &data)
    {
        QMutexLocker locker(&m_mutex);
        m_buffer[m_writeIndex] = data;
        m_writeIndex = (m_writeIndex + 1) % m_capacity;
        m_size = qMin(m_size + 1, m_capacity);
    }

    void writeMultiple(const QVector<T> &data)
    {
        QMutexLocker locker(&m_mutex);
        for (const T &value : data) {
            m_buffer[m_writeIndex] = value;
            m_writeIndex = (m_writeIndex + 1) % m_capacity;
        }

        m_size = qMin(m_size + data.size(), m_capacity);
    }

    T readLatest()
    {
        QMutexLocker locker(&m_mutex);
        if (m_size == 0) {
            return T();
        }
        return m_buffer[(m_writeIndex - 1 + m_capacity) % m_capacity];
    }

    QVector<T> readLatest(int count)
    {
        QMutexLocker locker(&m_mutex);
        QVector<T> result;
        if (count <= 0 || m_size == 0) return result;
        if (count > m_size) {
            count = m_size;
        }
        result.reserve(count);
        int index = (m_writeIndex - count + m_capacity) % m_capacity;
        for (int i = 0; i < count; ++i) {
            result.append(m_buffer[index]);
            index = (index + 1) % m_capacity;
        }
        return result;
    }

    QVector<T> readAll()
    {
        QMutexLocker locker(&m_mutex);
        QVector<T> result;
        if (m_size == 0) {
            return result;
        }
        result.reserve(m_size);
        if (m_size < m_capacity) {
            result = m_buffer.mid(0, m_size);
        } else {
            result = m_buffer.mid(m_writeIndex, m_capacity - m_writeIndex);
            result.append(m_buffer.mid(0, m_writeIndex));
        }
        return result;
    }

    bool isEmpty() const
    {
        QMutexLocker locker(&m_mutex);
        return m_size == 0;
    }

    bool isFull() const
    {
        QMutexLocker locker(&m_mutex);
        return m_size == m_capacity;
    }

    int size() const
    {
        QMutexLocker locker(&m_mutex);
        return m_size;
    }

    void clear()
    {
        QMutexLocker locker(&m_mutex);
        m_size = 0;
        m_writeIndex = 0;
    }

private:
    static constexpr int DefaultSize = 4000;
    QVector<T> m_buffer;
    int     m_capacity;
    int     m_size;
    int     m_writeIndex;
    mutable QMutex m_mutex;
};


#endif // CHARTBUFFER_H
