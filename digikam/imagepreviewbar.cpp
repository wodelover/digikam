/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-18-03
 * Description : image preview thumbs bar
 *
 * Copyright (C) 2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "imagepreviewbar.h"
#include "imagepreviewbar.moc"

// Qt includes.

#include <QList>
#include <QToolTip>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QCursor>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPolygon>
#include <QTextDocument>

// KDE includes.

#include <kdebug.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kmimetype.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kstandarddirs.h>

// Local includes.

#include "album.h"
#include "albumdb.h"
#include "albummanager.h"
#include "albumsettings.h"
#include "albumiconviewtooltip.h"
#include "ddragobjects.h"
#include "imageattributeswatch.h"
#include "metadatahub.h"
#include "ratingpopupmenu.h"
#include "dpopupmenu.h"
#include "themeengine.h"

namespace Digikam
{

class ImagePreviewBarPriv
{

public:

    ImagePreviewBarPriv()
    {
        // Pre-computed star polygon for a 15x15 pixmap.
        starPolygon << QPoint(0,  6);
        starPolygon << QPoint(5,  5);
        starPolygon << QPoint(7,  0);
        starPolygon << QPoint(9,  5);
        starPolygon << QPoint(14, 6);
        starPolygon << QPoint(10, 9);
        starPolygon << QPoint(11, 14);
        starPolygon << QPoint(7,  11);
        starPolygon << QPoint(3,  14);
        starPolygon << QPoint(4,  9);
    }

    QPolygon                starPolygon;

    QPixmap                 ratingPixmap;
};

ImagePreviewBar::ImagePreviewBar(QWidget* parent, int orientation, bool exifRotate)
               : ThumbBarView(parent, orientation, exifRotate), 
                 d(new ImagePreviewBarPriv)
{
    setMouseTracking(true);
    readToolTipSettings();
    setToolTip(new ImagePreviewBarToolTip(this));

    // -- Load rating Pixmap ------------------------------------------

    d->ratingPixmap = QPixmap(15, 15);
    d->ratingPixmap.fill(Qt::transparent);

    QPainter painter(&d->ratingPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(ThemeEngine::instance()->textSpecialRegColor());
    painter.setPen(ThemeEngine::instance()->textRegColor());
    painter.drawPolygon(d->starPolygon, Qt::WindingFill);
    painter.end();

    if (orientation == Qt::Vertical)
        setMinimumWidth(d->ratingPixmap.width()*5 + 6 + 2*getMargin());
    else
        setMinimumHeight(d->ratingPixmap.width()*5 + 6 + 2*getMargin());

    // ----------------------------------------------------------------

    ImageAttributesWatch *watch = ImageAttributesWatch::instance();

    connect(watch, SIGNAL(signalImageRatingChanged(qlonglong)),
            this, SLOT(slotImageRatingChanged(qlonglong)));

    connect(ThemeEngine::instance(), SIGNAL(signalThemeChanged()),
            this, SLOT(slotThemeChanged()));
}

ImagePreviewBar::~ImagePreviewBar()
{
    delete d;
}

QPixmap ImagePreviewBar::ratingPixmap() const
{
    return d->ratingPixmap;
}

void ImagePreviewBar::setSelectedItem(ImagePreviewBarItem* ltItem)
{
    ThumbBarItem *item = dynamic_cast<ThumbBarItem*>(ltItem);
    if (item) ThumbBarView::setSelected(item);
}

void ImagePreviewBar::slotImageRatingChanged(qlonglong imageId)
{
    for (ThumbBarItem *item = firstItem(); item; item = item->next())
    {
        ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);
        if (ltItem->info().id() == imageId)
        {
            triggerUpdate();
            return;
        }
    }
}

ImageInfo ImagePreviewBar::currentItemImageInfo() const
{
    if (currentItem())
    {
        ImagePreviewBarItem *item = dynamic_cast<ImagePreviewBarItem*>(currentItem());
        return item->info();
    }

    return ImageInfo();
}

ImageInfoList ImagePreviewBar::itemsImageInfoList()
{
    ImageInfoList list;

    for (ThumbBarItem *item = firstItem(); item; item = item->next())
    {
        ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);
        if (ltItem)
        {
            list << ltItem->info();
        }
    }

