/**************************************************************************
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/.
**
**************************************************************************/

#include "dbcreator.h"

DbCreator::DbCreator(QObject *parent) : QObject(parent)
  , db(QSqlDatabase::addDatabase("QSQLITE", __FUNCTION__))
  , table_countries("countries")
  , table_operators("operators")
{

}

DbCreator::~DbCreator()
{
    if (db.isOpen()){
        db.close();
    }
}

bool DbCreator::createDB(const QString &pathFile)
{
    QFile::remove(pathFile);
    if (QFile::exists(pathFile) && QFile(pathFile).size() > 0) {
        qDebug() << "File of Database exist. I do not create new database" << Qt::endl;
        return true;
    }
    db.setDatabaseName(pathFile);
    if(!db.open()){
        qCritical() << "Database not opened" << pathFile << Qt::endl;
        return false;
    }
    const bool ret = createTablesFromJsonFile();
    db.close();
    return ret;
}

bool DbCreator::createTablesFromJsonFile()
{
    const bool retCountries = createCountriesTable();
    const bool retOperators = createOperatorsTable();
    if (!retCountries || !retOperators) {
        return false;
    }
    QVector<CountryData> vecCountryData;
    QVector<OperatorData> vecOperatorData;
    readJsonFile(&vecCountryData, &vecOperatorData);
    if (vecCountryData.isEmpty() || vecOperatorData.isEmpty()) {
        return false;
    }
    const bool retQueryCountries = runQuery(createCountiesQuery(vecCountryData));
    const bool retQueryOperators = runQuery(createOperatorsQuery(vecOperatorData));
    if (!retQueryCountries || !retQueryOperators) {
        return false;
    }
    return true;
}

void DbCreator::readJsonFile(QVector<CountryData> *vecCountryData, QVector<OperatorData> *vecOperatorData)
{
    QFile file(QDir().absolutePath() + "/../MobileNetworkOperator/mcc-mnc-table/mcc-mnc-table.json");
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Can not open file" << file.fileName() << Qt::endl;
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    const QJsonArray arr = doc.array();
    for (const QJsonValue& value: arr) {
        const QJsonObject obj = value.toObject();
        const QString mcc = obj.value("mcc").toString();
        CountryData countryData;
        countryData.name        = obj.value("country").toString();
        countryData.code        = obj.value("iso").toString();
        countryData.mcc         = mcc;
        OperatorData operatorData;
        operatorData.name       = obj.value("network").toString();
        operatorData.mnc        = obj.value("mnc").toString();
        operatorData.mcc        = mcc;
        if (mcc.indexOf('\'') > -1
                || countryData.name.indexOf('\'') > -1
                || countryData.code.indexOf('\'') > -1
                || operatorData.name.indexOf('\'') > -1
                || operatorData.mnc.indexOf('\'') > -1
                ) {
            continue;
        }
        vecOperatorData->append(operatorData);
        const auto it = std::find(vecCountryData->begin(), vecCountryData->end(), countryData);
        if (it == std::end(*vecCountryData)) {
            vecCountryData->append(countryData);
        }
    }
    if (vecCountryData->isEmpty() || vecOperatorData->isEmpty()) {
        qWarning() << "Can not load data from file:" << file.fileName() << Qt::endl;
    }
}

bool DbCreator::createCountriesTable()
{
    QSqlQuery query(db);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "mcc TEXT NOT NULL,"
            "code TEXT NOT NULL,"
            "name TEXT NOT NULL"
            ");").arg(table_countries));

    if (query.exec()){
        return true;
    }
    qWarning() << __FUNCTION__ << "Couldn't create the table:" << query.lastError() << Qt::endl;
    return false;
}

bool DbCreator::createOperatorsTable()
{
    QSqlQuery query(db);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "mcc TEXT NOT NULL,"
            "mnc TEXT NOT NULL,"
            "name TEXT NOT NULL"
            ");").arg(table_operators));

    if (query.exec()){
        return true;
    }
    qWarning() << __FUNCTION__ << "Couldn't create the table:" << query.lastError() << Qt::endl;
    return false;
}

QString DbCreator::createCountiesQuery(const QVector<CountryData> &vecCountryData)
{
    QString strQuery = QString("INSERT INTO %1 (name, mcc, code) VALUES ").arg(table_countries);
    for (const CountryData& countryData: vecCountryData) {
        const QString strQ = QString("('%1', '%2', '%3'),").arg(countryData.name, countryData.mcc, countryData.code);
        strQuery.append(strQ);
    }
    strQuery.replace(strQuery.size() - 1, 1, ";");
    return vecCountryData.isEmpty() ? QString{} : strQuery;
}

QString DbCreator::createOperatorsQuery(const QVector<OperatorData> &vecOperatorData)
{
    QString strQuery = QString("INSERT INTO %1 (name, mcc, mnc) VALUES ").arg(table_operators);
    for (const OperatorData& operatorData: vecOperatorData) {
        const QString strQ = QString("('%1', '%2', '%3'),").arg(operatorData.name, operatorData.mcc, operatorData.mnc);
        strQuery.append(strQ);
    }
    strQuery.replace(strQuery.size() - 1, 1, ";");
    return vecOperatorData.isEmpty() ? QString{} : strQuery;
}

bool DbCreator::runQuery(const QString &strQuery)
{
    QSqlQuery query(db);
    query.prepare(strQuery);
    if(query.exec()){
        return true;
    }
    qWarning() << __FUNCTION__ << "failed: " << query.lastError() << Qt::endl;
    return false;
}
