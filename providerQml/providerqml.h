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
#include <QModelIndex>

class ProviderQml : public QObject
{
    Q_OBJECT
public:
    explicit ProviderQml(QObject *parent = nullptr);

    Q_INVOKABLE void emptySlot(const int mcc, const int mnc);

signals:
    void signalChangeOperatorName(QModelIndex index, const QString &mcc, const QString &mnc, const QString &oldName, const QString &newName);
    void signalCreateNewOperator(const QString &mcc, const QString &mnc, const QString &name);
};

