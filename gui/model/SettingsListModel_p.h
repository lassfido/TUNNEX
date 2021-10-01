//
//  SettingsListModel_p.h
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//

#ifndef SettingsListModel_p_h
#define SettingsListModel_p_h

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QDebug>
#include "SettingsListModel.h"
#include "controller/AbstractSettingsWidget.h"

namespace WKB::impldetail {
    class SettingsListModelPrivate : public QObject {
    public:
        SettingsListModelPrivate(model::SettingsListModel *p);
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &parent = QModelIndex(),
                      int role = Qt::DisplayRole) const;
        QSharedPointer<widgets::AbstractSettingsWidget> getWidget(const QModelIndex &parent) const;
        void setWidget(const QVariant &value,
                       widgets::AbstractSettingsWidget *widget);

    private:
        Q_DECLARE_PUBLIC(model::SettingsListModel);
        model::SettingsListModel *q_ptr;
        QMap<QString,QSharedPointer<widgets::AbstractSettingsWidget>> _data;
    };
}

using namespace WKB;
using namespace WKB::impldetail;

SettingsListModelPrivate::SettingsListModelPrivate(model::SettingsListModel *p):
    q_ptr(p){
}

int SettingsListModelPrivate::rowCount(const QModelIndex &parent) const {
    return _data.count();
}


QVariant SettingsListModelPrivate::data(const QModelIndex &parent, int role) const {
    if(!parent.isValid())
        return QVariant();
    switch (role) {
        case Qt::DisplayRole:
            return _data.keys()[parent.row()];
            break;

        default:
            return QVariant();
            break;
    }
}

QSharedPointer<widgets::AbstractSettingsWidget> SettingsListModelPrivate::getWidget(const QModelIndex &parent) const {
    if(!parent.isValid())
        return nullptr;

    QString key = _data.keys()[parent.row()];
    return _data[key];
}


void SettingsListModelPrivate::setWidget(const QVariant &value,  widgets::AbstractSettingsWidget* widget) {
    _data[value.toString()] = QSharedPointer<widgets::AbstractSettingsWidget>(widget);
}


#endif /* SettingsListModel_p_h */


