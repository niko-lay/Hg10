/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/system/CardDoneMessage>
#include <bb/platform/Notification>
#include <bb/device/DisplayInfo>
#include <QTimer>

#include "XMPPService.hpp"
#include "LoginController.hpp"
#include "ListContactsController.hpp"
#include "Image/NetImageTracker.h"
#include "ConversationController.hpp"
#include "SettingsController.hpp"
#include "GoogleConnectController.hpp"
#include "DropBoxConnectController.hpp"
#include "RoomController.hpp"
#include "SuggestContactController.hpp"
#include "SmileyPickerController.hpp"
#include "ApplicationLogController.hpp"
#include "DriveController.hpp"
#include "CustomizationController.hpp"
#include "ThemeEditorController.hpp"
#include "BugReportController.hpp"
#include "HistoryBrowserController.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(),
        m_pLocaleHandler(NULL),
        m_app(app),
        m_root(NULL),
        m_qml(NULL),
        m_Settings("Amonchakai", "Hg10Service"),
        m_InvokeManager(new bb::system::InvokeManager(this)),
        m_HeadlessStart(false),
        m_isCard(false)
{



    bool connectResult;

    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning.
    Q_UNUSED(connectResult);
    connectResult = connect(m_InvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    qDebug() << "HubIntegration: started and connected to invokeManager";


    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

    switch(m_InvokeManager->startupMode()) {
        case bb::system::ApplicationStartupMode::LaunchApplication:
            qDebug() << "HeadlessHubIntegration: Regular application launch";
            break;
        case bb::system::ApplicationStartupMode::InvokeApplication:
            m_HeadlessStart = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching app from invoke";
            break;
        case bb::system::ApplicationStartupMode::InvokeCard:
            m_HeadlessStart = true;
            m_isCard = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching card from invoke";
            break;
        // enable when 10.3 beta is released
        //case ApplicationStartupMode::InvokeHeadless:
        case 4:
            qDebug() << "--------------------- HeadlessHubIntegration: Launching headless from invoke";
            m_HeadlessStart = true;
            break;
        default:
            qDebug() << "--------------------- HeadlessHubIntegration: other launch: " << m_InvokeManager->startupMode();
            break;
       }

    // ---------------------------------------------------------------------
    // Regular integration

    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);




    XMPP *client = XMPP::get();
    //client->logger()->setLoggingType(QXmppLogger::StdoutLogging);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();


    qmlRegisterType<LoginController>("Network.LoginController", 1, 0, "LoginController");
    qmlRegisterType<ListContactsController>("Network.ListContactsController", 1, 0, "ListContactsController");
    qmlRegisterType<NetImageTracker>("com.netimage", 1, 0, "NetImageTracker");
    qmlRegisterType<ConversationController>("Network.ConversationController", 1, 0, "ConversationController");
    qmlRegisterType<SettingsController>("conf.SettingsController", 1, 0, "SettingsController");
    qmlRegisterType<GoogleConnectController>("Network.GoogleConnectController", 1, 0, "GoogleConnectController");
    qmlRegisterType<DropBoxConnectController>("Network.DropBoxConnectController", 1, 0, "DropBoxConnectController");
    qmlRegisterType<RoomController>("Network.RoomController", 1, 0, "RoomController");
    qmlRegisterType<SuggestContactController>("Network.SuggestContactController", 1, 0, "SuggestContactController");
    qmlRegisterType<SmileyPickerController>("Network.SmileyPickerController", 1, 0, "SmileyPickerController");
    qmlRegisterType<QTimer>("Lib.QTimer", 1, 0, "QTimer");
    qmlRegisterType<ApplicationLogController>("Lib.ApplicationLogController", 1, 0, "ApplicationLogController");
    qmlRegisterType<DriveController>("Network.DriveController", 1, 0, "DriveController");
    qmlRegisterType<CustomizationController>("Network.CustomizationController", 1, 0, "CustomizationController");
    qmlRegisterType<ThemeEditorController>("Network.ThemeEditorController", 1, 0, "ThemeEditorController");
    qmlRegisterType<HistoryBrowserController>("Network.HistoryBrowserController", 1, 0, "HistoryBrowserController");
    qmlRegisterType<BugReportController>("Lib.BugReport", 1, 0, "BugReport");

    if(!m_HeadlessStart) {

        // Create scene document from main.qml asset, the parent is set
        // to ensure the document gets destroyed properly at shut down.
        QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

        // Create root object for the UI
        AbstractPane *root = qml->createRootObject<AbstractPane>();

        // Set created root object as the application scene
        Application::instance()->setScene(root);

        bb::device::DisplayInfo display;
        QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
        displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
        displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
        qml->setContextProperty( "DisplayInfo", displayDimensions );

        // ------------------------------------------------------------
        // disconnect the UI from the headless, when the app is minimized.
        QTimer::singleShot(1000, this, SLOT(initReconnect()));

    }
}

