/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquickwebcontentsview.h"

// Needed to get access to content::GetContentClient()
#define CONTENT_IMPLEMENTATION

#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"
#include "ipc/ipc_message.h"

#include "browser_context_qt.h"
#include "content_browser_client_qt.h"
#include "web_contents_delegate_qt.h"

#include <QWidget>
#include <QUrl>

class QQuickWebContentsViewPrivate
{
public:
    scoped_ptr<WebContentsDelegateQt> webContentsDelegate;
};

QQuickWebContentsView::QQuickWebContentsView()
{
    d.reset(new QQuickWebContentsViewPrivate);

    content::BrowserContext* browser_context = static_cast<ContentBrowserClientQt*>(content::GetContentClient()->browser())->browser_context();
    d->webContentsDelegate.reset(WebContentsDelegateQt::CreateNewWindow(browser_context, GURL(std::string("http://qt-project.org/")), NULL, MSG_ROUTING_NONE, gfx::Size(), this));
}

QQuickWebContentsView::~QQuickWebContentsView()
{
}

QUrl QQuickWebContentsView::url() const
{
    GURL gurl = d->webContentsDelegate->web_contents()->GetActiveURL();
    return QUrl(QString::fromStdString(gurl.spec()));
}

void QQuickWebContentsView::setUrl(const QUrl& url)
{
    QString urlString = url.toString();
    GURL gurl(urlString.toStdString());
    if (!gurl.has_scheme())
        gurl = GURL(std::string("http://") + urlString.toStdString());

    content::NavigationController::LoadURLParams params(gurl);
    params.transition_type = content::PageTransitionFromInt(content::PAGE_TRANSITION_TYPED | content::PAGE_TRANSITION_FROM_ADDRESS_BAR);
    d->webContentsDelegate->web_contents()->GetController().LoadURLWithParams(params);
    d->webContentsDelegate->web_contents()->GetView()->Focus();
}

void QQuickWebContentsView::goBack()
{
    d->webContentsDelegate->web_contents()->GetController().GoToOffset(-1);
    d->webContentsDelegate->web_contents()->GetView()->Focus();
}

void QQuickWebContentsView::goForward()
{
    d->webContentsDelegate->web_contents()->GetController().GoToOffset(1);
    d->webContentsDelegate->web_contents()->GetView()->Focus();
}

void QQuickWebContentsView::reload()
{
    d->webContentsDelegate->web_contents()->GetController().Reload(false);
    d->webContentsDelegate->web_contents()->GetView()->Focus();
}
