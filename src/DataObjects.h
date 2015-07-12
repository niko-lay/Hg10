/*
 * DataObjects.h
 *
 *  Created on: 16 mars 2014
 *      Author: PierreL
 */

#ifndef DATAOBJECTS_H_
#define DATAOBJECTS_H_

#include <QtCore/QObject>
#include <QString>


struct XMPPServiceMessages {
    enum Message {
        LOGIN                       = 1,
        REPLY_LOGGED_IN,
        REPLY_CONNECTION_FAILED,
        REQUEST_CONNECTION_STATUS,
        DISCONNECT,
        REQUEST_CONTACT_LIST,
        REPLY_CONTACT_LIST,
        STATUS_UPDATE,
        PRESENCE_UPDATE,
        MESSAGE                     = 10,
        SEND_MESSAGE,
        SEND_FILE,
        REFRESH_SETTINGS,
        CREATE_ROOM,
        ADD_PARTICIPANT,
        ADVANCED_LOGIN,
        OAUTH2_LOGIN,
        REQUEST_CONTACT_LIST_PRESENCE,
        SET_STATUS_TEXT,
        SET_PRESENCE                = 20,
        ADD_CONTACT,
        OTR_REQUEST_START,
        OTR_REQUEST_STOP,
        OTR_SETUP_KEYS,
        OTR_GET_OWN_FINGERPRINT,
        OTR_OWN_FINGERPRINT,
        OTR_FINGERPRINT_RECEIVED,
        OTR_GONE_SECURE,
        OTR_GONE_UNSECURE,
        OTR_WAS_NOT_SECURE,
        OTR_REQUEST_STATUS,
        OTR_SMP_QUESTION,
        OTR_SMP_REPLY,
        UPDATE_HUB,
        REMOVE_HUB
    };

};


class DriveItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString id          READ getID          WRITE setID          NOTIFY idChanged)
    Q_PROPERTY( QString iconLink    READ getIconLink    WRITE setIconLink    NOTIFY iconLinkChanged)
    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle       NOTIFY titleChanged)
    Q_PROPERTY( QString type        READ getType        WRITE setType        NOTIFY typeChanged)
    Q_PROPERTY( qint64  timestamp   READ getTimestamp   WRITE setTimestamp   NOTIFY timestampChanged)
    Q_PROPERTY( QString openLink    READ getOpenLink    WRITE setOpenLink    NOTIFY openLinkChanged)
    Q_PROPERTY( QString downloadLink READ getDownloadLink WRITE setDownloadLink    NOTIFY downloadLinkChanged)

private:
    QString         m_Id;
    QString         m_IconLink;
    QString         m_Title;
    QString         m_Type;
    qint64          m_Timestamp;
    QString         m_OpenLink;
    QString         m_DownloadLink;
    QString         m_MD5;

public:
    DriveItem(QObject *parent)  : QObject(parent), m_Timestamp(0) {};

    inline const QString &getID() const                     { return m_Id; }
    inline void           setID(const QString &c)           { m_Id = c; emit idChanged();}

    inline const QString &getIconLink() const               { return m_IconLink; }
    inline void           setIconLink(const QString &c)     { m_IconLink = c; emit iconLinkChanged();}

    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &c)        { m_Title = c; emit titleChanged();}

    inline const QString &getType() const                   { return m_Type; }
    inline void           setType(const QString &c)         { m_Type = c; emit typeChanged();}

    inline qint64         getTimestamp() const              { return m_Timestamp; }
    inline void           setTimestamp(qint64 c)            { m_Timestamp = c; emit timestampChanged();}

    inline const QString &getOpenLink() const               { return m_OpenLink; }
    inline void           setOpenLink(const QString &c)     { m_OpenLink = c; emit openLinkChanged();}

    inline const QString &getDownloadLink() const           { return m_DownloadLink; }
    inline void           setDownloadLink(const QString &c) { m_DownloadLink = c; emit downloadLinkChanged();}

    inline const QString &getHash() const                   { return m_MD5; }
    inline void           setHash(const QString &c)         { m_MD5 = c; }

    // ----------------------------------------------------------------------------------------------
Q_SIGNALS:

    void idChanged();
    void iconLinkChanged();
    void titleChanged();
    void typeChanged();
    void timestampChanged();
    void openLinkChanged();
    void downloadLinkChanged();

};


class Contact : public QObject {
	Q_OBJECT

	Q_PROPERTY( QString name 		READ getName      	WRITE setName        NOTIFY nameChanged)
	Q_PROPERTY( QString id        	READ getID   	    WRITE setID          NOTIFY idChanged)
	Q_PROPERTY( qint64  timestamp 	READ getTimestamp 	WRITE setTimestamp   NOTIFY timestampChanged)
	Q_PROPERTY( QString timestampString   READ getTimestampString   WRITE setTimestampString   NOTIFY timestampStringChanged)
	Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)
	Q_PROPERTY( QString preview     READ getPreview     WRITE setPreview     NOTIFY previewChanged)
	Q_PROPERTY( int     presence    READ getPresence    WRITE setPresence    NOTIFY presenceChanged)
	Q_PROPERTY( int     read        READ getRead        WRITE setRead        NOTIFY readChanged)


	// ----------------------------------------------------------------------------------------------


