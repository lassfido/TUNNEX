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

#ifndef AbstractSettingsWidget_hpp
#define AbstractSettingsWidget_hpp

#include <QWidget>

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
