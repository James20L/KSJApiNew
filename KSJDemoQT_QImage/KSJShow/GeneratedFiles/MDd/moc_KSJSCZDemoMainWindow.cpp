/****************************************************************************
** Meta object code from reading C++ file 'KSJSCZDemoMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../KSJSCZDemoMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'KSJSCZDemoMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CKSJSCZDemoMainWindow_t {
    QByteArrayData data[11];
    char stringdata0[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CKSJSCZDemoMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CKSJSCZDemoMainWindow_t qt_meta_stringdata_CKSJSCZDemoMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 21), // "CKSJSCZDemoMainWindow"
QT_MOC_LITERAL(1, 22, 9), // "OnCapture"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 15), // "OnRefreshDevice"
QT_MOC_LITERAL(4, 49, 21), // "OnSaveImagePushButton"
QT_MOC_LITERAL(5, 71, 18), // "OnSetFovPushButton"
QT_MOC_LITERAL(6, 90, 14), // "OnSelectDevice"
QT_MOC_LITERAL(7, 105, 19), // "OnTrigetModeChanged"
QT_MOC_LITERAL(8, 125, 26), // "OnMirrorChkBoxStateChanged"
QT_MOC_LITERAL(9, 152, 24), // "OnFlipChkBoxStateChanged"
QT_MOC_LITERAL(10, 177, 34) // "OnFixFrameRateDoubleSpinBoxCh..."

    },
    "CKSJSCZDemoMainWindow\0OnCapture\0\0"
    "OnRefreshDevice\0OnSaveImagePushButton\0"
    "OnSetFovPushButton\0OnSelectDevice\0"
    "OnTrigetModeChanged\0OnMirrorChkBoxStateChanged\0"
    "OnFlipChkBoxStateChanged\0"
    "OnFixFrameRateDoubleSpinBoxChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CKSJSCZDemoMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x09 /* Protected */,
       3,    0,   60,    2, 0x09 /* Protected */,
       4,    0,   61,    2, 0x09 /* Protected */,
       5,    0,   62,    2, 0x09 /* Protected */,
       6,    1,   63,    2, 0x09 /* Protected */,
       7,    1,   66,    2, 0x09 /* Protected */,
       8,    1,   69,    2, 0x09 /* Protected */,
       9,    1,   72,    2, 0x09 /* Protected */,
      10,    1,   75,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void CKSJSCZDemoMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CKSJSCZDemoMainWindow *_t = static_cast<CKSJSCZDemoMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnCapture(); break;
        case 1: _t->OnRefreshDevice(); break;
        case 2: _t->OnSaveImagePushButton(); break;
        case 3: _t->OnSetFovPushButton(); break;
        case 4: _t->OnSelectDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->OnTrigetModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->OnMirrorChkBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->OnFlipChkBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnFixFrameRateDoubleSpinBoxChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CKSJSCZDemoMainWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CKSJSCZDemoMainWindow.data,
      qt_meta_data_CKSJSCZDemoMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CKSJSCZDemoMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CKSJSCZDemoMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CKSJSCZDemoMainWindow.stringdata0))
        return static_cast<void*>(const_cast< CKSJSCZDemoMainWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int CKSJSCZDemoMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
