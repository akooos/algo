#ifndef FACTORYITEM
#define FACTORYITEM


/**
 * @author Ákos Tóth
 * @email  zuiadaton@gmail.com
 * @brief  Template class for self-register Factory pattern
 */

template <class T>
struct FactoryItem{

    static T* create(){
        return new T;
    }
};


#endif // FACTORYITEM