void ApplicationUI::initReconnect() {
    connect(m_app, SIGNAL(thumbnail()),  XMPP::get(), SLOT(disconnectToXMPPService()));
    connect(m_app, SIGNAL(fullscreen()), XMPP::get(), SLOT(connectToXMPPService()));
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("Hg10_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}



// -----------------------------------------------------------------------------------------
// receive card request


void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request) {
//    qDebug() << "invoke!" << request.action();

    if(request.action().compare("bb.action.VIEW") == 0 || request.action().compare("bb.action.OPEN") == 0) {
//         qDebug() << "HubIntegration: onInvoked: view item: " << request.data();

         JsonDataAccess jda;

         QVariantMap objectMap = (jda.loadFromBuffer(request.data())).toMap();
         QVariantMap itemMap = objectMap["attributes"].toMap();


         QVariantList items = m_Settings.value("hub/items").toList();

         QString addresse;
         QString name;
         QVariantMap item;
         for(int index = 0; index < items.size(); index++) {
             item = items.at(index).toMap();
             QString sourceId = item["messageid"].toString();

              if (item["sourceId"].toString() == itemMap["messageid"].toString() ||
                  item["sourceId"].toString() == itemMap["sourceId"].toString()) {

                  addresse = item["userData"].toString();

                  break;
              }
         }

         QmlDocument *qml = NULL;

         QSettings settings("Amonchakai", "Hg10");
         if(settings.value("ConversationTheme", 0) == 0)
             qml = QmlDocument::create("asset:///ConversationCardBBM.qml")
                                                          .parent(this);
         else
             qml = QmlDocument::create("asset:///ConversationCard.qml")
                                                           .parent(this);

         m_root = qml->createRootObject<NavigationPane>();

         m_app->setScene(m_root);


         QObject *thread = m_root->findChild<QObject*>("conversationCard");
         if(thread != NULL) {
             qml->setContextProperty("_app", this);

             bb::device::DisplayInfo display;
             QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
             displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
             displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
             qml->setContextProperty( "DisplayInfo", displayDimensions );

             thread->setProperty("name", item["name"].toString());

             if(QFile::exists(QDir::homePath() + QLatin1String("/vCards/") + addresse + ".png"))
                 thread->setProperty("avatar", QDir::homePath() + QLatin1String("/vCards/") + addresse + ".png");
             else
                 thread->setProperty("avatar", "asset:///images/avatar.png");

             thread->setProperty("room", false);
             thread->setProperty("id", addresse);


         } else
             qDebug() << "pageThread variable is not found in the qml document :(";


         InvokeRequest request;
         request.setTarget("com.amonchakai.Hg10Service");
         request.setAction("bb.action.MARKREAD");
         request.setMimeType("hub/item");
         request.setUri(QUrl("pim:"));

         QByteArray bytes;
         jda.saveToBuffer(objectMap, &bytes);
         request.setData(bytes);

         bb::platform::Notification *notif = new bb::platform::Notification();
         notif->clearEffectsForAll();
         notif->deleteLater();

         m_InvokeManager->invoke(request);

         bool check = connect(XMPP::get(), SIGNAL(closeCardRequest()), this, SLOT(closeCard()));
         Q_ASSERT(check);
         Q_UNUSED(check);

    }

    if(request.action().compare("bb.action.COMPOSE") == 0) {

    }


}

void ApplicationUI::closeCard() {
    m_app->requestExit();

    if (m_isCard) {
        // Assemble response message
        CardDoneMessage message;
        message.setData(tr(""));
        message.setDataType("text/plain");
        message.setReason(tr("Success!"));

        // Send message
        m_InvokeManager->sendCardDone(message);
    }
}



