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

#ifndef HostSettings_h
#define HostSettings_h

#include "controller/AbstractSettingsWidget.h"

// Forward Declaration
namespace WKB::widgets::Ui {
    class HostSettings;
}

namespace WKB::impldetail {
    class HostSettingsPrivate;

}

namespace WKB::widgets {
    /// Used for reading and writing the Paramters in settings using the Array interface
    struct HostParameter {
        QString serverName;
        QString hostName;
        QString username;
        QString password;
        quint16 port = 22;
    };

    using HostParameters = QList<HostParameter>;

    /// Widget for setting ssh hosts and test the connection
    class HostSettings: public AbstractSettingsWidget {
        Q_OBJECT

        public:
        HostSettings(QWidget* parent);


        public slots:
        void onServerlistViewchanged();
        void on_ClearButton_clicked();
        void on_Hostname_textChanged();
        void on_OkButton_clicked();
        void on_Password_textChanged();
        void on_Port_textChanged();
        void on_RemoveButton_clicked();
        void on_ServerName_textChanged();
        void on_Username_textChanged();
        void on_testConnectButton_clicked();
        virtual void commitToSettings() override;
        virtual void resetToDefaults() override;

    private:
        Ui::HostSettings *_ui;
        Q_DECLARE_PRIVATE(WKB::impldetail::HostSettings);
        impldetail::HostSettingsPrivate *d_ptr;
    };
}


#endif /* HostSettings_h */
