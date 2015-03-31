#ifndef CSINGLETON_H
#define CSINGLETON_H
// definition of singleton
template <class T>
class CSingleton
{ 
    static T *inst;
    protected:
        CSingleton(){}
    public:
        inline static T * instance()
        {
            //return  (inst) ? inst  : inst = new T;
            if ( inst == 0 ){
                inst = new T;
            }
            return inst;

        }
        inline static void destruct()
        {
           if ( inst != 0 )
           {
                delete inst;
                inst = 0;
           }
        }
};

template <class T> T* CSingleton<T>::inst = 0;
// -----------------

#endif // CSINGLETON_H
