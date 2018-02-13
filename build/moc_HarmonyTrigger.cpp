/****************************************************************************
** Meta object code from reading C++ file 'HarmonyTrigger.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../HarmonyTrigger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HarmonyTrigger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HarmonyTrigger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

static const char qt_meta_stringdata_HarmonyTrigger[] = {
    "HarmonyTrigger\0"
};

void HarmonyTrigger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HarmonyTrigger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HarmonyTrigger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HarmonyTrigger,
      qt_meta_data_HarmonyTrigger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HarmonyTrigger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HarmonyTrigger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HarmonyTrigger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HarmonyTrigger))
        return static_cast<void*>(const_cast< HarmonyTrigger*>(this));
    return QObject::qt_metacast(_clname);
}

int HarmonyTrigger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_HarmonyTriggerWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      42,   21,   21,   21, 0x08,
      63,   21,   21,   21, 0x08,
      83,   21,   21,   21, 0x08,
     103,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HarmonyTriggerWidget[] = {
    "HarmonyTriggerWidget\0\0channelChanged(int)\0"
    "noteModeChanged(int)\0noteMinChanged(int)\0"
    "noteMaxChanged(int)\0noteChanged(int)\0"
};

void HarmonyTriggerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HarmonyTriggerWidget *_t = static_cast<HarmonyTriggerWidget *>(_o);
        switch (_id) {
        case 0: _t->channelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->noteModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->noteMinChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->noteMaxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->noteChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HarmonyTriggerWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HarmonyTriggerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_HarmonyTriggerWidget,
      qt_meta_data_HarmonyTriggerWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HarmonyTriggerWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HarmonyTriggerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HarmonyTriggerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HarmonyTriggerWidget))
        return static_cast<void*>(const_cast< HarmonyTriggerWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int HarmonyTriggerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
