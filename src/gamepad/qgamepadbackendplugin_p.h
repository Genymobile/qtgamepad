/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Gamepad module
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
****************************************************************************/

#ifndef QGAMEPADBACKENDPLUGIN_P_H
#define QGAMEPADBACKENDPLUGIN_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGamepad/qtgamepadglobal.h>

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

class QGamepadBackend;

#define QtGamepadBackendFactoryInterface_iid "org.qt-project.Qt.Gamepad.QtGamepadBackendFactoryInterface.5.9"

class Q_GAMEPAD_EXPORT QGamepadBackendPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QGamepadBackendPlugin(QObject *parent = nullptr);
    ~QGamepadBackendPlugin();

    virtual QGamepadBackend *create(const QString &key, const QStringList &paramList) = 0;

signals:

public slots:
};

QT_END_NAMESPACE

#endif // QGAMEPADBACKENDPLUGIN_P_H
