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

#pragma once

#include <QObject>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "itemdata.h"

class DbCreator : public QObject
{
    Q_OBJECT
public:
    explicit DbCreator(QObject *parent = nullptr);
    ~DbCreator();

    bool createDB(const QString &pathFile);

private:
    bool createTablesFromJsonFile();
    void readJsonFile(QVector<CountryData>* vecCountryData, QVector<OperatorData>* vecOperatorData);
    bool createCountriesTable();
    bool createOperatorsTable();

    QString createCountiesQuery(const QVector<CountryData>& vecCountryData);
    QString createOperatorsQuery(const QVector<OperatorData>& vecOperatorData);
    bool runQuery(const QString& strQuery);


private:
    QSqlDatabase db;
    const QString table_countries;
    const QString table_operators;
};

