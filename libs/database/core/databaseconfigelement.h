/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-06-27
 * Description : Database element configuration
 *
 * Copyright (C) 2009-2010 by Holger Foerster <hamsi2k at freenet dot de>
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

#ifndef _DATABASE_CONFIG_ELEMENT_H_
#define _DATABASE_CONFIG_ELEMENT_H_

// Qt includes

#include <QMap>

// Local includes

#include "digikam_export.h"
#include "databaseaction.h"

namespace Digikam
{

class DIGIKAM_EXPORT DatabaseConfigElement
{
public:

    static bool                   checkReadyForUse();
    static QString                errorMessage();
    static DatabaseConfigElement  element(const QString& databaseType);

public:

    QString                       databaseID;
    QString                       hostName;
    QString                       port;
    QString                       connectOptions;
    QString                       databaseName;
    QString                       userName;
    QString                       password;
    QString                       dbServerCmd;
    QString                       dbInitCmd;
    QMap<QString, DatabaseAction> sqlStatements;
};

} // namespace Digikam

#endif // _DATABASE_CONFIG_ELEMENT_H_
