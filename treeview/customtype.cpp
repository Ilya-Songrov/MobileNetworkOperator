#include "customtype.h"

CustomType::CustomType(QObject *parent)
    : QObject(parent)
    , _typeData(NoneType)
{

}

CustomType::CustomType(const CountryData &countryData, QObject *parent)
    : QObject(parent)
    , _icon (countryData.icon)
    , _name (countryData.name)
    , _mcc  (countryData.mcc)
    , _mnc  ()
    , _code (countryData.code)
    , _typeData(CountryDataType)
{

}

CustomType::CustomType(const OperatorData &operatorData, QObject *parent)
    : QObject(parent)
    , _icon (operatorData.icon)
    , _name (operatorData.name)
    , _mcc  (operatorData.mcc)
    , _mnc  (operatorData.mnc)
    , _code ()
    , _typeData(OperatorDataType)
{

}

CustomType::CustomType(const CustomType &other) : QObject(other.parent())
{
    _icon       = other._icon       ;
    _name       = other._name       ;
    _mcc        = other._mcc        ;
    _mnc        = other._mnc        ;
    _code       = other._code       ;
    _typeData   = other._typeData   ;
}

CustomType::~CustomType()
{

}

void CustomType::setIcon(QString value)
{
    _icon = value;
    emit iconChanged();
}

void CustomType::setName(QString value)
{
    _name = value;
    emit nameChanged();
}

void CustomType::setMcc(QString value)
{
    _mcc = value;
    emit mccChanged();
}

void CustomType::setMnc(QString value)
{
    _mnc = value;
    emit mncChanged();
}

void CustomType::setCode(QString value)
{
    _code = value;
    emit codeChanged();
}

void CustomType::setTypeData(int value)
{
    _typeData = value;
    emit typeDataChanged();
}

QString CustomType::icon()
{
    return _icon;
}

QString CustomType::name()
{
    return _name;
}

QString CustomType::mcc()
{
    return _mcc;
}

QString CustomType::mnc()
{
    return _mnc;
}

QString CustomType::code()
{
    return _code;
}

int CustomType::typeData()
{
    return _typeData;
}


