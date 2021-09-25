//
//  SettingsListModel.cpp
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//

#include "SettingsListModel.h"
#include "SettingsListModel_p.h"

using namespace WKB::model;

SettingsListModel::SettingsListModel(QObject *parent):
    QAbstractListModel(parent),
    d_ptr(new WKB::impldetail::SettingsListModelPrivate(this)){
    
    }

QSharedPointer<widgets::AbstractSettingsWidget> SettingsListModel::getWidget(const QModelIndex &parent) { 
    Q_D(SettingsListModel);
    return d->getWidget(parent);
}

QVariant SettingsListModel::data(const QModelIndex &parent, int role) const { 
    const Q_D(SettingsListModel);
    return d->data(parent,role);
}

int SettingsListModel::rowCount(const QModelIndex &parent) const { 
    const Q_D(SettingsListModel);
    return d->rowCount();
}

void SettingsListModel::setWidget(const QVariant &value,  widgets::AbstractSettingsWidget *widget) {
    Q_D(SettingsListModel);
    d->setWidget(value, widget);
}




