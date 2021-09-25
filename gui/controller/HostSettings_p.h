//
//  HostSettings_p.h
//  tunnex
//
//  Created by Henrik Quanz on 17.09.21.
//

#ifndef HostSettings_p_h
#define HostSettings_p_h

#include "HostSettings.h"
#include "view/ui_hostsettings.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QSettings>
#include <QStringListModel>

namespace WKB::impldetail {

    class HostSettingsPrivate : public QObject {
        Q_OBJECT
    public:
        HostSettingsPrivate(widgets::HostSettings *p);
        void on_ServerName_textedChanged();
        void on_Hostname_textChanged();
        void on_Password_textChanged();
        void on_Port_textChanged();
        void on_Username_textChanged();
        void submitDataToSettings();
        void resetToDefaults();

        void on_OkButton_clicked();
        void on_removeButton_clicked();
        void on_ServerlistView_changed();


    public slots:
        void on_Severlistview_selectionModel_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    private:
        Q_DECLARE_PUBLIC(widgets::HostSettings);
        QSettings _settings;
        widgets::HostSettings *q_ptr;
        QStringListModel *_serverNamesModel;
        widgets::HostParameters _hostParameters;
        bool areInputsFromEditsVaid();
        void activateButtons(bool activate);
        void inputChanged();
        void setupValidators(WKB::widgets::HostSettings *q);
        void updateHostListView();
        void updateHostParameters();
    };
}

using namespace WKB::impldetail;

HostSettingsPrivate::HostSettingsPrivate(widgets::HostSettings *p):
    q_ptr(p),
    _serverNamesModel(new QStringListModel()){
        Q_Q(WKB::widgets::HostSettings);
        q->_ui->setupUi(q);

        // Setup validators for hostname
        setupValidators(q);

        // Setup List Widget model
        QListView* serverListview = q->_ui->ServerlistView;
        serverListview->setModel(_serverNamesModel);

        // Read in list of hosts from settings and update model
        updateHostParameters();

        // Connect the signals from view
        QItemSelectionModel *selectionModel = q->_ui->ServerlistView->selectionModel();
        connect(selectionModel, &QItemSelectionModel::currentChanged,
                this, &HostSettingsPrivate::on_Severlistview_selectionModel_currentChanged);
}

void HostSettingsPrivate::updateHostParameters() {
    Q_Q(widgets::HostSettings);
    int size = _settings.beginReadArray("Hosts");
    for (int i = 0; i < size; i++) {
        widgets::HostParameter param;
        _settings.setArrayIndex(i);
        param.serverName = _settings.value("serverName").toString();
        param.hostName = _settings.value("hostName").toString();
        param.password = _settings.value("password").toString();
        param.username = _settings.value("username").toString();
        param.port = _settings.value("port").toString();
        _hostParameters.append(param);
    }
    _settings.endArray();
    updateHostListView();
}

