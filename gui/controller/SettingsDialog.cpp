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

#include "SettingsDialog.h"
#include "controller/SettingsDialog_p.h"
#include "view/ui_settingsdialog.h"

using namespace WKB;
using namespace WKB::widgets;
using namespace WKB::impldetail;


SettingsDialog::SettingsDialog(QWidget* parent) Q_DECL_NOEXCEPT:
    QDialog(parent),
    d_ptr(new SettingsDialogPrivate(this)),
    _ui(new Ui::SettingsDialog()){
}

SettingsDialog::~SettingsDialog(){
    delete d_ptr;
}

void SettingsDialog::accept(){
    Q_D(SettingsDialog);
    d->accept();
    QDialog::accept();
}

void SettingsDialog::reject(){
    QDialog::reject();
    return;
}

void SettingsDialog::onButtonClicked(QAbstractButton *button){
    Q_D(SettingsDialog);
    QDialogButtonBox::StandardButton standardButton =
        this->_ui->buttonBox->standardButton(button);

    switch(standardButton){
        case QDialogButtonBox::Apply:
            d->onApply();
            break;
        case QDialogButtonBox::RestoreDefaults:
            d->onRestoreDefaults();
            break;
        default:
            break;
    }
}

#include "moc_SettingsDialog_p.cpp"
