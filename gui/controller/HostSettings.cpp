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


#include "HostSettings.h"
#include "HostSettings_p.h"


using namespace WKB::widgets;

HostSettings::HostSettings(QWidget* parent):
    AbstractSettingsWidget(parent),
    d_ptr(new impldetail::HostSettingsPrivate(this)),
    _ui(new Ui::HostSettings)
{

}

void HostSettings::onServerlistViewchanged(){
    Q_D(HostSettings);
    d->on_ServerlistView_changed();
}

void HostSettings::on_ClearButton_clicked(){
    Q_D(HostSettings);
    d->on_ClearButton_clicked();
}

void HostSettings::on_Hostname_textChanged(){
    Q_D(HostSettings);
    d->on_Hostname_textChanged();
    d->inputChanged();
}

void HostSettings::on_OkButton_clicked(){
    Q_D(HostSettings);
    d->on_OkButton_clicked();
}

void HostSettings::on_Password_textChanged(){
    Q_D(HostSettings);
    d->on_Password_textChanged();
}

void HostSettings::on_Port_textChanged(){
    Q_D(HostSettings);
    d->on_Port_textChanged();
}

void HostSettings::on_RemoveButton_clicked(){
    Q_D(HostSettings);
    d->on_removeButton_clicked();
}

void HostSettings::on_ServerName_textChanged(){
    Q_D(HostSettings);
    d->on_ServerName_textChanged();
    d->inputChanged();
}

void HostSettings::on_Username_textChanged() {
    Q_D(HostSettings);
    d->on_Username_textChanged();
}

void HostSettings::on_testConnectButton_clicked() {
    Q_D(HostSettings);
    d->on_testConnectButton_clicked();
}

void HostSettings::commitToSettings() {
    Q_D(HostSettings);
    d->submitDataToSettings();
}

void HostSettings::resetToDefaults() {
    Q_D(HostSettings);
    d->resetToDefaults();
}






#include "moc_HostSettings_p.cpp"
