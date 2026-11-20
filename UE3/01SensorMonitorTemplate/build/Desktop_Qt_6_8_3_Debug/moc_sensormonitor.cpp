/****************************************************************************
** Meta object code from reading C++ file 'sensormonitor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sensormonitor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensormonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13SensorMonitorE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13SensorMonitorE = QtMocHelpers::stringData(
    "SensorMonitor",
    "btnLED1Clicked",
    "",
    "btnLED2Clicked",
    "btnLED3Clicked",
    "btnStartTempClicked",
    "btnStopTempClicked",
    "setHardwareButton1",
    "value",
    "setHardwareButton2",
    "readTemperature",
    "spinBoxChanged"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN13SensorMonitorE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x09,    1 /* Protected */,
       3,    0,   69,    2, 0x09,    2 /* Protected */,
       4,    0,   70,    2, 0x09,    3 /* Protected */,
       5,    0,   71,    2, 0x09,    4 /* Protected */,
       6,    0,   72,    2, 0x09,    5 /* Protected */,
       7,    1,   73,    2, 0x09,    6 /* Protected */,
       9,    1,   76,    2, 0x09,    8 /* Protected */,
      10,    0,   79,    2, 0x09,   10 /* Protected */,
      11,    0,   80,    2, 0x09,   11 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SensorMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN13SensorMonitorE.offsetsAndSizes,
    qt_meta_data_ZN13SensorMonitorE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13SensorMonitorE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SensorMonitor, std::true_type>,
        // method 'btnLED1Clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btnLED2Clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btnLED3Clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btnStartTempClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'btnStopTempClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setHardwareButton1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setHardwareButton2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'readTemperature'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'spinBoxChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SensorMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SensorMonitor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->btnLED1Clicked(); break;
        case 1: _t->btnLED2Clicked(); break;
        case 2: _t->btnLED3Clicked(); break;
        case 3: _t->btnStartTempClicked(); break;
        case 4: _t->btnStopTempClicked(); break;
        case 5: _t->setHardwareButton1((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->setHardwareButton2((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->readTemperature(); break;
        case 8: _t->spinBoxChanged(); break;
        default: ;
        }
    }
}

const QMetaObject *SensorMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13SensorMonitorE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SensorMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
