/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2005-03-10
 * Description : a plugin to apply texture over an image
 * 
 * Copyright (C) 2005-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2006-2008 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#ifndef IMAGEEFFECT_TEXTURE_H
#define IMAGEEFFECT_TEXTURE_H

// Qt includes.

#include <qstring.h>

// Digikam includes.

#include "ctrlpaneldlg.h"

class QComboBox;

class KIntNumInput;

namespace DigikamTextureImagesPlugin
{

class ImageEffect_Texture : public Digikam::CtrlPanelDlg
{
    Q_OBJECT

public:

    ImageEffect_Texture(QWidget* parent);
    ~ImageEffect_Texture();

private:

    QString getTexturePath(int texture);

private slots:

    void readUserSettings();

private:

    void writeUserSettings();
    void resetValues();    
    void prepareEffect();
    void prepareFinal();
    void putPreviewData();
    void putFinalData();
    void renderingFinished();

private:

    enum TextureTypes 
    {
        PaperTexture=0,
        Paper2Texture,
        FabricTexture,
        BurlapTexture,
        BricksTexture,
        Bricks2Texture,
        CanvasTexture,
        MarbleTexture,
        Marble2Texture,
        BlueJeanTexture,
        CellWoodTexture,
        MetalWireTexture,
        ModernTexture,
        WallTexture,
        MossTexture,
        StoneTexture
    };

    QComboBox    *m_textureType;

    KIntNumInput *m_blendGain;
};

}  // NameSpace DigikamTextureImagesPlugin

#endif /* IMAGEEFFECT_TEXTURE_H */
