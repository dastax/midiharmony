/****************************************************************************
** Meta object code from reading C++ file 'Harmony.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Harmony.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Harmony.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Harmony[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,    9,    8,    8, 0x0a,
      47,   39,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Harmony[] = {
    "Harmony\0\0ev\0handleMidiEvent(MidiEvent)\0"
    "curtime\0rollArpeggios(jack_time_t)\0"
};

void Harmony::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Harmony *_t = static_cast<Harmony *>(_o);
        switch (_id) {
        case 0: _t->handleMidiEvent((*reinterpret_cast< MidiEvent(*)>(_a[1]))); break;
        case 1: _t->rollArpeggios((*reinterpret_cast< jack_time_t(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Harmony::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Harmony::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Harmony,
      qt_meta_data_Harmony, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Harmony::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Harmony::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Harmony::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Harmony))
        return static_cast<void*>(const_cast< Harmony*>(this));
    return QObject::qt_metacast(_clname);
}

int Harmony::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
