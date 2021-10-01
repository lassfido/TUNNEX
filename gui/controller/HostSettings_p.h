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

#ifndef HostSettings_p_h
#define HostSettings_p_h

#include "HostSettings.h"
#include "view/ui_hostsettings.h"
#include <QDebug>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QStringListModel>
#include <QErrorMessage>
#include <QThread>
#include <QTimer>
#include "QLibssh2/Ssh2Client.h"

namespace WKB::impldetail {
    class Ssh2TestConnectionThread : public QThread {
        Q_OBJECT
        QString _hostname;
        qlibssh2::Ssh2Settings _settings;
        qlibssh2::Ssh2Client::SessionStates _lastState;

    public:
        void run() override;
        void setHostname(const QString &hostname);
        void setSettings(const qlibssh2::Ssh2Settings &settings);

    signals:
        void finishedWith(const int exitCode);
        void lastState(const qlibssh2::Ssh2Client::SessionStates state);

    private slots:
        void onSessionStateChanged(const qlibssh2::Ssh2Client::SessionStates state);
        void onSSH2Error(std::error_code error);

    };

    class HostSettingsPrivate : public QObject {
        Q_OBJECT
    public:
        HostSettingsPrivate(widgets::HostSettings *p);
        ~HostSettingsPrivate();
        void on_ClearButton_clicked();
        void on_Hostname_textChanged();
        void on_OkButton_clicked();
        void on_Password_textChanged();
        void on_Port_textChanged();
        void on_ServerName_textChanged();
        void on_ServerlistView_changed();
        void on_Username_textChanged();
        void on_removeButton_clicked();
        void on_testConnectButton_clicked();

        void submitDataToSettings();
        void resetToDefaults();


    public slots:
        void on_Severlistview_selectionModel_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    protected slots:
        void activateUserInput();
        void deactivateUserInput();
        void userInputAllowed(bool allowed = true);

    private slots:
        void onLastStateChanged(const qlibssh2::Ssh2Client::SessionStates state);
        void onSsh2ThreadFinishedWith(const int exitCode);

    signals:
        void modelModified();

    private:
        Q_DECLARE_PUBLIC(widgets::HostSettings);
        QSettings _settings;
        widgets::HostSettings *q_ptr;
        QStringListModel *_serverNamesModel;
        widgets::HostParameters _hostParameters;
        Ssh2TestConnectionThread _ssh2TestThread;
        qlibssh2::Ssh2Client::SessionStates _lastState;
        bool areInputsFromEditsVaid();
        void activateButtons(bool activate);
        void inputChanged();
        void setupValidators(WKB::widgets::HostSettings *q);
        void updateHostListView();
        void readModelFromSettings();
    };
}

using namespace WKB::impldetail;


void Ssh2TestConnectionThread::run(){
    qlibssh2::Ssh2Client *ssh2Client = new qlibssh2::Ssh2Client(_settings,this);
    connect(this, &QThread::finished, ssh2Client, &qlibssh2::Ssh2Client::disconnectFromHost);
   connect(this, &QThread::finished, ssh2Client, &QObject::deleteLater);
   connect(ssh2Client,&qlibssh2::Ssh2Client::sessionStateChanged,
            this,
           &Ssh2TestConnectionThread::onSessionStateChanged
           ,Qt::BlockingQueuedConnection);
   connect(ssh2Client,&qlibssh2::Ssh2Client::ssh2Error,
            this,
           &Ssh2TestConnectionThread::onSSH2Error
           ,Qt::BlockingQueuedConnection);
    ssh2Client->connectToHost(_hostname);
    emit(finishedWith(exec()));
}

void Ssh2TestConnectionThread::setHostname(const QString &hostname){
    _hostname = hostname;
}

void Ssh2TestConnectionThread::setSettings(const qlibssh2::Ssh2Settings &settings){
    _settings = settings;
}

void Ssh2TestConnectionThread::onSessionStateChanged(const qlibssh2::Ssh2Client::SessionStates state){
    switch (state) {
        case qlibssh2::Ssh2Client::NotEstableshed:
            _lastState = state;
            break;
        case qlibssh2::Ssh2Client::StartingSession:
            _lastState = state;
            break;
        case qlibssh2::Ssh2Client::GetAuthMethods:
            _lastState = state;
            break;
        case qlibssh2::Ssh2Client::Authentication:
            _lastState = state;
            break;
        case qlibssh2::Ssh2Client::Established:
            emit(lastState(_lastState));
            quit();
            break;
        case qlibssh2::Ssh2Client::FailedToEstablish:
            emit(lastState(_lastState));
            exit(1);
            break;
        case qlibssh2::Ssh2Client::Closing:
            break;
        case qlibssh2::Ssh2Client::Closed:
            emit(lastState(_lastState));
            exit(0);
            break;
        case qlibssh2::Ssh2Client::Aborted:
            break;
        default:
            break;
    }
}

