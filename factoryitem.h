#ifndef FACTORYITEM
#define FACTORYITEM

template <class T>
struct FactoryItem{

    static T* create(){
        return new T;
    }
};


#endif // FACTORYITEM

