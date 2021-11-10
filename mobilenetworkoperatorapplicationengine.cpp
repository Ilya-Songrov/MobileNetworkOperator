#include "mobilenetworkoperatorapplicationengine.h"

MobileNetworkOperatorApplicationEngine::MobileNetworkOperatorApplicationEngine(QObject *parent)
    : QQmlApplicationEngine(parent)
    , treeModel(new TreeModel())
    , providerQml(new ProviderQml())
    #ifdef QT_DEBUG
    , managerDb(new ManagerDb("mcc-mnc-database.db"))
    , dbCreator(new DbCreator())
    #else
    , managerDb(new ManagerDb("mcc-mnc-database.db"))
    #endif
{
#ifdef QT_DEBUG
    dbCreator->createDB(managerDb->getPathDB());
#endif
    setConnections();
    setQmlProperty();
    loadQml();
    start();
}

MobileNetworkOperatorApplicationEngine::~MobileNetworkOperatorApplicationEngine()
{

}

void MobileNetworkOperatorApplicationEngine::changeOperatorName(QModelIndex index, const QString &mcc, const QString &mnc, const QString &oldName, const QString &newName)
{
    const bool ret = managerDb->changeOperatorName(mcc, mnc, oldName, newName);
    if (ret) {
        treeModel->updateOperatorData(index, OperatorData{createOperatorIcon(mcc, mnc), newName, mcc, mnc});
    }
    else{
        QMessageBox::information(nullptr, "Info", "Error while saving to database");
    }
}

void MobileNetworkOperatorApplicationEngine::createNewOperator(const QString &mcc, const QString &mnc, const QString &name)
{
    QString error;
    bool success = managerDb->mccExist(mcc);
    if (success) {
        success = managerDb->saveNewOperator(mcc, mnc, name);
        if (success) {
            treeModel->insertNewOperatorData(OperatorData{createOperatorIcon(mcc, mnc), name, mcc, mnc});
        }
        else{
            error = "Error while saving to database";
        }
    }
    else{
        error = "Error! There is no such country in the database. You cannot create this item.";
    }

    if (!success) {
        QMessageBox::information(nullptr, "Info", error);
    }
}

void MobileNetworkOperatorApplicationEngine::setConnections()
{
    connect(providerQml.get(), &ProviderQml::signalChangeOperatorName, this, &MobileNetworkOperatorApplicationEngine::changeOperatorName);
    connect(providerQml.get(), &ProviderQml::signalCreateNewOperator, this, &MobileNetworkOperatorApplicationEngine::createNewOperator);
}

void MobileNetworkOperatorApplicationEngine::setQmlProperty()
{
    qmlRegisterType<CustomType>("MobileNetworkOperator.CustomType", 1, 0, "CustomType");

    this->rootContext()->setContextProperty("rectScreen", getRectScreen());
    this->rootContext()->setContextProperty("treeModel", treeModel.get());
    this->rootContext()->setContextProperty("providerQml", providerQml.get());
}

void MobileNetworkOperatorApplicationEngine::loadQml()
{
#ifdef QT_DEBUG
    const QUrl url(QUrl::fromLocalFile(QFileInfo(__FILE__).dir().absolutePath() + "/resources/qml/main.qml"));
#else
    const QUrl url(QStringLiteral("qrc:/qml/qml/main.qml"));
#endif
    this->load(url);
    if (this->rootObjects().isEmpty()){
        qCritical() << "Qml files were not loaded" << Qt::endl;
        emit exit(-1);
    }
}

void MobileNetworkOperatorApplicationEngine::start()
{
    const bool ret = managerDb->loadDatabase();
    if (!ret) {
        const QMessageBox::StandardButton standardButton = QMessageBox::question(nullptr, "Warning", "Database was not loaded. Do you want to continue?");
        if (standardButton != QMessageBox::Yes) {
            emit exit(EXIT_FAILURE);
            return;
        }
    }
    const auto vecCountries = managerDb->getAllCountryData();
    const auto vecOperators = managerDb->getAllOperatorData();
    QVector<NodeData> vecNodes;
    for (const auto &country: vecCountries) {
        NodeData node;
        node.countryData = country;
        for (const auto &oper : vecOperators) {
            if (oper.mcc == country.mcc) {
                node.vecOperators.append(oper);
            }
        }
        vecNodes.append(node);
    }
    treeModel->loadData(vecNodes);
}

QRect MobileNetworkOperatorApplicationEngine::getRectScreen()
{
    const auto *primaryScreen = QGuiApplication::primaryScreen();
    QRect rect(QPoint(), QSize(3, 3) * 222);
    rect.moveCenter(primaryScreen ? primaryScreen->geometry().center() : QPoint());
    return rect;
}