void Ssh2TestConnectionThread::onSSH2Error(std::error_code error){
    //_sshClient->disconnectFromHost();
    QErrorMessage messageDialog;
    messageDialog.showMessage(tr("SSH2 Error"), error.message().c_str());
    exit(-1);
}

HostSettingsPrivate::HostSettingsPrivate(widgets::HostSettings *p):
    q_ptr(p),
    _serverNamesModel(new QStringListModel())
    //_sshClient()
    {
        Q_Q(WKB::widgets::HostSettings);
        q->_ui->setupUi(q);

        // Setup validators for hostname
        setupValidators(q);

        // Setup List Widget model
        QListView* serverListview = q->_ui->ServerlistView;
        serverListview->setModel(_serverNamesModel);


        // Connect the signals from view
        QItemSelectionModel *selectionModel = q->_ui->ServerlistView->selectionModel();
        connect(selectionModel, &QItemSelectionModel::currentChanged,
                this, &HostSettingsPrivate::on_Severlistview_selectionModel_currentChanged);

        // Connect model and controller to update view on model change
        connect(this, &HostSettingsPrivate::modelModified,
                this, &HostSettingsPrivate::updateHostListView);

        // Read in list of hosts from settings and update model
        readModelFromSettings();
        // Setup default values as texfield hint from qlibssh2
        q->_ui->Username->setPlaceholderText(qlibssh2::defaultUser());
        q->_ui->Password->setPlaceholderText(qlibssh2::defaultKey());
        // Connect Threads
        connect(&_ssh2TestThread, &Ssh2TestConnectionThread::lastState,
            this, &HostSettingsPrivate::onLastStateChanged);
        connect(&_ssh2TestThread, &Ssh2TestConnectionThread::finishedWith,
            this,&HostSettingsPrivate::onSsh2ThreadFinishedWith);
}

HostSettingsPrivate::~HostSettingsPrivate(){
    //delete _sshClient;
    delete _serverNamesModel;
}


void HostSettingsPrivate::submitDataToSettings(){
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
    readModelFromSettings();
}

// UI Control function

void HostSettingsPrivate::on_ClearButton_clicked(){
    Q_Q(widgets::HostSettings);

    QList<QLineEdit*> lineEdits = q->findChildren<QLineEdit*>();

    for(QLineEdit* lineEdit : lineEdits)
        lineEdit->clear();

    q->_ui->ServerlistView->selectionModel()->clearSelection();
    inputChanged();

}

void HostSettingsPrivate::on_Hostname_textChanged(){
    inputChanged();
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
        newHost.port = portEdit->text().toUInt();

    if(!passwordEdit->text().isEmpty())
        newHost.password = passwordEdit->text();

    if(!usernameEdit->text().isEmpty())
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

    emit(modelModified());
    //q->_ui->ServerlistView->selectionModel()->select( _serverNamesModel->index(serverNameIndex), QItemSelectionModel::ClearAndSelect);
}

void HostSettingsPrivate::on_Password_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_Port_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_ServerName_textChanged(){
    Q_Q(widgets::HostSettings);
}

void HostSettingsPrivate::on_ServerlistView_changed(){
    qDebug() << "Server List View changed";
}

void HostSettingsPrivate::on_Username_textChanged(){
    inputChanged();
}

void HostSettingsPrivate::on_removeButton_clicked(){
    Q_Q(widgets::HostSettings);
    QItemSelectionModel *selectedItems = q->_ui->ServerlistView->selectionModel();
    if(selectedItems->selectedRows().isEmpty())
        return;
    // Update model and view
    int rowSelected = selectedItems->currentIndex().row();
    _serverNamesModel->removeRow(rowSelected);
    _hostParameters.removeAt(rowSelected);
    emit(modelModified());
}

void HostSettingsPrivate::on_testConnectButton_clicked(){
    Q_Q(widgets::HostSettings);
    qlibssh2::Ssh2Settings settings;
    QItemSelectionModel *selectedServer = q->_ui->ServerlistView->selectionModel();
    // If more than one server selected do nothing
    if(selectedServer->selectedRows().count() > 1 )
        return;

    // If nothing is selected
    if(selectedServer->selectedRows().count() == 0)
        return;

    // Get the selected Server Config
    widgets::HostParameter param = _hostParameters[selectedServer->selectedRows().first().row()];

    // Setup the settings model of qlibssh2
    if(!param.username.isEmpty())
        settings.user = param.username;

    if(param.port != 22)
        settings.port = QVariant(settings.port).toUInt();

    if(!param.password.isEmpty())
        settings.passphrase = param.password;

    if(_ssh2TestThread.isRunning()){
        _ssh2TestThread.quit();
        _ssh2TestThread.wait();
    }

    _ssh2TestThread.setSettings(settings);
    _ssh2TestThread.setHostname(param.hostName);
    _ssh2TestThread.start();
    deactivateUserInput();
    q->_ui->testConnectProgressBar->setMaximum(0);
}

