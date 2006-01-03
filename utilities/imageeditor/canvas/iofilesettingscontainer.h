/* ============================================================
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 * Date  : 2006-01-03
 * Description : IO file Settings Container.
 * 
 * Copyright 2005 by Gilles Caulier
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

#ifndef IOFILESETTINGSCONTAINER_H
#define IOFILESETTINGSCONTAINER_H

namespace Digikam
{

class IOFileSettingsContainer
{

public:
    
    IOFileSettingsContainer(){};
    ~IOFileSettingsContainer(){};

public:

    // JPEG quality value.
    int  JPEGCompression;

    // PNG compression value.
    int  PNGCompression;

    // TIFF deflat compression.
    bool TIFFCompression;

    // RAW file decoding using quality factor. 
    bool enableRAWQuality;

    // RAW quality decoding factor value.
    int  RAWquality;
    
};

}  // namespace Digikam

#endif  // IOFILESETTINGSCONTAINER_H
