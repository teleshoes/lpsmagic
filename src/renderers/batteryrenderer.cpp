/*
    Copyright (C) 2012  Radu Andries <admiral0@tuxfamily.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "batteryrenderer.h"
#include "../renderermanager.h"
#include "../renderutil.h"
#include "qxtlogger.h"
#include <QPainter>
#include <QString>
#include <QDate>

using namespace MeeGo;

QImage BatteryRenderer::icon()
{
    return RenderUtil::systemIcon("battery-1.png");
}

QString BatteryRenderer::info()
{
    QString msg("Battery utility plugin");
    return msg;
}

QString BatteryRenderer::author()
{
    return QString();
}

int BatteryRenderer::getMaxSize (QString prefix)
{
    int lh=RenderUtil::contentLineHeight();
    return lh;
}

QString BatteryRenderer::brief (QString prefix)
{
    return QString();
}

/*
 * %batIcon  battery state icon (use resource system)
 * %batPercent bat remaining percent
 * %batCurrent current in mA {positive is discharging}
 * %batman lol
 */
QImage* BatteryRenderer::render (QString prefix)
{
    QStringList tokens=RenderUtil::parseToken(prefix);
    QString pref=tokens.at(0);
    qxtLog->info(QString("Will render in battery plugin prefix: %1").arg(pref));
    if(pref=="batIcon"){
      batIcon();
    }else if(pref=="batPercent"){
      batPercent(tokens.at(1));
    }else if(pref=="batCurrent"){
      batCurrent(tokens.at(1));
    }else{
      //batman
      batman();
    }
    return out;
}

void BatteryRenderer::init()
{
    battery=new QmBattery;
    lh=RenderUtil::lineHeight();
    battery->startCurrentMeasurement(MeeGo::QmBattery::RATE_5000ms);
}

void BatteryRenderer::batIcon()
{
    QmBattery::ChargerType charger=battery->getChargerType();
    int perc=battery->getRemainingCapacityPct();
    QmBattery::ChargingState state=battery->getChargingState();
    if(state==QmBattery::StateCharging){
      switch(charger){
        case QmBattery::Wall:
          out=RendererManager::instance()->renderPrefix("icon","{plug.png}");
          break;
        case QmBattery::Unknown:
          out=RendererManager::instance()->renderPrefix("icon","{battery-1.png}");
          break;
        default:
          out=RendererManager::instance()->renderPrefix("icon","{usb.png}");
      }
    }else{
      if(perc>80){
        out=RendererManager::instance()->renderPrefix("icon","{battery-4.png}");
      }else if(perc>60){
        out=RendererManager::instance()->renderPrefix("icon","{battery-3.png}");
      }else if(perc>25){
        out=RendererManager::instance()->renderPrefix("icon","{battery-2.png}");
      }else{
        out=RendererManager::instance()->renderPrefix("icon","{battery-1.png}");
      }
    }
}

void BatteryRenderer::batPercent(QString suffix)
{
  int percent=battery->getRemainingCapacityPct();
  QString cap("%1%2");
  cap=cap.arg(percent).arg(suffix);
  int iw=RenderUtil::expectedTextWidth(cap);
  out=RendererManager::instance()->renderPrefix("dumbtext",QString("{")+cap+"}");
}

void BatteryRenderer::batCurrent(QString suffix)
{
  int current=battery->getBatteryCurrent();
  QString cap("%1%2");
  cap=cap.arg(current).arg(suffix);
  int iw=RenderUtil::expectedTextWidth(cap);
  out=RendererManager::instance()->renderPrefix("dumbtext",QString("{")+cap+"}");
}

void BatteryRenderer::batman()
{
    int iw=lh*2.1;
    out=new QImage(iw,lh,QImage::Format_ARGB32);
    int sh=lh-2;
    int sw=iw-2;
    RenderUtil::fillImage(out);
    QPainter p(out);
    p.setPen(Qt::white);
    p.setBrush(RendererManager::foreground());
    
    p.setClipRect(1,1,sw,sh);
    QPainterPath path;
    //path.setFillRule(Qt::WindingFill);
    QPointF x(0.2*sw,0.96*sh);
    QPointF y(0.29*sw,0.05*sh);
    QPointF z;
    path.moveTo(x);
    x.setX(-0.02*sw);
    x.setY(0.57*sh);
    z.setX(-0.02*sw);
    z.setY(0.4*sh);
    path.cubicTo(x,z,y);
    path.cubicTo(0.28*sw,0.33*sh,0.23*sw,0.44*sh,0.42*sw,0.34*sh);
    
    p.drawPath(path); 
}
