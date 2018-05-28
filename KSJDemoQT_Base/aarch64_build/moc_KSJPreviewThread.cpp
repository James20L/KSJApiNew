/****************************************************************************
** Meta object code from reading C++ file 'KSJPreviewThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qtdemo_linux/KSJPreviewThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'KSJPreviewThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CKSJPreviewThread_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CKSJPreviewThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CKSJPreviewThread_t qt_meta_stringdata_CKSJPreviewThread = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CKSJPreviewThread"
QT_MOC_LITERAL(1, 18, 11), // "UpdateImage"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "unsigned char*"
QT_MOC_LITERAL(4, 46, 5) // "pData"

    },
    "CKSJPreviewThread\0UpdateImage\0\0"
    "unsigned char*\0pData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CKSJPreviewThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void CKSJPreviewThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CKSJPreviewThread *_t = static_cast<CKSJPreviewThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateImage((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CKSJPreviewThread::*_t)(unsigned char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CKSJPreviewThread::UpdateImage)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CKSJPreviewThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CKSJPreviewThread.data,
      qt_meta_data_CKSJPreviewThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CKSJPreviewThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CKSJPreviewThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CKSJPreviewThread.stringdata0))
        return static_cast<void*>(const_cast< CKSJPreviewThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CKSJPreviewThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CKSJPreviewThread::UpdateImage(unsigned char * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
