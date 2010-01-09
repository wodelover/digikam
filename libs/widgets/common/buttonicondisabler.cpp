/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-01-09
 * Description : A helper class to disable the the icon of a togglebutton, if it is not checked.
 *               This will simulate a disabled button, that is still clickable.
 *
 * Copyright (C) 2006-2010 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2009-2010 by Andi Clemens <andi dot clemens at gmx dot net>
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

#include "buttonicondisabler.moc"

// Qt includes

#include <QAbstractButton>

namespace Digikam
{

ButtonIconDisabler::ButtonIconDisabler(QAbstractButton* button)
                  : QObject(button), m_button(button)
{
    m_icon = m_button->icon();

    int minSize = qMin(m_button->size().width(), m_button->size().height());
    QSize size(minSize, minSize);
    QPixmap pix     = m_icon.pixmap(size, QIcon::Disabled);
    m_icon_disabled = QIcon(pix);

    m_button->setEnabled(m_button->isEnabled());
    showIcon(m_button->isChecked());

    connect(m_button, SIGNAL(toggled(bool)),
            this, SLOT(showIcon(bool)));
}

ButtonIconDisabler::~ButtonIconDisabler()
{
}

void ButtonIconDisabler::showIcon(bool show)
{
    m_button->setIcon(show ? m_icon : m_icon_disabled);
}

} // namespace Digikam
