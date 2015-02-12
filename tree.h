#ifndef TREE_H
#define TREE_H

#include "treeitem.h"

template <class T>
class Tree : public TreeItem<T>
{

    private:

     TreeItem<T> *currentItem;

    public:
        Tree():TreeItem<T>(0),currentItem(0)
        {

        }

        TreeItem<T> *stepForward()
        {
            if ( currentItem == 0)
            {
                currentItem = this->begin ();

                return currentItem;
            }

            if ( currentItem == this->end() )
                return this->end();

            currentItem = after(currentItem);

            return currentItem;


        }

        TreeItem<T> * next()
        {
            if ( currentItem == 0)
            {
                currentItem = this->begin ();

                return currentItem;
            }

            if ( currentItem->childCount () != 0 )
            {

                currentItem = currentItem->child (0);

                return currentItem;

            } else
            {
                TreeItem<T> *parent = currentItem->parent ();


                if ( parent != 0 )
                {

                    currentItem = parent->after (currentItem);

                    if ( currentItem == 0 )
                    {
                        if ( parent != parent->root () )
                        {

                            currentItem = parent->parentNext ();

                            return currentItem;

                        } else  return 0;

                    }

                    return 0;




                } else
                {
                    currentItem = after(currentItem);

                    return currentItem;
                }

                return currentItem;
            }



        }

        void setCurrentItemToBeginning()
        {
            currentItem = this->begin ();
        }

        TreeItem<T> *current()
        {
            return currentItem;
        }



};

#endif // TREE_H
