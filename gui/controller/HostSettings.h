//
//  HostSettings.hpp
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//

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
    // Used for reading and writing the HostParameter Settings
    struct HostParameter {
        QString serverName;
        QString hostName;
        QString username;
        QString password;
        QString port = QString("22");
    };

    using HostParameters = QList<HostParameter>;

    class HostSettings: public AbstractSettingsWidget {
        Q_OBJECT

        public:
        HostSettings(QWidget* parent);


        public slots:
        void on_ServerName_textChanged();
        void on_Hostname_textChanged();
        void on_Password_textChanged();
        void on_Port_textChanged();
        void on_Username_textChanged();
        void on_OkButton_clicked();
        void on_removeButton_clicked();
        void onServerlistViewchanged();
        virtual void commitToSettings() override;
        virtual void resetToDefaults() override;

    private:
        Ui::HostSettings *_ui;
        Q_DECLARE_PRIVATE(WKB::impldetail::HostSettings);
        impldetail::HostSettingsPrivate *d_ptr;
    };
}


#endif /* HostSettings_h */
