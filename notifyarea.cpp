#include <QDesktopWidget>
#include <fstream>
#include "notifyarea.h"
#include "notifywidget.h"
#include "dbus.h"

NotifyArea::NotifyArea(char *configPath, bool debug)
{
debugMode = debug;
config = new char[255];
strcpy(config, configPath);

this->ReReadConfig();
messageWidget = new NotifyWidget("MessageWidget",&messageStack, this);
if(debugMode) fprintf(stderr,"MessageWidget created.\n");

notificationWidget = new NotifyWidget("NotificationWidget",&notificationStack, this);
if(debugMode) fprintf(stderr,"NotificationWidget created.\n");
index = (unsigned int) 1;
}

/////////////////////////////////////////////////////////////////////////////////
void NotifyArea::ReReadConfig()
{
maxOpacity = this->readConfigString("Opacity").toFloat();
if(maxOpacity<=0.1)
	{
	maxOpacity=0.7;
	}
widgetStyle = this->readConfigString("GeneralStyle");
if(widgetStyle=="")
	{
	widgetStyle = "margin: 0px; background: black; border: 3px solid white; color: lime; border-radius: 15px;";
	}
for(int i=1;i<4;i++) UrgencyTag[i] = this->readConfigString("UrgencyTag"+QString::number(i));

}
/////////////////////////////////////////////////////////////////////////////////

QString NotifyArea::readConfigString(QString param)
{
if(debugMode) fprintf(stderr,"Reading config string %s ...\n",param.toStdString().c_str());
QString result="";
std::string tmp;
std::ifstream file;
file.open(config);
if(!file.fail())
	{
	while(!file.eof())
		{
		std::getline( file, tmp, '\n' );
		if(tmp.find("#")==0) continue; //skip comments
		if(tmp.find(param.toStdString().c_str())!=std::string::npos)
			{
			result=tmp.substr(tmp.find("=")+1).c_str();
			}
		}
	}
if(debugMode) fprintf(stderr,"Returning config string: '%s' = '%s'.\n", param.toStdString().c_str(), result.toStdString().c_str());
return result;
}

void NotifyArea::NotificationClosed(unsigned id, unsigned reason)
{
if(debugMode) fprintf(stderr,"Closing Notification with id=%d...\n",(int)id);
for(std::vector<message>::iterator iter=messageStack.begin(); iter != messageStack.end(); iter++)
	{
	if(iter->id == id)
		{
		messageStack.erase(iter);
		messageWidget->timer->stop();
		messageWidget->checkIfNeedToShow();
		}
	}
for(std::vector<message>::iterator iter=notificationStack.begin(); iter != notificationStack.end(); iter++)
	{
	if(iter->id == id)
		{
		notificationStack.erase(iter);
		notificationWidget->timer->stop();
		notificationWidget->checkIfNeedToShow();
		}
	}
if(debugMode) fprintf(stderr,"Notification %d closed.\n",(int)id);
}

QPoint NotifyArea::getWidgetPosition(char *widgetName)
{
QPoint p;

if(debugMode) fprintf(stderr,"Calculating widget position...\n");

QDesktopWidget *desktop;
desktop = new QDesktopWidget();
QRect desktopGeometry = desktop->availableGeometry();

if(debugMode) fprintf(stderr,"Got desktop geometry.\n");

int position = this->readConfigString(QString(widgetName)+"Position").toInt();
if(debugMode) fprintf(stderr,"Got widget position from config.\n");

switch (position)
	{
	case 0:	p = desktopGeometry.bottomLeft();
	case 1:
		p = desktopGeometry.bottomRight();
		break;
	case 2:
		p = desktopGeometry.topRight();
		break;
	case 3:
		p = desktopGeometry.topLeft();
		break;
	default:
		p = desktopGeometry.bottomLeft();	
	}
if(debugMode) fprintf(stderr,"Returning widget coordinates.\n");
return p;
}