#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "itemdata.h"
#include "customtype.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum TreeModelRole{
        CustomTypeRole  = Qt::UserRole + 1,
    };
    Q_ENUM(TreeModelRole)

    TreeModel(QObject *parent = nullptr);
    ~TreeModel();

    void loadData(const QVector<NodeData> &vecItemData);
    void updateOperatorData(QModelIndex index, const OperatorData &operatorData);
    void insertNewOperatorData(const OperatorData &operatorData);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariant getCustomType(const QModelIndex &index);
    Q_INVOKABLE QString getCountryIcon(const QString &str_mcc);
    Q_INVOKABLE QString getOperatorIcon(const QString &str_mcc, const QString &str_mnc);


private:
    TreeItem *getItem(const QModelIndex &index) const;
    TreeItem *findItemCountry(const QString &mcc, TreeItem *rootItem);

private:
    const QHash<int, QByteArray> m_roleNameMapping;
    TreeItem *rootItem;
};
