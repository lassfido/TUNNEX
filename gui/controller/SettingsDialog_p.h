/* This file is part of TUNNEX.

 TUNNEX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 TUNNEX is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

 TUNNEX 1.0.0
 Copyright (c) Henrik Quanz
*/

#ifndef SettingsDialog_p_h
#define SettingsDialog_p_h

#include "controller/SettingsDialog.h"
#include "view/ui_settingsdialog.h"
#include "model/SettingsListModel.h"
#include "controller/HostSettings.h"
#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QStringListModel>
#include <QStringList>
#include <QSharedPointer>
#include <QHBoxLayout>
#include <QModelIndexList>


using SharedQWidget = QSharedPointer<QWidget>;

namespace WKB {
    namespace impldetail{
        class SettingsDialogPrivate : public QObject{
            Q_OBJECT
            private:
                Q_DECLARE_PUBLIC (WKB::widgets::SettingsDialog);
                WKB::widgets::SettingsDialog *q_ptr;
                QSettings _settings;
                model::SettingsListModel *_model;
            public:
                SettingsDialogPrivate(widgets::SettingsDialog *s) Q_DECL_NOEXCEPT;
                ~SettingsDialogPrivate();
                void accept();
                void onApply();
                void onRestoreDefaults();
                void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        };
    }
}

//*****************************************************************
// Implementation
//*****************************************************************

using namespace WKB::impldetail;

SettingsDialogPrivate::SettingsDialogPrivate(WKB::widgets::SettingsDialog *s)
Q_DECL_NOEXCEPT: q_ptr(s),
    _model(new model::SettingsListModel())
    {
    Q_Q(WKB::widgets::SettingsDialog);
    q->_ui->setupUi(q);


    // Setup Model
    widgets::HostSettings *hostSettings = new widgets::HostSettings(q);
        hostSettings->hide();
    _model->setWidget(tr("Host Settings"),
                      hostSettings);

    q->_ui->settingsList->setModel(_model);

    // Connection to selection model
    QItemSelectionModel *selectionModel = q->_ui->settingsList->selectionModel();

    connect(selectionModel,
            &QItemSelectionModel::selectionChanged,
            this,
            &SettingsDialogPrivate::on_selectionChanged);
}

SettingsDialogPrivate::~SettingsDialogPrivate(){
    delete _model;
}

void SettingsDialogPrivate::accept(){
    Q_Q(widgets::SettingsDialog);
    this->onApply();
}

void SettingsDialogPrivate::onRestoreDefaults(){
    Q_Q(widgets::SettingsDialog);
    this->_settings.clear();
    QList<widgets::AbstractSettingsWidget*> widgets = q->findChildren<widgets::AbstractSettingsWidget*>();
    for(widgets::AbstractSettingsWidget *widget : widgets)
        widget->resetToDefaults();

}

void SettingsDialogPrivate::onApply(){
    Q_Q(widgets::SettingsDialog);
    QList<widgets::AbstractSettingsWidget*> widgets = q->findChildren<widgets::AbstractSettingsWidget*>();
    for(widgets::AbstractSettingsWidget *widget : widgets){
        widget->commitToSettings();
    }

}

void SettingsDialogPrivate::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_Q(widgets::SettingsDialog);

    QModelIndexList selectedItems = selected.indexes();
    QModelIndexList deselectedItems = deselected.indexes();

    // Check if selection is not empty
    if(selectedItems.count() == 0)
        return;

    // Check if we need to remove the widget first
    if(deselectedItems.count() >= 1){
        QModelIndex indexDeselected = deselectedItems.first();
        QSharedPointer<QWidget> deselectedWidget = _model->getWidget(indexDeselected);
    }

    // Add widget to layout
    QModelIndex indexSelected = selectedItems.first();
    if(!indexSelected.isValid())
        return;

    QWidget *settingsWidget = q->_ui->settingsWidget;
    QSharedPointer<QWidget> toBeDisplayed = _model->getWidget(indexSelected);

    // Resize SettingsDialog to fit the current widget and reset minimum size
    QSize settingsWidgetSize = q->_ui->settingsWidget->size();
    QSize settingsWidgetMinimumSize = toBeDisplayed->minimumSize();
    if(settingsWidgetSize.width() < settingsWidgetMinimumSize.width() ||
       settingsWidgetSize.height() < settingsWidgetMinimumSize.height()){
        QSize currentDialogSize = q->size(),
        expansionNeeded = settingsWidgetMinimumSize - settingsWidgetSize;
        QSize newMinimumSize = currentDialogSize + expansionNeeded;
        q->resize(newMinimumSize);
        q->setMinimumWidth(newMinimumSize.width() + 10);
    }
    q->_ui->gridLayout->replaceWidget(settingsWidget, toBeDisplayed.get());
    toBeDisplayed->show();
}


#endif /* SettingsDialog_p_h */



