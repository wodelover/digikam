/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2012-10-23
 * Description : a command line tool to show QtAV codecs available.
 *
 * Copyright (C) 2017-2019 by Gilles Caulier, <caulier dot gilles at gmail dot com>
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

#include <QDebug>
#include <QPainterPath>

// QtAv includes

#if defined(Q_CC_CLANG)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wundef"
#endif

#include <QtAV/VideoDecoder.h> // krazy:exclude=includes
#include <QtAV/VideoEncoder.h> // krazy:exclude=includes
#include <QtAV/AudioDecoder.h> // krazy:exclude=includes
#include <QtAV/AudioEncoder.h> // krazy:exclude=includes
#include <QtAV/AVMuxer.h>      // krazy:exclude=includes
#include <QtAV/LibAVFilter.h>  // krazy:exclude=includes

#if defined(Q_CC_CLANG)
#   pragma clang diagnostic pop
#endif

using namespace QtAV;

int main(int /*argc*/, char** /*argv*/)
{
    qDebug() << "--- Available Video Decoder Codecs ---";
    qDebug() << VideoDecoder::supportedCodecs();

    qDebug() << "--- Available Audio Decoder Codecs ---";
    qDebug() << AudioDecoder::supportedCodecs();

    qDebug() << "--- Available Video Encoder Codecs ---";
    qDebug() << VideoEncoder::supportedCodecs();

    qDebug() << "--- Available Audio Encoder Codecs ---";
    qDebug() << AudioEncoder::supportedCodecs();

    qDebug() << "--- Available Media Formats ---";
    qDebug() << AVMuxer::supportedFormats();

    qDebug() << "--- Available Media Extensions ---";
    qDebug() << AVMuxer::supportedExtensions();

    qDebug() << "--- Available Video Filters ---";
    qDebug() << LibAVFilter::videoFilters();

    qDebug() << "--- Available Audio Filters ---";
    qDebug() << LibAVFilter::audioFilters();

    return 0;
}
