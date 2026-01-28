#ifndef CHARTBUFFER_H
#define CHARTBUFFER_H

#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QtGlobal>


template<typename T>
struct DefaultMinMax
{
    T min(const T& a, const T& b) const
    {
        return a < b ? a : b;
    }

    T max(const T& a, const T& b) const
    {
        return a > b ? a : b;
    }

    bool equal(const T& a, const T& b) const
    {
        return a == b;
    }
};

/* =======================
 * ChartBuffer
 * ======================= */
template <typename T, typename MinMaxFunc = DefaultMinMax<T>>
class ChartBuffer
{
public:
    explicit ChartBuffer(int size = DefaultSize,
                         MinMaxFunc minMax = MinMaxFunc())
        : m_capacity(size),
        m_size(0),
        m_writeIndex(0),
        m_minMax(minMax),
        m_hasMinMax(false)
    {
        m_buffer.resize(size);
    }

    void write(const T &data)
    {
        QMutexLocker locker(&m_mutex);

        bool needRecompute = false;

        if (m_size == m_capacity) {
            const T& old = m_buffer[m_writeIndex];
            if (m_hasMinMax && (m_minMax.equal(m_min, old) || m_minMax.equal(m_max, old))) {
                needRecompute = true;
            }
        }


        m_buffer[m_writeIndex] = data;
        m_writeIndex = (m_writeIndex + 1) % m_capacity;
        m_size = qMin(m_size + 1, m_capacity);

        if (!m_hasMinMax) {
            m_min = m_max = data;
            m_hasMinMax = true;
        } else {
            m_min = m_minMax.min(m_min, data);
            m_max = m_minMax.max(m_max, data);
        }

        if (needRecompute) {
            recomputeMinMaxLocked();   // 修正问题 2
        }
    }

    void writeMultiple(const QVector<T> &data)
    {
        QMutexLocker locker(&m_mutex);

        bool needRecompute = false;

        for (const T& value : data) {
            if (m_size == m_capacity) {
                const T& old = m_buffer[m_writeIndex];
                if (m_hasMinMax && (m_minMax.equal(m_min, old) || m_minMax.equal(m_max, old))) {
                    needRecompute = true;
                }
            }

            m_buffer[m_writeIndex] = value;
            m_writeIndex = (m_writeIndex + 1) % m_capacity;
            m_size = qMin(m_size + 1, m_capacity);

            if (!m_hasMinMax) {
                m_min = m_max = value;
                m_hasMinMax = true;
            } else {
                m_min = m_minMax.min(m_min, value);
                m_max = m_minMax.max(m_max, value);
            }
        }

        if (needRecompute) {
            recomputeMinMaxLocked();
        }
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
        if (count > m_size) count = m_size;

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
        if (m_size == 0) return result;

        result.reserve(m_size);

        if (m_size < m_capacity) {
            result = m_buffer.mid(0, m_size);
        } else {
            result = m_buffer.mid(m_writeIndex, m_capacity - m_writeIndex);
            result.append(m_buffer.mid(0, m_writeIndex));
        }
        return result;
    }

    bool getMinMax(T& minVal, T& maxVal) const
    {
        QMutexLocker locker(&m_mutex);
        if (!m_hasMinMax) return false;

        minVal = m_min;
        maxVal = m_max;
        return true;
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
        m_hasMinMax = false;
    }

private:
    void recomputeMinMaxLocked()
    {
        if (m_size == 0) {
            m_hasMinMax = false;
            return;
        }

        m_min = m_max = m_buffer[0];
        for (int i = 1; i < m_size; ++i) {
            m_min = m_minMax.min(m_min, m_buffer[i]);
            m_max = m_minMax.max(m_max, m_buffer[i]);
        }
    }

private:
    static constexpr int DefaultSize = 4000;

    QVector<T>     m_buffer;
    int            m_capacity;
    int            m_size;
    int            m_writeIndex;

    T              m_min;
    T              m_max;
    bool           m_hasMinMax;

    MinMaxFunc     m_minMax;
    mutable QMutex m_mutex;
};

#endif // CHARTBUFFER_H
