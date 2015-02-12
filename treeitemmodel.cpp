#include "treeitemmodel.h"

TreeItemModel::TreeItemModel(QObject *parent) : QAbstractItemModel(parent)
{
    tree = new Tree<QVariant>();
    tree->add("");
    tree->appendChild();
    tree->child(0)->add("Teszt");
    tree->child(0)->appendChild();
    tree->child(0)->child(0)->add("Gyerek");
    tree->child(0)->child(0)->add("születik");
    tree->child(0)->child(0)->appendChild();
    tree->child(0)->child(0)->child(0)->add("Még1x");
    tree->appendChild();
    tree->child(1)->add("Elek");
 rootItem = tree->root();
}

TreeItemModel::~TreeItemModel()
{
 delete tree;
}
QModelIndex TreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
   if (!hasIndex(row, column, parent))
      return QModelIndex();

   PTIQVar parentItem;

   if (!parent.isValid())
       parentItem = rootItem;
   else
       parentItem = static_cast<PTIQVar>(parent.internalPointer());

   PTIQVar childItem = parentItem->child(row);
   if (childItem)
       return createIndex(row, column, childItem);
   else
       return QModelIndex();
}

QModelIndex TreeItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
         return QModelIndex();

    PTIQVar childItem = static_cast<PTIQVar>(child.internalPointer());
    PTIQVar parentItem = childItem->parent();

    if (parentItem == rootItem)
             return QModelIndex();

     return createIndex(parentItem->parentsRow(), 0, parentItem);
}
int TreeItemModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return tree->dataCount();
}
int TreeItemModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid () )
       return tree->childCount ();

    PTIQVar ti = static_cast<PTIQVar>( parent.internalPointer());
    return ti ? ti->childCount() : 0;
}

QVariant TreeItemModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid ())
        return QVariant();

    if ( role == Qt::SizeHintRole )
    {
        return QVariant();
    }
    if ( role == Qt::DisplayRole )
    {
        PTIQVar ti = static_cast < PTIQVar >( index.internalPointer ());

        if ( ti == 0)
            return QVariant();

        QVariant  data = ti->data (0);
        return data;

    }

    return QVariant();
}

QVariant TreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return tree->data(section);
    }

    return QVariant();
}
