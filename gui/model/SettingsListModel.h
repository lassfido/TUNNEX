//
//  SettingsListModel.h
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//

#ifndef SettingsListModel_h
#define SettingsListModel_h

#include<QAbstractListModel>


// Forward Declaration
namespace WKB::impldetail {
    class SettingsListModelPrivate;
}

namespace WKB::widgets {
    class AbstractSettingsWidget;
}

namespace WKB::model {
    class SettingsListModel : public QAbstractListModel {

    public:
        SettingsListModel(QObject *parent = nullptr);
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &parent = QModelIndex(),
                      int role = Qt::DisplayRole) const override;
        QSharedPointer<widgets::AbstractSettingsWidget> getWidget(const QModelIndex &parent);
        void setWidget(const QVariant &value,
                        widgets::AbstractSettingsWidget *widget);

    private:
        Q_DECLARE_PRIVATE(WKB::impldetail::SettingsListModel);
        WKB::impldetail::SettingsListModelPrivate *d_ptr;

    };
}


#endif /* SettingsListModel_h */
