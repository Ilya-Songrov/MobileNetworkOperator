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

#include "providerqml.h"

ProviderQml::ProviderQml(QObject *parent) : QObject(parent)
{

}

void ProviderQml::emptySlot(const int mcc, const int mnc)
{
    qDebug() << "function:" << Q_FUNC_INFO << mcc << mnc << Qt::endl;
}
