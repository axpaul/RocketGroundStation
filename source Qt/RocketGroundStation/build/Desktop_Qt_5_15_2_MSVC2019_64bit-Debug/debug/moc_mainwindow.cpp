/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "setSerialSettingsSig"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 20), // "SerialPort::Settings"
QT_MOC_LITERAL(4, 54, 13), // "serialOppened"
QT_MOC_LITERAL(5, 68, 1), // "p"
QT_MOC_LITERAL(6, 70, 12), // "serialClosed"
QT_MOC_LITERAL(7, 83, 15), // "handleErrorShow"
QT_MOC_LITERAL(8, 99, 5), // "error"
QT_MOC_LITERAL(9, 105, 11), // "settingShow"
QT_MOC_LITERAL(10, 117, 17), // "setSerialSettings"
QT_MOC_LITERAL(11, 135, 13), // "opennedSerial"
QT_MOC_LITERAL(12, 149, 12), // "closedSerial"
QT_MOC_LITERAL(13, 162, 14), // "openSerialPort"
QT_MOC_LITERAL(14, 177, 15), // "closeSerialPort"
QT_MOC_LITERAL(15, 193, 13), // "receptionData"
QT_MOC_LITERAL(16, 207, 13), // "receptionChek"
QT_MOC_LITERAL(17, 221, 4), // "data"
QT_MOC_LITERAL(18, 226, 5) // "about"

    },
    "MainWindow\0setSerialSettingsSig\0\0"
    "SerialPort::Settings\0serialOppened\0p\0"
    "serialClosed\0handleErrorShow\0error\0"
    "settingShow\0setSerialSettings\0"
    "opennedSerial\0closedSerial\0openSerialPort\0"
    "closeSerialPort\0receptionData\0"
    "receptionChek\0data\0about"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    0,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   81,    2, 0x0a /* Public */,
       9,    0,   84,    2, 0x0a /* Public */,
      10,    0,   85,    2, 0x0a /* Public */,
      11,    1,   86,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x0a /* Public */,
      15,    2,   92,    2, 0x0a /* Public */,
      18,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::QByteArray,   16,   17,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setSerialSettingsSig((*reinterpret_cast< SerialPort::Settings(*)>(_a[1]))); break;
        case 1: _t->serialOppened((*reinterpret_cast< SerialPort::Settings(*)>(_a[1]))); break;
        case 2: _t->serialClosed(); break;
        case 3: _t->handleErrorShow((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->settingShow(); break;
        case 5: _t->setSerialSettings(); break;
        case 6: _t->opennedSerial((*reinterpret_cast< SerialPort::Settings(*)>(_a[1]))); break;
        case 7: _t->closedSerial(); break;
        case 8: _t->openSerialPort(); break;
        case 9: _t->closeSerialPort(); break;
        case 10: _t->receptionData((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 11: _t->about(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(SerialPort::Settings );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::setSerialSettingsSig)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(SerialPort::Settings );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::serialOppened)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::serialClosed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::setSerialSettingsSig(SerialPort::Settings _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::serialOppened(SerialPort::Settings _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::serialClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
