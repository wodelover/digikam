/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-02-05
 * Description : file action manager task list
 *
 * Copyright (C) 2012 by Marcel Wiesweg <marcel.wiesweg@gmx.de>
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

#ifndef FILEACTIONIMAGEINFOLIST_H
#define FILEACTIONIMAGEINFOLIST_H

// Qt includes

#include <QAtomicPointer>
#include <QList>
#include <QExplicitlySharedDataPointer>

// Local includes

#include "imageinfo.h"
#include "progressmanager.h"

namespace Digikam
{

class FileActionProgressItemCreator
{
public:
    virtual ~FileActionProgressItemCreator() {}
    virtual ProgressItem* createProgressItem(const QString& action) = 0;
    virtual void addProgressItem(ProgressItem* item) = 0;
};

class TwoProgressItemsContainer : public QSharedData
{
public:

    ProgressItem* first() const { return firstItem; }
    ProgressItem* second() const { return secondItem; }

    void createFirstItem(const QString& action, FileActionProgressItemCreator* creator)
        { createProgressItem(firstItem, action, creator); }
    void createSecondItem(const QString& action, FileActionProgressItemCreator* creator)
        { createProgressItem(secondItem, action, creator); }

    void checkFinishFirst() { checkFinish(firstItem); }
    void checkFinishSecond() { checkFinish(secondItem); }

protected:

    QAtomicPointer<ProgressItem> firstItem;
    QAtomicPointer<ProgressItem> secondItem;

    void createProgressItem(QAtomicPointer<ProgressItem>& ptr, const QString& action, FileActionProgressItemCreator* creator);
    void checkFinish(QAtomicPointer<ProgressItem>& ptr);
};

class FileActionProgressItemContainer : public TwoProgressItemsContainer
{
public:

    void schedulingForDB(int numberOfInfos, const QString& action, FileActionProgressItemCreator* creator);
    void dbProcessed(int numberOfInfos);
    void dbFinished();
    void schedulingForWrite(int numberOfInfos, const QString& action, FileActionProgressItemCreator* creator);
    void written(int numberOfInfos);
    void finishedWriting();

};

class FileActionImageInfoList : public QList<ImageInfo>
{
public:

    FileActionImageInfoList() {}

    static FileActionImageInfoList create(const QList<ImageInfo>& list);
    static FileActionImageInfoList continueTask(const QList<ImageInfo>& list, FileActionProgressItemContainer* container);

    FileActionProgressItemContainer* progress() const { return container.data(); }

    /// before sending to db worker
    void schedulingForDB(int numberOfInfos, const QString& action, FileActionProgressItemCreator* creator)
        { progress()->schedulingForDB(numberOfInfos, action, creator); }
    void schedulingForDB(const QString& action, FileActionProgressItemCreator* creator)
        { schedulingForDB(size(), action, creator); }

    /// db worker progress info
    void dbProcessedOne() { dbProcessed(1); }
    void dbProcessed(int numberOfInfos) { progress()->dbProcessed(numberOfInfos); }

    void dbFinished() { progress()->dbFinished(); }

    /// db worker calls this before sending to file worker
    void schedulingForWrite(int numberOfInfos, const QString& action, FileActionProgressItemCreator* creator)
        { progress()->schedulingForWrite(numberOfInfos, action, creator); }
    void schedulingForWrite(const QString& action, FileActionProgressItemCreator* creator)
        { schedulingForWrite(size(), action, creator); }

    /// file worker calls this when finished
    void writtenToOne() { written(1); }
    void written(int numberOfInfos) { progress()->written(numberOfInfos); }

    void finishedWriting() { progress()->finishedWriting(); }

private:

    FileActionImageInfoList(const QList<ImageInfo>& list) : QList<ImageInfo>(list) {}

    QExplicitlySharedDataPointer<FileActionProgressItemContainer> container;
};


}

#endif

