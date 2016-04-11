/*****************************************************************************
 * ConsoleRenderer.h: Handle the "server" mode rendering
 *****************************************************************************
 * Copyright (C) 2008-2010 VideoLAN
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "ConsoleRenderer.h"
#include "Main/Core.h"
#include "Project/Project.h"
#include "Renderer/WorkflowRenderer.h"

#include <QCoreApplication>
#include <QStringList>

ConsoleRenderer::ConsoleRenderer(QObject *parent) :
    QObject(parent)
{
    m_outputFileName = qApp->arguments()[2];
    m_width = Core::instance()->project()->width();
    m_height = Core::instance()->project()->height();
    m_fps = Core::instance()->project()->fps();
    m_ar = Core::instance()->project()->aspectRatio();
    m_vbitrate = Core::instance()->project()->videoBitrate();
    m_abitrate = Core::instance()->project()->audioBitrate();
    connect( Core::instance()->workflow(), &MainWorkflow::frameChanged,
             this, &ConsoleRenderer::frameChanged);
    connect( Core::instance()->workflowRenderer(), SIGNAL( renderComplete() ), qApp, SLOT( quit() ) );
}

void
ConsoleRenderer::frameChanged( qint64 frame ) const
{
    static int      percent = 0;
    int             newPercent;

    newPercent = frame * 100 / Core::instance()->workflow()->getLengthFrame();
    if ( newPercent != percent )
    {
        percent = newPercent;
        qDebug().nospace() << percent << "%";
    }
}

void
ConsoleRenderer::startRender()
{
    Core::instance()->workflowRenderer()->startRenderToFile( m_outputFileName, m_width, m_height, m_fps, m_ar, m_vbitrate, m_abitrate );
}