    return list;
}

ImagePreviewBarItem* ImagePreviewBar::findItemByInfo(const ImageInfo &info) const
{
    if (!info.isNull())
    {
        for (ThumbBarItem *item = firstItem(); item; item = item->next())
        {
            ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);
            if (ltItem)
            {
                if (ltItem->info() == info)
                    return ltItem;
            }
        }
    }
    return 0;
}

ImagePreviewBarItem* ImagePreviewBar::findItemByPos(const QPoint& pos) const
{
    ThumbBarItem *item = findItem(pos);
    if (item)
    {
        ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);
        return ltItem;
    }

    return 0;
}

void ImagePreviewBar::readToolTipSettings()
{
    AlbumSettings* albumSettings = AlbumSettings::instance();
    if (!albumSettings) return;

    Digikam::ThumbBarToolTipSettings settings;
    settings.showToolTips   = albumSettings->getShowToolTips();
    settings.showFileName   = albumSettings->getToolTipsShowFileName();
    settings.showFileDate   = albumSettings->getToolTipsShowFileDate();
    settings.showFileSize   = albumSettings->getToolTipsShowFileSize();
    settings.showImageType  = albumSettings->getToolTipsShowImageType();
    settings.showImageDim   = albumSettings->getToolTipsShowImageDim();
    settings.showPhotoMake  = albumSettings->getToolTipsShowPhotoMake();
    settings.showPhotoDate  = albumSettings->getToolTipsShowPhotoDate();
    settings.showPhotoFocal = albumSettings->getToolTipsShowPhotoFocal();
    settings.showPhotoExpo  = albumSettings->getToolTipsShowPhotoExpo();
    settings.showPhotoMode  = albumSettings->getToolTipsShowPhotoMode();
    settings.showPhotoFlash = albumSettings->getToolTipsShowPhotoFlash();
    settings.showPhotoWB    = albumSettings->getToolTipsShowPhotoWB();
    setToolTipSettings(settings);
}

