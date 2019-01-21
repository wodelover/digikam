/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2018-07-30
 * Description : image editor plugin to emulate charcoal drawing.
 *
 * Copyright (C) 2018-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "charcoaltoolplugin.h"

// Qt includes

#include <QPointer>

// KDE includes

#include <klocalizedstring.h>

// Local includes

#include "editorwindow.h"
#include "charcoaltool.h"

namespace Digikam
{

CharcoalToolPlugin::CharcoalToolPlugin(QObject* const parent)
    : DPluginEditor(parent)
{
}

CharcoalToolPlugin::~CharcoalToolPlugin()
{
}

QString CharcoalToolPlugin::name() const
{
    return i18n("Charcoal Drawing");
}

QString CharcoalToolPlugin::iid() const
{
    return QLatin1String(DPLUGIN_IID);
}

QIcon CharcoalToolPlugin::icon() const
{
    return QIcon::fromTheme(QLatin1String("charcoaltool"));
}

QString CharcoalToolPlugin::description() const
{
    return i18n("A tool to emulate charcoal drawing");
}

QString CharcoalToolPlugin::details() const
{
    return i18n("<p>This Image Editor tool can emulate charcoal drawing to an image.</p>");
}

QList<DPluginAuthor> CharcoalToolPlugin::authors() const
{
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Gilles Caulier"),
                             QString::fromUtf8("caulier dot gilles at gmail dot com"),
                             QString::fromUtf8("(C) 2004-2019"))
            ;
}

void CharcoalToolPlugin::setup(QObject* const parent)
{
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setText(i18nc("@action", "Charcoal Drawing..."));
    ac->setObjectName(QLatin1String("editorwindow_filter_charcoal"));
    ac->setActionCategory(DPluginAction::EditorFilters);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotCharcoal()));

    addAction(ac);
}

void CharcoalToolPlugin::slotCharcoal()
{
    EditorWindow* const editor = dynamic_cast<EditorWindow*>(sender()->parent());

    if (editor)
    {
        CharcoalTool* const tool = new CharcoalTool(editor);
        tool->setPlugin(this);
        editor->loadTool(tool);
    }
}

} // namespace Digikam
