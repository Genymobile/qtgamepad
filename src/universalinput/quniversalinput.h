// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

/*
  Originally based on code from "core/input/input.h" from Godot Engine v4.0
  Copyright (c) 2014-present Godot Engine contributors
  Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
*/

#ifndef QUNIVERSALINPUT_H
#define QUNIVERSALINPUT_H

#include <QtCore/QObject>
#include <QtCore/QElapsedTimer>
#include <QtCore/QString>
#include <QtGui/QVector2D>
#include <QtUniversalInput/qtuniversalinputglobal.h>
#include <QtGui/QMouseEvent>

QT_BEGIN_NAMESPACE

enum class HatDirection {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
    Max = 4,
};

enum class HatMask {
    Center = 0,
    Up = 1,
    Right = 2,
    Down = 4,
    Left = 8,
};

enum class JoyAxis {
    Invalid = -1,
    LeftX = 0,
    LeftY = 1,
    RightX = 2,
    RightY = 3,
    TriggerLeft = 4,
    TriggerRight = 5,
    MAX = 10,
};

enum class JoyButton {
    Invalid = -1,
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    Back = 4,
    Guide = 5,
    Start = 6,
    LeftStick = 7,
    RightStick = 8,
    LeftShoulder = 9,
    RightShoulder = 10,
    DpadUp = 11,
    DpadDown = 12,
    DpadLeft = 13,
    DpadRight = 14,
    Misc1 = 15,
    Paddle1 = 16,
    Paddle2 = 17,
    Paddle3 = 18,
    Paddle4 = 19,
    Touchpad = 20,
    MAX = 128
};

inline HatMask operator|(HatMask a, HatMask b) {
    return (HatMask)((int)a | (int)b);
}

inline HatMask operator&(HatMask a, HatMask b) {
    return (HatMask)((int)a & (int)b);
}

inline HatMask &operator&=(HatMask &a, HatMask b) {
    return (HatMask &)((int &)a &= (int)b);
}

inline HatMask &operator|=(HatMask &a, HatMask b) {
    return (HatMask &)((int &)a |= (int)b);
}

inline HatMask operator~(HatMask a) {
    return (HatMask)(~(int)a);
}

class QUniversalInputPrivate;
class Q_UNIVERSALINPUT_EXPORT QUniversalInput : public QObject
{
    Q_OBJECT
public:
    enum {
        JoypadsMax = 16,
    };

    struct Action {
        quint64 frame;
        bool isPressed;
        bool isExact;
        float strength;
        float rawStrength;
    };
    struct VibrationInfo {
        float weakMagnitude;
        float strongMagnitude;
        float duration;
        quint64 timestamp;
    };
    struct VelocityTrack {
        QElapsedTimer frameTimer;
        QVector2D velocity;
        QVector2D accum;
        float accumTime = 0.0;
        float minRefFrame;
        float maxRefFrame;

        void update(const QVector2D &delta);
        void reset();
        VelocityTrack();
    };

    struct Joypad {
        QString name;
        QString uid;
        bool isConnected = false;
        bool lastButtons[size_t(JoyButton::MAX)] = { false };
        float lastAxis[size_t(JoyAxis::MAX)] = { 0.0f };
        HatMask lastHat = HatMask::Center;
        int mapping = -1;
        int hatCurrent = 0;
    };

    enum JoyType {
        TypeButton,
        TypeAxis,
        TypeHat,
        TypeMax,
    };

    enum JoyAxisRange {
        NegativeHalfAxis = -1,
        FullAxis = 0,
        PositiveHalfAxis = 1
    };

    struct JoyEvent {
        int type = TypeMax;
        int index = -1;
        float value = 0.0f;
    };

    struct JoyBinding {
        JoyType inputType;
        union {
            JoyButton button;

            struct {
                JoyAxis axis;
                JoyAxisRange range;
                bool invert;
            } axis;

            struct {
                HatDirection hat;
                HatMask hat_mask;
            } hat;

        } input;

        JoyType outputType;
        union {
            JoyButton button;

            struct {
                JoyAxis axis;
                JoyAxisRange range;
            } axis;

        } output;
    };

    struct JoyDeviceMapping {
        QString uid;
        QString name;
        QVector<JoyBinding> bindings;
    };

    static QUniversalInput *instance();

    QString getJoyName(int device) const;
    bool isJoyConnected(int device) const;
    bool isGamepad(int device) const;

    // API used by platform specific plugins
    // Joypad/Joystick/Gamepads
    int getUnusedJoyId();
    void updateJoyConnection(int index, bool isConnected, const QString &name, const QString &guid = QString());

    void joyButton(int device, JoyButton button, bool isPressed);
    void joyAxis(int device, JoyAxis axis, float value);
    void joyHat(int device, HatMask value);

    // Force Feedback
    QVector2D getJoyVibrationStrength(int device);
    float getJoyVibrationDuration(int device);
    quint64 getJoyVibrationTimestamp(int device);
    void addForce(int deivce, QVector2D strength, float duration);

    void setJoyAxis(int device, JoyAxis axis, float value);

    void setMouseDisabled(bool disabled); // looking for better name. Is "make invisible and center"
    bool isMouseDisabled() const;
    void mouseMove(const QVector2D& deltas);

Q_SIGNALS:
    void joyConnectionChanged(int index, bool isConnected);
    void joyButtonEvent(int device, JoyButton button, bool isPressed);
    void joyAxisEvent(int device, JoyAxis axis, float value);

    void mouseDisabledChanged();
    void mouseMovedWithDeltas(const QVector2D& deltas);

private Q_SLOTS:
    void loadPlugins();

private:
    QUniversalInput();
    ~QUniversalInput();

    void sendButtonEvent(int device, JoyButton index, bool pressed);
    void sendAxisEvent(int device, JoyAxis axis, float value);
    JoyEvent mappedButtonEvent(const JoyDeviceMapping &mapping, JoyButton button);
    JoyEvent mappedAxisEvent(const JoyDeviceMapping &mapping, JoyAxis axis, float inValue);
    void mappedHatEvents(const JoyDeviceMapping &mapping, HatDirection hat, JoyEvent events[size_t(HatDirection::Max)]);

    Q_DECLARE_PRIVATE(QUniversalInput)
    Q_DISABLE_COPY(QUniversalInput)
};

Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const JoyButton &joyButton);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const JoyAxis &axis);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const QUniversalInput::JoyAxisRange &range);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const HatDirection &hatDirection);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const HatMask &hatMask);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const QUniversalInput::JoyBinding &binding);
Q_UNIVERSALINPUT_EXPORT QDebug operator<<(QDebug debug, const QUniversalInput::JoyDeviceMapping &mapping);



QT_END_NAMESPACE

Q_DECLARE_METATYPE(JoyButton)
Q_DECLARE_METATYPE(JoyAxis)

#endif // QUNIVERSALINPUT_H