// Public Slots

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
                q->_ui->Port->setText(QVariant(param.port).toString());

            q->_ui->Username->clear();
            if(!param.username.isEmpty())
                q->_ui->Username->setText(param.username);
            q->_ui->testConnectLabel->setText(tr("Not Established"));
        }
    }
}

// Protected Slots
void HostSettingsPrivate::activateUserInput(){
    userInputAllowed(true);
}

void HostSettingsPrivate::deactivateUserInput(){
    userInputAllowed(false);
}

void HostSettingsPrivate::userInputAllowed(bool allowed){

    Q_Q(widgets::HostSettings);

    // LineEdits and Buttons
    QList<QPushButton*> buttons = q->findChildren<QPushButton*>();
    QList<QLineEdit*> lineEdits = q->findChildren<QLineEdit*>();

    for(auto button : buttons)
        button->setEnabled(allowed);

    for(auto edit : lineEdits)
        edit->setEnabled(allowed);

    // Server List view
    q->_ui->ServerlistView->setEnabled(allowed);

    // If enable check for valid input and enable the remove and add button
    if(allowed){
        inputChanged();
    }
}

// Private Slots
void HostSettingsPrivate::onLastStateChanged(const qlibssh2::Ssh2Client::SessionStates state){
    _lastState = state;
}

void HostSettingsPrivate::onSsh2ThreadFinishedWith(const int exitCode){
    Q_Q(widgets::HostSettings);
    if(exitCode != 0){
        QString failedText(tr("Error: "));
        switch (_lastState) {
            case qlibssh2::Ssh2Client::NotEstableshed:
                failedText += tr("Unable to Establish");
                break;
            case qlibssh2::Ssh2Client::StartingSession:
                failedText += tr("Starting Session");
                break;
            case qlibssh2::Ssh2Client::GetAuthMethods:
                failedText += tr("Auth");
                break;
            case qlibssh2::Ssh2Client::Authentication:
                failedText += tr("Auth");
                break;
            case qlibssh2::Ssh2Client::Established:
                break;
            case qlibssh2::Ssh2Client::FailedToEstablish:
                failedText += tr("Timeout");
                break;
            case qlibssh2::Ssh2Client::Closing:
                break;
            case qlibssh2::Ssh2Client::Closed:
                failedText += tr("Closed");
                break;
            case qlibssh2::Ssh2Client::Aborted:
                failedText += tr("Aborted");
                break;
            default:
                failedText += tr("Unkown");
                break;
        }
        q->_ui->testConnectLabel->setText(failedText);
    } else
        q->_ui->testConnectLabel->setText("Successfull");

    q->_ui->testConnectProgressBar->setMaximum(100);
    userInputAllowed();
}

// Private Functions
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
    q->_ui->RemoveButton->setEnabled(activate);
    q->_ui->testConnectButton->setEnabled(activate);
}

void HostSettingsPrivate::inputChanged(){
    Q_Q(widgets::HostSettings);
    if(areInputsFromEditsVaid())
        activateButtons(true);
    else
        activateButtons(false);
}

void HostSettingsPrivate::setupValidators(WKB::widgets::HostSettings *q) {
    QString validIPAddressRegex("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

    QString validHostnameRegex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");

    QRegularExpression validatorRegex(validIPAddressRegex + "|" + validHostnameRegex);

    QPointer<QValidator> validator = new QRegularExpressionValidator(validatorRegex);
    q->_ui->Hostname->setValidator(validator);

    // Set validator for server name
    QRegularExpression stringValidator("^[a-zA-Z0-9]+$");
    validator = new QRegularExpressionValidator(stringValidator);
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

void HostSettingsPrivate::readModelFromSettings() {
    Q_Q(widgets::HostSettings);
    int size = _settings.beginReadArray("Hosts");
    for (int i = 0; i < size; i++) {
        widgets::HostParameter param;
        _settings.setArrayIndex(i);
        param.serverName = _settings.value("serverName").toString();
        param.hostName = _settings.value("hostName").toString();
        param.password = _settings.value("password").toString();
        param.username = _settings.value("username").toString();
        param.port = _settings.value("port").toUInt();
        _hostParameters.append(param);
    }
    _settings.endArray();
    emit(modelModified());
}











#endif /* HostSettings_p_h */