void ImagePreviewBar::viewportPaintEvent(QPaintEvent* e)
{
    ThemeEngine* te = ThemeEngine::instance();
    QRect    er(e->rect());
    QPixmap  bgPix;

    if (countItems() > 0)
    {
        int cy=0, cx=0, ts=0, y1=0, y2=0, x1=0, x2=0;
        QPixmap  tile;

        if (getOrientation() == Qt::Vertical)
        {
            cy = viewportToContents(er.topLeft()).y();

            bgPix = QPixmap(contentsRect().width(), er.height());

            ts   = getTileSize() + 2*getMargin();
            tile = QPixmap(visibleWidth(), ts);

            y1 = (cy/ts)*ts;
            y2 = ((y1 + er.height())/ts +1)*ts;
        }
        else
        {
            cx = viewportToContents(er.topLeft()).x();

            bgPix = QPixmap(er.width(), contentsRect().height());

            ts   = getTileSize() + 2*getMargin();
            tile = QPixmap(ts, visibleHeight());

            x1 = (cx/ts)*ts;
            x2 = ((x1 + er.width())/ts +1)*ts;
        }

        bgPix.fill(te->baseColor());

        for (ThumbBarItem *item = firstItem(); item; item = item->next())
        {
            if (getOrientation() == Qt::Vertical)
            {
                if (y1 <= item->position() && item->position() <= y2)
                {
                    if (item == currentItem())
                        tile = te->thumbSelPixmap(tile.width(), tile.height());
                    else
                        tile = te->thumbRegPixmap(tile.width(), tile.height());

                    QPainter p(&tile);

                    if (item == currentItem())
                    {
                        p.setPen(QPen(te->textSelColor(), 3));
                        p.drawRect(2, 2, tile.width()-2, tile.height()-2);
                    }
                    else
                    {
                        p.setPen(QPen(te->textRegColor(), 1));
                        p.drawRect(0, 0, tile.width(), tile.height());
                    }

                    QPixmap pix;
                    if (pixmapForItem(item, pix))
                    {
                        int x = (tile.width()  - pix.width())/2;
                        int y = (tile.height() - pix.height())/2;

                        p.drawPixmap(x, y, pix);

                        ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);

                        QRect r(0, tile.height()-getMargin()-d->ratingPixmap.height(),
                                tile.width(), d->ratingPixmap.height());
                        int rating = ltItem->info().rating();
                        int xr     = (r.width() - rating * d->ratingPixmap.width())/2;
                        int wr     = rating * d->ratingPixmap.width();
                        p.drawTiledPixmap(xr, r.y(), wr, r.height(), d->ratingPixmap);
                    }

                    p.end();

                    QPainter p2(&bgPix);
                    p2.drawPixmap(0, item->position() - cy, tile);
                    p2.end();
                }
            }
            else
            {
                if (x1 <= item->position() && item->position() <= x2)
                {
                    if (item == currentItem())
                        tile = te->thumbSelPixmap(tile.width(), tile.height());
                    else
                        tile = te->thumbRegPixmap(tile.width(), tile.height());

                    QPainter p(&tile);

                    if (item == currentItem())
                    {
                        p.setPen(QPen(te->textSelColor(), 2));
                        p.drawRect(1, 1, tile.width()-1, tile.height()-1);
                    }
                    else
                    {
                        p.setPen(QPen(te->textRegColor(), 1));
                        p.drawRect(0, 0, tile.width(), tile.height());
                    }

                    QPixmap pix;
                    if (pixmapForItem(item, pix))
                    {
                        int x = (tile.width() - pix.width())/2;
                        int y = (tile.height()- pix.height())/2;
                        p.drawPixmap(x, y, pix);

                        ImagePreviewBarItem *ltItem = dynamic_cast<ImagePreviewBarItem*>(item);

                        QRect r(0, tile.height()-getMargin()-d->ratingPixmap.height(),
                                tile.width(), d->ratingPixmap.height());
                        int rating = ltItem->info().rating();
                        int xr     = (r.width() - rating * d->ratingPixmap.width())/2;
                        int wr     = rating * d->ratingPixmap.width();
                        p.drawTiledPixmap(xr, r.y(), wr, r.height(), d->ratingPixmap);
                    }

                    p.end();

                    QPainter p2(&bgPix);
                    p2.drawPixmap(item->position() - cx, 0, tile);
                    p2.end();
                }
            }
        }

        QPainter p3(viewport());

        if (getOrientation() == Qt::Vertical)
            p3.drawPixmap(0, er.y(), bgPix);
        else
            p3.drawPixmap(er.x(), 0, bgPix);

        p3.end();
    }
    else
    {
        bgPix = QPixmap(contentsRect().width(), contentsRect().height());
        bgPix.fill(te->baseColor());

        QPainter p5(viewport());
        p5.drawPixmap(0, 0, bgPix);
        p5.end();
    }

    checkPreload();
}

void ImagePreviewBar::slotThemeChanged()
{
    QPainter painter(&d->ratingPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(ThemeEngine::instance()->textSpecialRegColor());
    painter.setPen(ThemeEngine::instance()->textRegColor());
    painter.drawPolygon(d->starPolygon, Qt::WindingFill);
    painter.end();
    slotUpdate();
}

// -------------------------------------------------------------------------

ImagePreviewBarItem::ImagePreviewBarItem(ImagePreviewBar *view, const ImageInfo &info)
                   : ThumbBarItem(view, info.fileUrl())
{
    m_info = info;
}

ImagePreviewBarItem::~ImagePreviewBarItem()
{
}

ImageInfo ImagePreviewBarItem::info()
{
    return m_info;
}

// -------------------------------------------------------------------------

ImagePreviewBarToolTip::ImagePreviewBarToolTip(ThumbBarView* parent)
                      : ThumbBarToolTip(parent)
{
}

ImagePreviewBarToolTip::~ImagePreviewBarToolTip()
{
}

QString ImagePreviewBarToolTip::tipContents()
{
    if (!item()) return QString();
    ImageInfo info = dynamic_cast<ImagePreviewBarItem *>(item())->info();
    return AlbumIconViewToolTip::fillTipContents(info);
}

}  // namespace Digikam
