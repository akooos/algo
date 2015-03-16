#ifndef FACTORYBUILDER_H
#define FACTORYBUILDER_H


#include <map>
#include <vector>

#include "factoryitem.h"

/**
 * @author Ákos Tóth
 * @email  zuiadaton@gmail.com
 * @brief  Template class for self-register Factory pattern
 */

typedef void*(*FPCtor)();
/**
 * @template T base class that you want to construct with this factory class
 * @template K unique id key to find which class you want to construct
 */
template <class T, class K>
class FactoryBuilder{   

        static std::map<K,FPCtor> *m_items;
        public:
            static bool regist(K id, FPCtor ctor){

                if ( !m_items )
                    m_items = new std::map<K,FPCtor>();

                FactoryBuilder::m_items->insert(std::pair<K,FPCtor>(id,ctor));
                return true;
            }
            static std::vector<K> items(){

                std::vector<K> result;

                if ( !m_items )
                    return result;

                for( typename std::map<K,FPCtor>::iterator it = m_items->begin(); it != m_items->end() ;++it)
                   result.push_back(it->first);

                return result;
            }
            static T* create(K id){

                if ( !m_items )
                    return 0;

                typename std::map<K,FPCtor>::iterator it = m_items->find(id);
                if ( it != m_items->end() )
                {
                    FPCtor ctor = it->second ;
                    return reinterpret_cast<T*>(ctor());
                } else
                    return NULL;
            }
            static void deleteRegs(){
                if ( m_items )
                {
                    m_items = 0;
                    delete m_items;
                }
            }

};
template <class T, class K> std::map<K,FPCtor> * FactoryBuilder<T,K>::m_items = 0 ;
/**
   @param ClassName : class that you want to be registered
   @param Factory   : class that can create ClassName class(es)(factory items), this class inherits FactoryBuilder
   @param FactoryItemId : should be uniqu register id for ClassName class
  */
// ## will concatenate ClassName value to "Registered" word(ClassName will be prefix)
#define RegisterFactoryItem(ClassName,Factory,FactoryItemId) static bool _ ## ClassName ## Registered = Factory::regist(FactoryItemId,reinterpret_cast<FPCtor>(&ClassName::create));

#endif // FACTORYBUILDER_H
