#include "managerdb.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

ManagerDb::ManagerDb(const QString &pathDatabase, QObject *parent) : QObject(parent)
  , db(QSqlDatabase::addDatabase("QSQLITE", __FUNCTION__))
  , _pathDatabase(pathDatabase)
  , key_countries("countries")
  , key_operators("operators")
{

}

ManagerDb::~ManagerDb()
{
    if (db.isOpen()){
        db.close();
    }
}

bool ManagerDb::loadDatabase()
{
    if (!QFile::exists(_pathDatabase)) {
        qCritical() << "File of Database is not exist" << _pathDatabase << Qt::endl;
        return false;
    }
    db.setDatabaseName(_pathDatabase);
    if(!db.open()){
        qCritical() << "Database not opened" << _pathDatabase << Qt::endl;
        return false;
    }
    return true;
}

bool ManagerDb::changeOperatorName(const QString &mcc, const QString &mnc, const QString &oldName, const QString &newName)
{
    if (mcc.isEmpty() || mnc.isEmpty() || oldName.isEmpty() || newName.isEmpty()){
        qWarning() << Q_FUNC_INFO << "failed: value cannot be empty" << Qt::endl;
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QString("UPDATE %1 SET name = :newName WHERE name = :oldName AND mcc = :mcc AND mnc = :mnc").arg(key_operators));
    query.bindValue(":mcc", mcc);
    query.bindValue(":mnc", mnc);
    query.bindValue(":oldName", oldName);
    query.bindValue(":newName", newName);
    if(query.exec()){
        return true;
    }
    qWarning() << Q_FUNC_INFO << "failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDb::saveNewOperator(const QString &mcc, const QString &mnc, const QString &name)
{
    if (name.isEmpty() || mcc.isEmpty() || mnc.isEmpty()){
        qWarning() << Q_FUNC_INFO << "failed: value cannot be empty" << mcc << mnc << name << Qt::endl;
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QString("INSERT INTO %1 (mcc, mnc, name) VALUES (:mcc, :mnc, :name)").arg(key_operators));
    query.bindValue(":mcc", mcc);
    query.bindValue(":mnc", mnc);
    query.bindValue(":name", name);
    if(query.exec()){
        return true;
    }
    qWarning() << Q_FUNC_INFO << "failed: " << query.lastError() << Qt::endl;
    return false;
}

QVector<CountryData> ManagerDb::getAllCountryData() const
{
    QVector<CountryData> vec;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM " + key_countries);
    if(!query.exec()){
        qWarning() << Q_FUNC_INFO << "failed: " << query.lastError() << Qt::endl;
        return {};
    }
    const QSqlRecord record = query.record();
    const int id_mcc        = record.indexOf("mcc");
    const int id_code       = record.indexOf("code");
    const int id_name       = record.indexOf("name");
    while (query.next())
    {
        const QString str_mcc           = query.value(id_mcc).toString();
        const QString str_code          = query.value(id_code).toString();
        const QString str_name          = query.value(id_name).toString();
        vec.append(CountryData{createCountryIcon(str_code), str_name, str_mcc, str_code});
    }
    return vec;
}

QVector<OperatorData> ManagerDb::getAllOperatorData() const
{
    QVector<OperatorData> vec;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM " + key_operators);
    if(!query.exec()){
        qWarning() << Q_FUNC_INFO << "failed: " << query.lastError() << Qt::endl;
        return {};
    }
    const QSqlRecord record = query.record();
    const int id_mcc        = record.indexOf("mcc");
    const int id_mnc        = record.indexOf("mnc");
    const int id_name       = record.indexOf("name");
    while (query.next())
    {
        const QString str_mcc           = query.value(id_mcc).toString();
        const QString str_mnc           = query.value(id_mnc).toString();
        const QString str_name          = query.value(id_name).toString();
        vec.append(OperatorData{createOperatorIcon(str_mcc, str_mnc), str_name, str_mcc, str_mnc});
    }
    return vec;
}

bool ManagerDb::mccExist(const QString &mcc)
{
    QSqlQuery query(db);
    query.prepare(QString("SELECT * FROM %1 WHERE mcc = :mcc").arg(key_countries));
    query.bindValue(":mcc", mcc);
    if (query.exec()){
        return query.next();
    }
    qWarning() << __FUNCTION__ << "failed: " << query.lastError() << Qt::endl;
    return false;
}

QString ManagerDb::getPathDB() const
{
    return _pathDatabase;
}

