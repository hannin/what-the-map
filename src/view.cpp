/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "view.h"

#include <QtWidgets>

#ifndef QT_NO_WHEELEVENT
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            this->_view->zoomIn(60);
        else
            this->_view->zoomOut(60);
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

View::View(const QString &name, QWidget *parent)
    : QFrame(parent)
{
    this->setFrameStyle(Sunken | StyledPanel);
    this->_graphicsView = new GraphicsView(this);
    this->_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    this->_graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    this->_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    this->_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = this->style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap("./res/ic_zoom_in_black_24dp.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap("./res/ic_zoom_out_black_24dp.png"));
    zoomOutIcon->setIconSize(iconSize);
    this->_zoomSlider = new QSlider;
    this->_zoomSlider->setMinimum(-200);
    this->_zoomSlider->setMaximum(400);
    this->_zoomSlider->setValue(-100);
    this->_zoomSlider->setTickPosition(QSlider::TicksRight);
    this->_zoomSlider->setTickInterval(40);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomOutIcon);
    zoomSliderLayout->addWidget(this->_zoomSlider);

    this->_resetButton = new QToolButton;
    this->_resetButton->setText(tr("0"));
    this->_resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    this->_label = new QLabel(name);
    this->_antialiasButton = new QToolButton;
    this->_antialiasButton->setText(tr("Antialiasing"));
    this->_antialiasButton->setCheckable(true);
    this->_antialiasButton->setChecked(false);
    labelLayout->addWidget(this->_label);
    labelLayout->addStretch();
    labelLayout->addWidget(this->_antialiasButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(this->_graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addWidget(this->_resetButton, 2, 1);
    setLayout(topLayout);

    connect(this->_resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(this->_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(this->_graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
        this, SLOT(setResetButtonEnabled()));
    connect(this->_graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
        this, SLOT(setResetButtonEnabled()));
    connect(this->_antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));

    this->setupMatrix();
}

QGraphicsView *View::view() const
{
    return dynamic_cast<QGraphicsView *>(this->_graphicsView);
}

void View::resetView()
{
    //this->_zoomSlider->setValue(250);
    setupMatrix();
    this->_graphicsView->ensureVisible(QRectF(0, 0, 0, 0));
    this->_resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    this->_resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (this->_zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    this->_graphicsView->setMatrix(matrix);
    this->setResetButtonEnabled();
}

void View::toggleAntialiasing()
{
    this->_graphicsView->setRenderHint(QPainter::Antialiasing, this->_antialiasButton->isChecked());
}

void View::zoomIn(int level)
{
    this->_zoomSlider->setValue(this->_zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
    this->_zoomSlider->setValue(this->_zoomSlider->value() - level);
}