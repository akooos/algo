#ifndef TREEITEMMODEL_H
#define TREEITEMMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "algorythm.h"
#include "tree.h"

typedef TreeItem<QVariant>* PTIQVar;


class TreeItemModel : public QAbstractItemModel
{
    Q_OBJECT

    Tree<QVariant>      *tree;
    PTIQVar             rootItem;

public:
    virtual QModelIndex index ( int row, int column, const QModelIndex &parent ) const;
    virtual QModelIndex parent ( const QModelIndex &child ) const;
    virtual int columnCount ( const QModelIndex &parent ) const;
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                             int role = Qt::DisplayRole) const;
    explicit TreeItemModel( QObject *parent = 0);
    ~TreeItemModel();

signals:

public slots:
};

#endif // TREEITEMMODEL_H
