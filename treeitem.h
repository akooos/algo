#ifndef NOTFTREEITEM_H
#define NOTFTREEITEM_H

#include <QList>

template < class T >
class TreeItem
{

    private:

        QList< TreeItem<T>  *> childItems;
        QList< T >  childdatas;
        TreeItem <T>*parentItem;



    public:
        TreeItem(TreeItem<T> *parent ):parentItem(parent)
        {

        }

        virtual   ~TreeItem()
        {
            clearChildren ();
        }


        void add(T _data)
        {
            childdatas.append (_data);
        }

        void insert(int row, T _data)
        {
            childdatas.insert (row,_data);
        }

        void addChild(  TreeItem<T>  *child)
        {
            child->setParentItem (this);
            childItems.append (child);
        }

        void insertChild(int row, TreeItem<T>  *child  )
        {
            child->setParentItem (this);
            childItems.insert (row,child);

        }

        T removeAt(int index)
        {
            T t = data(index);

            childdatas.removeAt (index);

            return t;
        }

        void appendChild()
        {
            addChild (new TreeItem<T>(this));
        }

        void removeChild(int index)
        {
            TreeItem *ti = childItems.at (index);
            delete ti;
            ti = 0;

            childItems.removeAt (index);
        }

        TreeItem<T>  *  child(int row) const
        {

            if ( row < 0 || row >= childItems.count() )
                return NULL;

            return childItems.at (row);
        }

        T data(int column) const
        {
            return childdatas.at (column);
        }

        bool hasChild (TreeItem<T> * ptr) const
        {
            int i = childItems.indexOf ( ptr );
            return i != -1;
        }

        bool hasData(T _data) const
        {
            int index = childdatas.indexOf (_data);
            return index != -1;
        }
        int childCount() const
        {
            return childItems.count ();
        }

        int dataCount() const
        {
            return childdatas.count ();
        }

        int parentsRow() const
        {
            if ( parentItem != 0 )

                return parentItem->childItems.indexOf(const_cast< TreeItem<T> *>(this));

            return -1;

        }
        TreeItem<T>  * parent()
        {
            return parentItem;
        }

        TreeItem<T> *root()
        {
            if ( parentItem != 0 )
            {
                TreeItem<T> *ti = parentItem->root();
                return ti;

            } else return this;

        }

        TreeItem<T> *oldestParentBegin()
        {
            if ( parentItem != 0 )
            {

                return parentItem->oldestParentBegin ();

            } else return begin();

        }

        TreeItem<T> *youngestChildEnd()
        {
            if ( childCount ()  == 0 )
                return 0;

            TreeItem  *item = end()->youngestChildEnd ();

            if ( item == 0 )
            {
                return end ();

            } else return item;
        }

        TreeItem<T> * parentNext()
        {
            if ( parent () == 0 )
            {
                return 0 ;
            }

            TreeItem<T> * res =  parent()->after (this);

            if ( res == 0 )
            {
                 res = parent ()->parentNext ();

                 return res;

            }

            return res;
        }

        TreeItem<T> *after(TreeItem<T> *ti)
        {
            int index = childItems.indexOf (ti);

            if ( (index+1) >= childItems.count () || index == -1 )
                return 0;

            return childItems.at (index+1);
        }



        TreeItem<T> *begin()
        {
            if ( childCount ()  == 0 )
                return 0;

            return child (0);
        }

        TreeItem<T> *end()
        {
            if ( childCount ()  == 0 )
                return 0;

            return child (childCount()-1);
        }

        void clearChildren()
        {
            TreeItem<T> *ti;

            while( childItems.count ()  != 0 )
            {
                ti = childItems.at (0);

                ti->clearChildren ();

                removeChild (0);
            }
        }

        TreeItem<T> * before(TreeItem<T> *ti)
        {
            int index = childItems.indexOf (ti);

            if ( (index-1) < 0 )
                return 0;

            return childItems.at (index-1);
        }

        int childIndexOf(TreeItem<T> *ti) const
        {
            return childItems.indexOf (ti);
        }

        int dataIndexOf(T _data)
        {
            return childdatas.indexOf (_data);
        }
        void setParentItem(TreeItem<T> *new_parent)
        {
            parentItem = new_parent;
        }

};

#endif // NOTFTREEITEM_H