private:
	QString m_Name;
	QString m_ID;
	qint64  m_Timestamp;
	QString m_TimestampString;
	QString m_Avatar;
	QString m_Preview;
	int     m_Presence;
	int     m_Read;

	// ----------------------------------------------------------------------------------------------

public:
	Contact(QObject *parent = 0) : QObject(parent) {}
	virtual ~Contact() {}


	inline const QString &getName() const					{ return m_Name; }
	inline void			  setName(const QString &s)		    { m_Name = s; emit nameChanged(); }

	inline const QString &getID() const				        { return m_ID; }
	inline void			  setID(const QString &c)		    { m_ID = c; emit idChanged();}


	inline const qint64  &getTimestamp() const				{ return m_Timestamp; }
	inline void			  setTimestamp(qint64 c)	        { m_Timestamp = c; emit timestampChanged(); }

	inline const QString  &getTimestampString() const             { return m_TimestampString; }
	inline void            setTimestampString(const QString &c)   { m_TimestampString = c; emit timestampStringChanged(); }

	inline const QString &getAvatar() const                 { return m_Avatar; }
	inline void           setAvatar(const QString &c)       { m_Avatar = c; emit avatarChanged(); }

	inline const QString &getPreview() const                 { return m_Preview; }
	inline void           setPreview(const QString &c)       { m_Preview = c; emit previewChanged(); }

	inline int            getPresence() const                { return m_Presence; }
	inline void           setPresence(int c)                 { m_Presence = c; emit presenceChanged(); }

	inline int            getRead() const                    { return m_Read; }
    inline void           setRead(int c)                     { m_Read = c; emit readChanged(); }

	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void nameChanged();
		void idChanged();
		void timestampChanged();
		void timestampStringChanged();
		void avatarChanged();
		void previewChanged();
		void presenceChanged();
		void readChanged();

};

// ---------------------------------------------------------------------------------------------
// Stickers

class Sticker : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString localUrl        READ getLocalUrl      WRITE setLocalUrl        NOTIFY localUrlChanged)
    Q_PROPERTY( QString distUrl         READ getDistUrl       WRITE setDistUrl         NOTIFY distUrlChanged)

private:
    QString m_LocalUrl;
    QString m_DistUrl;


public:
    Sticker(QObject *parent = 0) : QObject(parent) {}
    virtual ~Sticker() {}

    inline const QString &getLocalUrl() const                   { return m_LocalUrl; }
    inline void           setLocalUrl(const QString &s)         { m_LocalUrl = s; emit localUrlChanged(); }

    inline const QString &getDistUrl() const                     { return m_DistUrl; }
    inline void           setDistUrl(const QString &c)           { m_DistUrl = c; emit distUrlChanged();}


    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void localUrlChanged();
        void distUrlChanged();

};

// ---------------------------------------------------------------------------------------------
// Wallpapers

class WallpaperItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString userName       READ getUserName      WRITE setUserName       NOTIFY userNameChanged)
    Q_PROPERTY( QString userId         READ getUserId        WRITE setUserId         NOTIFY userIdChanged)
    Q_PROPERTY( QString wallpaper      READ getWallpaper     WRITE setWallpaper      NOTIFY wallpaperChanged)

private:
    QString m_UserName;
    QString m_UserId;
    QString m_Wallpaper;


public:
    WallpaperItem(QObject *parent = 0) : QObject(parent) {}
    virtual ~WallpaperItem() {}

    inline const QString &getUserName() const                   { return m_UserName; }
    inline void           setUserName(const QString &s)         { m_UserName = s; emit userNameChanged(); }

    inline const QString &getUserId() const                     { return m_UserId; }
    inline void           setUserId(const QString &c)           { m_UserId = c; emit userIdChanged();}

    inline const QString &getWallpaper() const                  { return m_Wallpaper; }
    inline void           setWallpaper(const QString &c)        { m_Wallpaper = c; emit wallpaperChanged();}

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void userNameChanged();
        void userIdChanged();
        void wallpaperChanged();

};



// ----------------------------------------------------------------------------------------------
// Objects to handle group chat

class RenderRoom : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString name        READ getName        WRITE setName        NOTIFY nameChanged)
    Q_PROPERTY( QString id          READ getID          WRITE setID          NOTIFY idChanged)
    Q_PROPERTY( QString avatar      READ getAvatar      WRITE setAvatar      NOTIFY avatarChanged)

