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
#include "main-window.h"
#include "view.h"
#include "node-graphics-item.h"

#include <QHBoxLayout>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), _mapdata(new MapData)
{
    this->loadMapData();

    this->populateScene();
    View *view = new View("Main view");
    view->view()->setScene(this->_scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    this->setLayout(layout);

    this->setWindowTitle(tr("What The Map!!!"));
}

void MainWindow::loadMapData()
{
    this->_mapdata->load();
    this->transformCoordinates();
}

void MainWindow::transformCoordinates()
{
    auto &box = this->_mapdata->box;
    double x_side = box.x_max - box.x_min;
    double y_side = box.y_max - box.y_min;
    //int x_div = x_side / 30000;
    //int y_div = y_side / 30000;
    int x_div = 1;
    int y_div = 1;
    double x_padding = box.x_min + x_side / 2;
    double y_padding = box.y_min + y_side / 2;

    auto x_transformer = [&](double &x) {
        x = (x - x_padding) / x_div;
    };
    auto y_transformer = [&](double &y) {
        y = (y - y_padding) / y_div;
    };

    this->_mapdata->transformCoordinates(
        x_transformer, y_transformer, MapData::ReverseState::Y);
}

void MainWindow::populateScene()
{
    this->_scene = new QGraphicsScene(this);

    for (auto &division : this->_mapdata->adj_map.divided_map) {
        NodeGraphicsItem *item = new NodeGraphicsItem(division.first, this->_mapdata);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        //item->setCacheMode(QGraphicsItem::ItemCoordinateCache);
        this->_scene->addItem(item);
    }
}