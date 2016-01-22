
//!This file is to implement archive for different types.

template <class T>
typename std::enable_if<
    std::is_arithmetic<T>::value, void
>::type
archive(T & data ){
    switch(m_iAT)
    {
    case seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)&data, sizeof(T));
        m_iBase += sizeof(T);
        break;
    case deseralizer:
        std::memcpy((char *) &data, m_pReadBuf + m_iBase, sizeof(T));
        m_iBase += sizeof(T);
        break;
    case length_retriver:
        m_iBase += sizeof(T);
        break;
    }
}

void archive(String & s);

template<class T, size_t N>
typename std::enable_if<
    std::is_arithmetic<T>::value, void
>::type
archive(T (& data)[N])
{
    switch (m_iAT) {
    case seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)&data, sizeof(T) * N);
        m_iBase += sizeof(T) * N;
        break;
    case deseralizer:
        std::memcpy((char *)&data, m_pReadBuf + m_iBase, sizeof(T) * N);
        m_iBase += sizeof(T) * N;
        break;
    case length_retriver:
        m_iBase += sizeof(T) * N;
        break;
    }
}

template<class T, size_t N>
typename std::enable_if<
    !std::is_arithmetic<T>::value, void
>::type
archive(T(&data)[N])
{
    for(size_t i = 0; i < N; i ++)
    {
        archive(data[i]);
    }
}

template<class T>
typename std::enable_if<
    std::is_arithmetic<T>::value, void
>::type
archive(T * & data, size_t  count)
{
    switch(m_iAT)
    {
    case seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)data, sizeof(T) * count);
        m_iBase += sizeof(T)*count;
        break;
    case deseralizer:
        std::memcpy((char *)data, m_pReadBuf + m_iBase, sizeof(T)*count);
        m_iBase += sizeof(T) * count;
        break;
    case length_retriver:
        m_iBase += sizeof(T) * count;
        break;
    }
}

template<class T>
typename std::enable_if<
    std::is_arithmetic<T>::value, void
>::type
archive(std::vector<T> & data)
{
    size_t count = data.size();
    archive(count);
    switch(m_iAT)
    {
    case seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)&data.front(), sizeof(T) * count);
        m_iBase += count * sizeof(T);
        break;
    case deseralizer:
        data.clear();
        for(int i = 0; i < count; ++i)
        {
            T d;
            archive_data(d);
            data.push_back(d);
        }
        break;
    case length_retriver:
        m_iBase += sizeof(T) * count;
        break;
    }
}