private:
    QString m_Name;
    QString m_ID;
    QString m_Avatar;

    // ----------------------------------------------------------------------------------------------

public:
    RenderRoom(QObject *parent = 0) : QObject(parent) {}
    virtual ~RenderRoom() {}


    inline const QString &getName() const                   { return m_Name; }
    inline void           setName(const QString &s)         { m_Name = s; emit nameChanged(); }

    inline const QString &getID() const                     { return m_ID; }
    inline void           setID(const QString &c)           { m_ID = c; emit idChanged();}

    inline const QString &getAvatar() const                 { return m_Avatar; }
    inline void           setAvatar(const QString &c)       { m_Avatar = c; emit avatarChanged(); }

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void nameChanged();
        void idChanged();
        void avatarChanged();
};


struct GroupChat {
    QString         m_RoomID;
    QList<QString>  m_Participants;
};

inline QDataStream &operator<<(QDataStream& stream, const GroupChat& room) {
    stream << room.m_RoomID;
    stream << room.m_Participants.size();

    for(int i = 0 ; i < room.m_Participants.size() ; ++i)
        stream << room.m_Participants.at(i);

    return stream;
}

inline QDataStream &operator>>(QDataStream& stream, GroupChat& room) {
    stream >> room.m_RoomID;
    int nbParticipants = 0;
    stream >> nbParticipants;

    for(int i = 0 ; i < nbParticipants ; ++i) {
        QString participantId;
        stream >> participantId;
        room.m_Participants.push_back(participantId);
    }

    return stream;
}

// ----------------------------------------------------------------------------------------------
// Objects to handle message history

struct TimeEvent {
    QString     m_Who;
    QString     m_What;
    qint64      m_When;
    int         m_Read;
    QString     m_MessageID;
};

inline QDataStream &operator<<(QDataStream& stream, const TimeEvent& event) {
    stream << event.m_Who;
    stream << event.m_What;
    stream << event.m_When;
    stream << event.m_Read;
    stream << event.m_MessageID;

    return stream;
}

inline QDataStream &operator>>(QDataStream& stream, TimeEvent& event) {
    stream >> event.m_Who;
    stream >> event.m_What;
    stream >> event.m_When;
    stream >> event.m_Read;
    stream >> event.m_MessageID;

    return stream;
}



struct History {
    QList<TimeEvent>    m_History;
};

inline QDataStream &operator<<(QDataStream& stream, const History& history) {
    for(int i = 0 ; i < history.m_History.size() ; ++i) {
        stream << history.m_History.at(i);
    }

    return stream;
}


inline QDataStream &operator>>(QDataStream& stream, History& history) {

    TimeEvent e;
    do {
        stream >> e;
        if(!e.m_Who.isEmpty())
            history.m_History.push_back(e);
    } while(!e.m_Who.isEmpty());

    return stream;
}


// ----------------------------------------------------------------------------------------------
// Objects to handle audio message history

struct AudioMessage {
    QString     m_LocalUrl;
    QString     m_DistUrl;
};

inline QDataStream &operator<<(QDataStream& stream, const AudioMessage& event) {
    stream << event.m_LocalUrl;
    stream << event.m_DistUrl;

    return stream;
}

inline QDataStream &operator>>(QDataStream& stream, AudioMessage& event) {
    stream >> event.m_LocalUrl;
    stream >> event.m_DistUrl;

    return stream;
}





// ---------------------------------------------------------------------------------------------
// ActionComposer

class ActionComposerItem : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString image           READ getImage      WRITE setImage        NOTIFY imageChanged)
    Q_PROPERTY( QString category        READ getCategory   WRITE setCategory     NOTIFY categoryChanged)
    Q_PROPERTY( QString caption         READ getCaption    WRITE setCaption      NOTIFY captionChanged)
    Q_PROPERTY( int action              READ getAction     WRITE setAction       NOTIFY actionChanged)

private:
    QString m_Image;
    QString m_Category;
    QString m_Caption;
    int m_Action;


public:
    ActionComposerItem(QObject *parent = 0) : QObject(parent), m_Action(0) {}
    virtual ~ActionComposerItem() {}

    inline const QString &getImage() const                      { return m_Image; }
    inline void           setImage(const QString &s)            { m_Image = s; emit imageChanged(); }

    inline const QString &getCategory() const                   { return m_Category; }
    inline void           setCategory(const QString &s)         { m_Category = s; emit categoryChanged(); }

    inline const QString &getCaption() const                    { return m_Caption; }
    inline void           setCaption(const QString &c)          { m_Caption = c; emit captionChanged();}

    inline int            getAction() const                     { return m_Action; }
    inline void           setAction(int c)                      { m_Action = c; emit actionChanged();}

    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void imageChanged();
        void categoryChanged();
        void captionChanged();
        void actionChanged();

};




#endif /* DATAOBJECTS_H_ */