void HostSettingsPrivate::setupValidators(WKB::widgets::HostSettings *q) {
    QString validIPAddressRegex("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

    QString validHostnameRegex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");
    QRegExp validatorRegex(validIPAddressRegex + "|" + validHostnameRegex);

    QRegExpValidator *validator = new QRegExpValidator(validatorRegex);
    q->_ui->Hostname->setValidator(validator);

    // Set validator for server name
    QRegExp stringValidator("^[a-zA-Z0-9]+$");
    validator = new QRegExpValidator(stringValidator);
    q->_ui->ServerName->setValidator(validator);
}

void HostSettingsPrivate::updateHostListView() {
    Q_Q(widgets::HostSettings);
    QStringList listToBeUpdated = _serverNamesModel->stringList();
    for (widgets::HostParameter &param : _hostParameters ) {
        QString serverName = param.serverName;
        if(listToBeUpdated.contains(serverName))
            continue;
        listToBeUpdated.append(serverName);
    }

    _serverNamesModel->setStringList(listToBeUpdated);

}

void HostSettingsPrivate::on_ServerName_textedChanged(){
    Q_Q(widgets::HostSettings);
}

void HostSettingsPrivate::on_Hostname_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_Password_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_Port_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_Username_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::submitDataToSettings() {
    Q_Q(widgets::HostSettings);

    _settings.beginWriteArray("Hosts");
    for(int i = 0; i < _hostParameters.size(); i++){
        _settings.setArrayIndex(i);
        // If yes modify content of array
        _settings.setValue("serverName", _hostParameters[i].serverName);
        _settings.setValue("hostName", _hostParameters[i].hostName);
        _settings.setValue("password", _hostParameters[i].password);
        _settings.setValue("username", _hostParameters[i].username);
        _settings.setValue("port", _hostParameters[i].port);
    }
    _settings.endArray();
    qDebug() << q->objectName() << " settings saved!";
}

void HostSettingsPrivate::resetToDefaults(){
    _hostParameters.clear();
    _serverNamesModel->setStringList(QStringList());
    updateHostParameters();
}

void HostSettingsPrivate::on_OkButton_clicked(){
    Q_Q(widgets::HostSettings);
    QLineEdit *serverNameEdit, *hostnameEdit, *passwordEdit, *portEdit, *usernameEdit;
    widgets::HostParameter newHost;

    serverNameEdit = q->_ui->ServerName;
    hostnameEdit = q->_ui->Hostname;
    passwordEdit = q->_ui->Password;
    portEdit = q->_ui->Port;
    usernameEdit = q->_ui->Username;


    newHost.serverName = serverNameEdit->text();
    newHost.hostName = hostnameEdit->text();

    // Check port, username, and password for input
    if(portEdit->text().length() != 0)
        newHost.port = portEdit->text();

    if(passwordEdit->text() != 0)
        newHost.password = passwordEdit->text();

    if(usernameEdit->text() != 0)
        newHost.username = usernameEdit->text();

    int serverNameIndex = _hostParameters.size();
    int i = 0;
    bool serverNameExists = false;
    for(widgets::HostParameter &param : _hostParameters){
        if(param.serverName == newHost.serverName){
            serverNameIndex = i;
            serverNameExists = true;
            break;
        }
        i++;
    }

    if(serverNameExists)
        _hostParameters[serverNameIndex] = newHost;
    else
        _hostParameters.append(newHost);

    updateHostParameters();

    q->_ui->ServerlistView->selectionModel()->select( _serverNamesModel->index(serverNameIndex), QItemSelectionModel::ClearAndSelect);
}

void HostSettingsPrivate::on_removeButton_clicked(){
    Q_Q(widgets::HostSettings);
    QItemSelectionModel *selectedItems = q->_ui->ServerlistView->selectionModel();
    // Update model and view
    int rowSelected = selectedItems->currentIndex().row();
    _serverNamesModel->removeRow(rowSelected);
    _hostParameters.removeAt(rowSelected);
    updateHostListView();
}

void HostSettingsPrivate::on_ServerlistView_changed(){
    qDebug() << "Server List View changed";
}

bool HostSettingsPrivate::areInputsFromEditsVaid() {
    Q_Q(widgets::HostSettings);
    QList<QLineEdit *> lineEdits = q->findChildren<QLineEdit *>();
    for(auto lineEdit : lineEdits){
        if(!lineEdit->hasAcceptableInput())
            return false;
    }

    return true;
}

void HostSettingsPrivate::activateButtons(bool activate){
    Q_Q(widgets::HostSettings);
    q->_ui->OkButton->setEnabled(activate);
    q->_ui->removeButton->setEnabled(activate);
}

void HostSettingsPrivate::inputChanged(){
    Q_Q(widgets::HostSettings);
    if(areInputsFromEditsVaid())
        activateButtons(true);
    else
        activateButtons(false);
}

void HostSettingsPrivate::on_Severlistview_selectionModel_currentChanged(const QModelIndex &current,const QModelIndex &previous) {
    Q_UNUSED(previous);
    Q_Q(widgets::HostSettings);
    if (!current.isValid())
        return;

    // Get the data: the server name
    QString serverName = current.data().toString();
    // Display the data corresponding to serverName
    for(const widgets::HostParameter &param : _hostParameters){
        if(param.serverName == serverName){
            q->_ui->ServerName->setText(param.serverName);
            q->_ui->Hostname->setText(param.hostName);

            q->_ui->Password->clear();
            if(!param.password.isEmpty())
                q->_ui->Password->setText(param.password);

            q->_ui->Port->clear();
            if(param.port != QString("22"))
                q->_ui->Port->setText(param.port);
            
            q->_ui->Username->clear();
            if(!param.username.isEmpty())
                q->_ui->Username->setText(param.username);
        }
    }
}



#endif /* HostSettings_p_h */





