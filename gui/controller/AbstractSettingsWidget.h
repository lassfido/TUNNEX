//
//  AbstractSettingsWidget.hpp
//  tunnex
//
//  Created by Henrik Quanz on 24.09.21.
//

#ifndef AbstractSettingsWidget_hpp
#define AbstractSettingsWidget_hpp

#include<QWidget>

namespace WKB::widgets {
    class AbstractSettingsWidget : public QWidget {
        Q_OBJECT
    public:
        AbstractSettingsWidget(QWidget *parent = nullptr):
        QWidget(parent){

        }

    public slots:
        virtual void commitToSettings() = 0;
        virtual void resetToDefaults() = 0;
    };
}

//#include "moc_AbstractSettingsWidget.cpp"

#endif /* AbstractSettingsWidget_hpp */
