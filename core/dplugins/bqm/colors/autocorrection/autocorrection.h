/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2008-11-28
 * Description : auto colors correction batch tool.
 *
 * Copyright (C) 2008-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_BQM_AUTO_CORRECTION_H
#define DIGIKAM_BQM_AUTO_CORRECTION_H

// Local includes

#include "batchtool.h"

class QComboBox;

using namespace Digikam;

namespace DigikamBqmAutoCorrectionPlugin
{

class AutoCorrection : public BatchTool
{
    Q_OBJECT

public:

    explicit AutoCorrection(QObject* const parent = nullptr);
    ~AutoCorrection();

    BatchToolSettings defaultSettings();

    BatchTool* clone(QObject* const parent=nullptr) const { return new AutoCorrection(parent); };

    void registerSettingsWidget();

private:

    bool toolOperations();

private Q_SLOTS:

    void slotAssignSettings2Widget();
    void slotSettingsChanged();

private:

    enum AutoCorrectionType
    {
        AutoLevelsCorrection = 0,
        NormalizeCorrection,
        EqualizeCorrection,
        StretchContrastCorrection,
        AutoExposureCorrection
    };

    QComboBox* m_comboBox;
};

} // namespace DigikamBqmAutoCorrectionPlugin

#endif // DIGIKAM_BQM_AUTO_CORRECTION_H
