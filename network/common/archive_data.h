
//!This file is to implement archive for different types.

template <class T>
typename boost::enable_if_c<
    boost::is_arithmetic<T>::value
>::type
archive(T & data ){
    switch(m_iAT)
    {
    case Archive::seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)&data, sizeof(T));
        m_iBase += sizeof(T);
        break;
    case Archive::deseralizer:
        std::memcpy((char *) &data, m_pReadBuf + m_iBase, sizeof(T));
        m_iBase += sizeof(T);
        break;
    case Archive::length_retriver:
        m_iBase += sizeof(T);
        break;
    }
}

void archive(String & s);

template<class T, size_t N>
typename boost::enable_if_c<
    boost::is_arithmetic<T>::value
>::type
archive(T (& data)[N])
{
    switch (m_iAT) {
    case Archive::seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)&data, sizeof(T) * N);
        m_iBase += sizeof(T) * N;
        break;
    case Archive::deseralizer:
        std::memcpy((char *)&data, m_pReadBuf + m_iBase, sizeof(T) * N);
        m_iBase += sizeof(T) * N;
        break;
    case Archive::length_retriver:
        m_iBase += sizeof(T) * N;
        break;
    }
}

template<class T, size_t N>
typename boost::enable_if_c<
    !boost::is_arithmetic<T>::value
>::type
archive(T(&data)[N])
{
    for(size_t i = 0; i < N; i ++)
    {
        archive(data[i]);
    }
}

template<class T>
typename boost::enable_if_c<
    boost::is_arithmetic<T>::value
>::type
archive(T * & data, size_t & count)
{
    archive(count);
    switch(m_iAT)
    {
    case seralizer:
        std::memcpy(m_pWriteBuf + m_iBase, (const char *)data, sizeof(T) * count);
        m_iBase += sizeof(T)*count;
        break;
    case deseralizer:
        data = (T*) malloc(sizeof(T) * count);
        if(data == NULL)
        {
            assert(false && "malloc failed!");
            exit(-1);
        }
        std::memcpy((char *)data, m_pReadBuf + m_iBase, sizeof(T)*count);
        m_iBase += sizeof(T) * count;
        break;
    case length_retriver:
        m_iBase += sizeof(T) * count;
        break;
    }
}

template<class T>
typename boost::enable_if_c<
    boost::is_arithmetic<T>::value
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
            archive(d);
            data.push_back(d);
        }
        break;
    case length_retriver:
        m_iBase += sizeof(T) * count;
        break;
    }
}
