/*
 * Copyright (C) 2011 Drogolov Roman Olegovich <drull@drull.org.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 *
 */




#include <QtCore>
#include <QPixmap>
#include <vector>
#include <string.h>

class NotifyWidget;
class QMyDBusAbstractAdaptor;
class QSystemTrayIcon; //alex

class Message
{
public:
	Message();
	Message(const Message &t);
	~Message();
	int timeout;
	int urgency;
	unsigned id;
	QPixmap *icon;
	QString app_name;
	QString text;
	QVariantMap hints;
	QStringList action;
	QString header;
	bool isComplete;
};

class NotifyArea : public QObject
{
Q_OBJECT
public:

NotifyArea(char*, bool);
QString readConfigString(QString);
QPoint getWidgetPosition(char *, QSize);
void ReReadConfig();

NotifyWidget *messageWidget;
NotifyWidget *notificationWidget;
QSystemTrayIcon *trayIcon; //alex
unsigned int index;
char *config;
bool debugMode;

int convertSpecialSymbols;
float maxOpacity;
int maxIconSize;
QString widgetStyle;
QString iconOverride;
QString systemIcon;
QString UrgencyTag[4];
QMyDBusAbstractAdaptor *adaptor;

public slots:
void CloseNotification(unsigned);

private: 
std::vector<Message*> messageStack;
std::vector<Message*> notificationStack;

signals:
void NotificationClosed(unsigned,unsigned);

};
