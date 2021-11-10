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
#include <QFile>
#include <QSqlDatabase>


#include "itemdata.h"

class ManagerDb : public QObject
{
public:
    explicit ManagerDb(const QString &pathDatabase, QObject *parent = nullptr);
    ~ManagerDb();

    bool loadDatabase();

    bool changeOperatorName(const QString &mcc, const QString &mnc, const QString &oldName, const QString &newName);
    bool saveNewOperator(const QString &mcc, const QString &mnc, const QString &name);

    QVector<CountryData> getAllCountryData() const;
    QVector<OperatorData> getAllOperatorData() const;

    bool mccExist(const QString &mcc);

    QString getPathDB() const;

private:
    bool read();

private:
    QSqlDatabase db;
    const QString _pathDatabase;
    const QString key_countries;
    const QString key_operators;
};

