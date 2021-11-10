#pragma once

#include <QObject>
#include <QDebug>

#include "itemdata.h"

class CustomType : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString icon READ icon       WRITE setIcon       NOTIFY iconChanged      )
    Q_PROPERTY(QString name READ name       WRITE setName       NOTIFY nameChanged      )
    Q_PROPERTY(QString mcc  READ mcc        WRITE setMcc        NOTIFY mccChanged       )
    Q_PROPERTY(QString mnc  READ mnc        WRITE setMnc        NOTIFY mncChanged       )
    Q_PROPERTY(QString code READ code       WRITE setCode       NOTIFY codeChanged      )
    Q_PROPERTY(int typeData READ typeData   WRITE setTypeData   NOTIFY typeDataChanged  )
public:
    enum TypeData{
        NoneType            = 0,
        CountryDataType     ,
        OperatorDataType    ,
    };
    Q_ENUM(TypeData)

    explicit CustomType(QObject *parent = nullptr);
    explicit CustomType(const CountryData &countryData, QObject *parent = nullptr);
    explicit CustomType(const OperatorData &operatorData, QObject *parent = nullptr);
    CustomType(const CustomType &other);
    ~CustomType();

    void setIcon    (QString value);
    void setName    (QString value);
    void setMcc     (QString value);
    void setMnc     (QString value);
    void setCode    (QString value);
    void setTypeData(int value);

    QString icon    ();
    QString name    ();
    QString mcc     ();
    QString mnc     ();
    QString code    ();
    int typeData    ();

signals:
    void iconChanged();
    void nameChanged();
    void mccChanged();
    void mncChanged();
    void codeChanged();
    void typeDataChanged();

private:
    QString _icon   ;
    QString _name   ;
    QString _mcc    ;
    QString _mnc    ;
    QString _code   ;
    int _typeData   ;
};

Q_DECLARE_METATYPE(CustomType)
