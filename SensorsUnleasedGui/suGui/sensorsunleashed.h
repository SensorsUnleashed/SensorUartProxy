/*******************************************************************************
 * Copyright (c) 2017, Ole Nissen.
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  1. Redistributions of source code must retain the above copyright 
 *  notice, this list of conditions and the following disclaimer. 
 *  2. Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following
 *  disclaimer in the documentation and/or other materials provided
 *  with the distribution. 
 *  3. The name of the author may not be used to endorse or promote
 *  products derived from this software without specific prior
 *  written permission.  
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This file is part of the Sensors Unleashed project
 *******************************************************************************/
#ifndef SENSORSUNLEASHED_H
#define SENSORSUNLEASHED_H

#include "database.h"
#include "coaphandler.h"
#include "node.h"
#include <QObject>
#include "sensorstore.h"

#include <QQmlContext>


/* This class serves as an interface between the the GUI and the c++, at
 * least until
*/
class sensorsunleashed : public QObject
{
    Q_OBJECT
public:
    explicit sensorsunleashed(database* db, coaphandler* coap, QQmlContext *context);

    Q_INVOKABLE void changeActiveNode(QVariant nodeinfo);
    Q_INVOKABLE QVariant changeActiveSensor(QVariant sensorinfo);

    Q_INVOKABLE void initNodelist();

    node* findNode(QString nodeid);
    Q_INVOKABLE QVariant createNode(QVariant nodeinfo);

    Q_INVOKABLE QVariantList getAllSensorsList();

private:
    database *db;
    coaphandler* nodecomm;
    QVector<node*> nodes;
    sensorstore* allsensorslist;
    QQmlContext *context;
signals:


    void nodeCreated(QVariant nodeinfo);

private slots:
    void updateDB(sensor* s);

public slots:


};

#endif // SENSORSUNLEASHED_H
