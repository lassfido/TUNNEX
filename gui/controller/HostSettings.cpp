//
//  HostSettings.cpp
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//


#include "HostSettings.h"
#include "HostSettings_p.h"


using namespace WKB::widgets;

HostSettings::HostSettings(QWidget* parent):
    AbstractSettingsWidget(parent),
    d_ptr(new impldetail::HostSettingsPrivate(this)),
    _ui(new Ui::HostSettings)
{

}

void HostSettings::on_ServerName_textChanged(){
    Q_D(HostSettings);
    d->on_ServerName_textedChanged();
    d->inputChanged();
}

void HostSettings::on_Hostname_textChanged(){
    Q_D(HostSettings);
    d->on_Hostname_textChanged();
    d->inputChanged();
}

void HostSettings::on_Password_textChanged(){
    Q_D(HostSettings);
    d->on_Password_textChanged();
}

void HostSettings::on_Port_textChanged(){
    Q_D(HostSettings);
    d->on_Port_textChanged();
}

void HostSettings::on_OkButton_clicked(){
    Q_D(HostSettings);
    d->on_OkButton_clicked();
}

void HostSettings::on_removeButton_clicked(){
    Q_D(HostSettings);
    d->on_removeButton_clicked();
}

void HostSettings::onServerlistViewchanged(){
    Q_D(HostSettings);
    d->on_ServerlistView_changed();
}

void HostSettings::commitToSettings() {
    Q_D(HostSettings);
    d->submitDataToSettings();
}

void HostSettings::resetToDefaults() {
    Q_D(HostSettings);
    d->resetToDefaults();
}

void HostSettings::on_Username_textChanged() { 
    Q_D(HostSettings);
    d->on_Username_textChanged();
}




#include "moc_HostSettings_p.cpp"
