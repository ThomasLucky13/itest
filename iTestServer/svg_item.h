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

#ifndef SVG_ITEM_H
#define SVG_ITEM_H

#include <QListWidgetItem>
#include <QByteArray>

class SvgItem : public QListWidgetItem
{
public:
    SvgItem();
    SvgItem(const QString &);
    SvgItem(const QString &, const QString &);

public slots:
    bool setSvg(QString);
    QString svg();
    void setText(QString str) {
        QListWidgetItem::setText(str.remove("\n"));
    }
    bool isValid();

private:
    QString si_svg;
    bool si_valid;
};

class ImageItem : public QListWidgetItem
{
public:
    ImageItem();
    ImageItem(const QString &);
    ImageItem(const QString &, const QString &);

public slots:
    bool setImage(QString);
    bool setImage(QByteArray);
    QString image();
    void setText(QString str) {
        QListWidgetItem::setText(str.remove("\n"));
    }
    bool isValid();

private:
    QString ii_image;
    bool ii_valid;
};

#endif // SVG_ITEM_H
