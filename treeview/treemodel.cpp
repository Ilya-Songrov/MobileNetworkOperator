#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_roleNameMapping({ {CustomTypeRole, "customTypeRole"} })
{
    rootItem = new TreeItem(QVector<QVariant>() << QString(), nullptr);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

void TreeModel::loadData(const QVector<NodeData> &vecItemData)
{
    if (rootItem->childCount() > 0) {
        qWarning() << "Data has already loaded" << Qt::endl;
        return;
    }

    beginResetModel();
    for (const NodeData &node : vecItemData) {
        QVariant variantCountry;
        variantCountry.setValue(new CustomType(node.countryData, this));
        TreeItem *treeItemCountry = new TreeItem(QVector<QVariant>() << variantCountry, rootItem);
        for (const OperatorData &operatorData : node.vecOperators) {
            QVariant variantOperator;
            variantOperator.setValue(new CustomType(operatorData, this));
            TreeItem *treeItemOperator = new TreeItem(QVector<QVariant>() << variantOperator, treeItemCountry);
            treeItemCountry->appendChild(treeItemOperator);
        }
        rootItem->appendChild(treeItemCountry);
    }
    endResetModel();
}

void TreeModel::updateOperatorData(QModelIndex index, const OperatorData &operatorData)
{
    const QVariant v = getCustomType(index);
    auto *customType = v.value<CustomType*>();
    if (customType) {
        customType->setIcon(operatorData.icon);
        customType->setName(operatorData.name);
        customType->setMcc(operatorData.mcc);
        customType->setMnc(operatorData.mnc);
    }
    else{
        qCritical() << "error while updating operator" << Qt::endl;
    }
}

void TreeModel::insertNewOperatorData(const OperatorData &operatorData)
{
    auto *treeItemCountry = findItemCountry(operatorData.mcc, rootItem);
    if (treeItemCountry) {
        QModelIndex indexCountry = createIndex(treeItemCountry->childNumber(), 0, treeItemCountry);
        if (!this->insertRow(0, indexCountry)){
            return;
        }
        QVariant variantOperator;
        variantOperator.setValue(new CustomType(operatorData, this));
        const QModelIndex child = this->index(0, 0, indexCountry);
        this->setData(child, variantOperator, TreeModel::CustomTypeRole);
    }
}


int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != TreeModelRole::CustomTypeRole){
        return QVariant();
    }

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && role == TreeModelRole::CustomTypeRole){
        return rootItem->data(section);
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }

    TreeItem *parentItem = getItem(parent);
    if (!parentItem){
        return QModelIndex();
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem){
        return false;
    }

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QModelIndex();
    }

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem){
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0){
        removeRows(0, rowCount());
    }

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem){
        return false;
    }

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return m_roleNameMapping;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != TreeModel::CustomTypeRole){
        return false;
    }

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result){
        emit dataChanged(index, index, {});
    }

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || role != TreeModel::CustomTypeRole || orientation != Qt::Horizontal){
        return false;
    }

    const bool result = rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

QVariant TreeModel::getCustomType(const QModelIndex &index)
{
    return data(index, CustomTypeRole);
}

QString TreeModel::getCountryIcon(const QString &str_mcc)
{
    auto *treeItemCountry = findItemCountry(str_mcc, rootItem);
    if (treeItemCountry) {
        CustomType *customType = treeItemCountry->data(0).value<CustomType*>();
        if (customType) {
            return createCountryIcon(customType->code());
        }
    }
    return createCountryIcon(QString());
}

QString TreeModel::getOperatorIcon(const QString &str_mcc, const QString &str_mnc)
{
    return createOperatorIcon(str_mcc, str_mnc);
}

TreeItem *TreeModel::findItemCountry(const QString &mcc, TreeItem *rootItem)
{
    const auto childs = rootItem->children();
    for (TreeItem *item : childs) {
        if (!item || (item && item->columnCount() < 1)) {
            continue;
        }
        CustomType *customType = item->data(0).value<CustomType*>();
        if (customType
                && customType->typeData() == CustomType::CountryDataType
                && customType->mcc() == mcc) {
            return item;
        }
    }
    return nullptr;
}
