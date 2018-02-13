/****************************************************************************
** Meta object code from reading C++ file 'jack_engine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../jack_engine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jack_engine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JackEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_JackEngine[] = {
    "JackEngine\0\0doRollArpeggios(jack_time_t)\0"
    "handleMidiEvent(MidiEvent)\0"
};

void JackEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        JackEngine *_t = static_cast<JackEngine *>(_o);
        switch (_id) {
        case 0: _t->doRollArpeggios((*reinterpret_cast< jack_time_t(*)>(_a[1]))); break;
        case 1: _t->handleMidiEvent((*reinterpret_cast< MidiEvent(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData JackEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject JackEngine::staticMetaObject = {
    { &JackClient::staticMetaObject, qt_meta_stringdata_JackEngine,
      qt_meta_data_JackEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JackEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JackEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JackEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JackEngine))
        return static_cast<void*>(const_cast< JackEngine*>(this));
    return JackClient::qt_metacast(_clname);
}

int JackEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = JackClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void JackEngine::doRollArpeggios(jack_time_t _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
