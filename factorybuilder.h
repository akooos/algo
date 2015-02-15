#ifndef FACTORYBUILDER_H
#define FACTORYBUILDER_H


#include <map>
#include <vector>
#include "factoryitem.h"

typedef void*(*FPCtor)();

template <class T, class K>
class FactoryBuilder{   

         static std::map<K,FPCtor> m_items;

        public:
            static bool regist(K id, FPCtor ctor){
                FactoryBuilder::m_items.insert(std::pair<K,FPCtor>(id,ctor));
                return true;
            }
            static std::vector<K> items(){
                std::vector<K> result;
                for( typename std::map<K,FPCtor>::iterator it = m_items.begin(); it != m_items.end() ;++it)
                   result.push_back(it->first);

                return result;
            }
            static T* create(K id){
                typename std::map<K,FPCtor>::iterator it = m_items.find(id);
                if ( it != m_items.end() )
                {
                    FPCtor ctor = it->second ;
                    return reinterpret_cast<T*>(ctor());
                } else
                    return NULL;
            }

};
template <class T, class K> std::map<K,FPCtor> FactoryBuilder<T,K>::m_items ;

// ## will concatenate ClassName value to "Registered" word(ClassName will be prefix)
#define RegisterFactoryItem(ClassName,Factory,FactoryItemId) static bool _ ## ClassName ## Registered = Factory::regist(FactoryItemId,reinterpret_cast<FPCtor>(&ClassName::create));

#endif // FACTORYBUILDER_H
