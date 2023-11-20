/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2016 Michal Tomlein

 iTest is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 iTest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with iTest; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#include "svg_item.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QBuffer>

SvgItem::SvgItem():
QListWidgetItem(0, QListWidgetItem::UserType)
{ si_valid = false; }

SvgItem::SvgItem(const QString &text):
QListWidgetItem(0, QListWidgetItem::UserType)
{ si_valid = false; setText(text); }

SvgItem::SvgItem(const QString &text, const QString &svg):
QListWidgetItem(0, QListWidgetItem::UserType)
{ si_valid = false; setText(text); setSvg(svg); }

bool SvgItem::setSvg(QString svg)
{
    svg.remove("\n");
    QSvgRenderer svgrenderer(svg.toUtf8());
    if (!svgrenderer.isValid()) {
        return false;
    } else {
        si_valid = true;
    }
    QPixmap pixmap(64, 64);
    QPainter painter(&pixmap);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(Qt::white));
    painter.drawRect(QRect(0, 0, 64, 64));
    painter.restore();
    svgrenderer.render(&painter);
    setIcon(QIcon(pixmap));
    si_svg = svg;
    return true;
}

QString SvgItem::svg() { return si_svg; }

bool SvgItem::isValid() { return si_valid; }



ImageItem::ImageItem():
QListWidgetItem(0, QListWidgetItem::UserType)
{ ii_valid = false; }

ImageItem::ImageItem(const QString &text):
QListWidgetItem(0, QListWidgetItem::UserType)
{ ii_valid = false; setText(text); }

ImageItem::ImageItem(const QString &text, const QString &image):
QListWidgetItem(0, QListWidgetItem::UserType)
{ ii_valid = false; setText(text); setImage(image); }

bool ImageItem::setImage(QString image)
{
    QByteArray rawImage;
    rawImage.append(image);
    QPixmap pixmap;
    if (!pixmap.loadFromData(rawImage.toBase64(), "PNG"))
        return false;
    else ii_valid = true;
    setIcon(QIcon(pixmap));

    ii_image = image;
    return true;
}

bool ImageItem::setImage(QByteArray rawImage)
{
    QPixmap pixmap;
    if (!pixmap.loadFromData(rawImage.data(), "PNG"))
        return false;
    else ii_valid = true;
    setIcon(QIcon(pixmap));

    //ii_image = image;
    return true;
}

QString ImageItem::image() { return ii_image; }

bool ImageItem::isValid() { return ii_valid; }
