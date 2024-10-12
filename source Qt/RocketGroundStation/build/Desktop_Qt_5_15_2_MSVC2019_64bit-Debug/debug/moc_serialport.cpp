/****************************************************************************
** Meta object code from reading C++ file 'serialport.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../serialport.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SerialPort_t {
    QByteArrayData data[17];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPort_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPort_t qt_meta_stringdata_SerialPort = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SerialPort"
QT_MOC_LITERAL(1, 11, 9), // "errorEmit"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "dataEmit"
QT_MOC_LITERAL(4, 31, 13), // "receptionChek"
QT_MOC_LITERAL(5, 45, 4), // "data"
QT_MOC_LITERAL(6, 50, 13), // "serialOpenned"
QT_MOC_LITERAL(7, 64, 20), // "SerialPort::Settings"
QT_MOC_LITERAL(8, 85, 1), // "p"
QT_MOC_LITERAL(9, 87, 12), // "serialClosed"
QT_MOC_LITERAL(10, 100, 10), // "openSerial"
QT_MOC_LITERAL(11, 111, 11), // "closeSerial"
QT_MOC_LITERAL(12, 123, 13), // "settingUpdate"
QT_MOC_LITERAL(13, 137, 11), // "settingPort"
QT_MOC_LITERAL(14, 149, 11), // "handleError"
QT_MOC_LITERAL(15, 161, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(16, 190, 5) // "error"

    },
    "SerialPort\0errorEmit\0\0dataEmit\0"
    "receptionChek\0data\0serialOpenned\0"
    "SerialPort::Settings\0p\0serialClosed\0"
    "openSerial\0closeSerial\0settingUpdate\0"
    "settingPort\0handleError\0"
    "QSerialPort::SerialPortError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPort[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    2,   62,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x26 /* Public | MethodCloned */,
       6,    1,   70,    2, 0x06 /* Public */,
       9,    0,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   74,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,
      12,    1,   76,    2, 0x0a /* Public */,
      14,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::QByteArray,    4,    5,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,   13,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void SerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialPort *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorEmit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->dataEmit((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 2: _t->dataEmit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->serialOpenned((*reinterpret_cast< SerialPort::Settings(*)>(_a[1]))); break;
        case 4: _t->serialClosed(); break;
        case 5: _t->openSerial(); break;
        case 6: _t->closeSerial(); break;
        case 7: _t->settingUpdate((*reinterpret_cast< SerialPort::Settings(*)>(_a[1]))); break;
        case 8: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialPort::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::errorEmit)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)(bool , QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::dataEmit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)(SerialPort::Settings );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::serialOpenned)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SerialPort::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialPort::serialClosed)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialPort::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_SerialPort.data,
    qt_meta_data_SerialPort,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPort.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SerialPort::errorEmit(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SerialPort::dataEmit(bool _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 3
void SerialPort::serialOpenned(SerialPort::Settings _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialPort::serialClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
