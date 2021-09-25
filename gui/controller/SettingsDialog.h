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

#ifndef SettingsDialog_hpp
#define SettingsDialog_hpp

#include <QDialog>
#include <QWidget>

// Forward Declarations
QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

namespace WKB {
    namespace widgets{
        namespace Ui{
            class SettingsDialog;
        }
    }

    namespace impldetail {
        class SettingsDialogPrivate;
    }
}

namespace WKB {
    namespace widgets {

    class SettingsDialog : public QDialog{
        Q_OBJECT
    public:
        SettingsDialog(QWidget *parent = nullptr) Q_DECL_NOEXCEPT;
        ~SettingsDialog();

    public slots:
        void accept() override;
        void reject() override;
        void onButtonClicked(QAbstractButton* button);

    private:
        Ui::SettingsDialog *_ui;
        Q_DECLARE_PRIVATE (WKB::impldetail::SettingsDialog);
        WKB::impldetail::SettingsDialogPrivate *d_ptr;

    };
}
}

#endif /* SettingsDialog_hpp */
