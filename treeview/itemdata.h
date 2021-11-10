#pragma once

#include <QString>
#include <QVector>
#include <QVariant>
#include <QFile>

struct CountryData{
    QString icon        ;
    QString name        ;
    QString mcc         ;
    QString code        ;
    bool operator==(const CountryData& c) { return c.icon == this->icon &&
                c.name == this->name &&
                c.mcc == this->mcc &&
                c.code == this->code; }
};

struct OperatorData{
    QString icon        ;
    QString name        ;
    QString mcc         ;
    QString mnc         ;
};

struct NodeData{
    CountryData             countryData;
    QVector<OperatorData>   vecOperators;
};


const QString question_icon  { "qrc:/otherImages/resources/otherImages/3017955_examination_inquiry_interrogation_investigation_poll_icon.svg" };
inline QString createCountryIcon(const QString &str_code)
{
    const QString folder    = "qrc:/countries/country-flag-icons/flags/1x1/";
    const QString path      = folder + str_code.toUpper() + ".svg";
    return QFile::exists(path.right(path.size() - 3)) ? path : question_icon;
}

inline QString createOperatorIcon(const QString &str_mcc, const QString &str_mnc)
{
    const QString folder    = "qrc:/operators/resources/operators/";
    const QString path      = folder + str_mcc + "_" + str_mnc + ".png";
    return QFile::exists(path.right(path.size() - 3)) ? path : question_icon;
}
