#ifndef UTIL
#define UTIL


template <class T> class VPtr
{
public:
    static T* asPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};
#endif // UTIL

