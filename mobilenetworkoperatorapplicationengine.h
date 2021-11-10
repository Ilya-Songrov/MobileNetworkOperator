#pragma once

#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QTimer>

#include "treemodel.h"
#include "managerdb.h"
#include "dbcreator.h"
#include "providerqml.h"

class MobileNetworkOperatorApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit MobileNetworkOperatorApplicationEngine(QObject *parent = nullptr);
    virtual ~MobileNetworkOperatorApplicationEngine();

private slots:
    void changeOperatorName(QModelIndex index, const QString &mcc, const QString &mnc, const QString &oldName, const QString &newName);
    void createNewOperator(const QString &mcc, const QString &mnc, const QString &name);


private:
    void setConnections();
    void setQmlProperty();
    void loadQml();
    void start();

    QRect getRectScreen();

private:
    QScopedPointer<TreeModel> treeModel;
    QScopedPointer<ProviderQml> providerQml;
    QScopedPointer<ManagerDb> managerDb;
#ifdef QT_DEBUG
    QScopedPointer<DbCreator> dbCreator;
#endif
};
