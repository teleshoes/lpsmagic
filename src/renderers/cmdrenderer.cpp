/*
    cmdrenderer: Copyright (C) 2012  Elliot Wolk <elliot.wolk@gmail.com>
    lpsmagic: Copyright (C) 2012  Radu Andries <admiral0@tuxfamily.org>

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


#include "cmdrenderer.h"
#include "../renderutil.h"
#include "../renderermanager.h"
#include "qxtlogger.h"
#include <QImage>
#include <QPainter>
#include <QDate>
#include <QColormap>

#include <string>
#include <iostream>
#include <stdio.h>

std::string CmdRenderer::exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

QImage CmdRenderer::icon()
{
    return Renderer::icon();
}

QString CmdRenderer::info()
{
    return brief("");
}

QString CmdRenderer::author()
{
    return QString("Elliot Wolk <elliot.wolk@gmail.com>");
}

int CmdRenderer::getMaxSize (QString prefix)
{
    return 0;
}

QString CmdRenderer::brief (QString prefix)
{
    return QString("Run commands and display the first line of STDOUT");
}

QImage* CmdRenderer::render (QString prefix)
{
    init();
    QString cmd = RenderUtil::parseToken(prefix).at(1);
    qxtLog->info(QString("Running cmd: %1").arg(cmd));
    std::string out = exec(cmd.toUtf8().data());
    int linePos = out.find("\n");
    if(linePos > 0)
    {
        out = out.substr(0, linePos);
    }
    QString text = QString::fromStdString(out);
    qxtLog->info(QString("cmd first line stdout: %1").arg(text));
    QColormap cmap=QColormap::instance(0);
    uint pix;
    
    QImage *i=new QImage(RenderUtil::expectedTextWidth(text),lineheight,QImage::Format_ARGB32);
    pix=cmap.pixel(RendererManager::background());
    i->fill(pix);
    QPainter p(i);
    p.setBrush(Qt::NoBrush);
    p.setPen(RendererManager::foreground());
    p.setFont(RendererManager::defaultFont());
    p.drawText(0,RenderUtil::baseline(),text);
    return i;
}

void CmdRenderer::init()
{
    lineheight=RenderUtil::contentLineHeight();
}

