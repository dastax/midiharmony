/****************************************************************************
** Meta object code from reading C++ file 'midiharmony.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../midiharmony.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midiharmony.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiHarmony[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      23,   12,   12,   12, 0x08,
      39,   34,   12,   12, 0x08,
      57,   12,   12,   12, 0x08,
      68,   12,   12,   12, 0x08,
      81,   34,   12,   12, 0x08,
      99,   12,   12,   12, 0x08,
     117,   12,   12,   12, 0x08,
     130,   12,   12,   12, 0x08,
     145,  143,   12,   12, 0x08,
     204,   12,   12,   12, 0x08,
     217,   12,   12,   12, 0x08,
     236,   12,   12,   12, 0x08,
     256,   12,   12,   12, 0x08,
     279,   12,   12,   12, 0x08,
     299,   12,   12,   12, 0x08,
     334,   12,   12,   12, 0x08,
     365,   12,   12,   12, 0x08,
     390,   12,   12,   12, 0x08,
     416,   12,   12,   12, 0x08,
     435,   12,   12,   12, 0x08,
     460,   12,   12,   12, 0x08,
     500,  489,   12,   12, 0x08,
     537,  534,   12,   12, 0x08,
     568,  489,   12,   12, 0x08,
     606,  534,   12,   12, 0x08,
     641,   12,   12,   12, 0x08,
     668,  665,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MidiHarmony[] = {
    "MidiHarmony\0\0newFile()\0loadFile()\0"
    "path\0loadFile(QString)\0saveFile()\0"
    "saveFileAs()\0saveFile(QString)\0"
    "jackCtrlClicked()\0addTrigger()\0"
    "delTrigger()\0,\0"
    "triggerSelectionChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "toggleMode()\0selectHarmony(int)\0"
    "harmonyChanged(int)\0arpTriggerChanged(int)\0"
    "arpModeChanged(int)\0"
    "arpCustomIntervalsChanged(QString)\0"
    "arpCustomTimesChanged(QString)\0"
    "arpIntervalsChanged(int)\0"
    "arpSlideRangeChanged(int)\0arpBPMChanged(int)\0"
    "arpNoteSpeedChanged(int)\0"
    "arpConstantSpeedChanged(int)\0index,name\0"
    "savePatternRequested(int,QString)\0pr\0"
    "curPatternChanged(QDomElement)\0"
    "saveTimePatternRequested(int,QString)\0"
    "curTimePatternChanged(QDomElement)\0"
    "learnIntervalsClicked()\0ev\0"
    "learnMidiIn(MidiEvent)\0"
};

void MidiHarmony::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MidiHarmony *_t = static_cast<MidiHarmony *>(_o);
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->loadFile(); break;
        case 2: _t->loadFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->saveFile(); break;
        case 4: _t->saveFileAs(); break;
        case 5: _t->saveFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->jackCtrlClicked(); break;
        case 7: _t->addTrigger(); break;
        case 8: _t->delTrigger(); break;
        case 9: _t->triggerSelectionChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 10: _t->toggleMode(); break;
        case 11: _t->selectHarmony((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->harmonyChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->arpTriggerChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->arpModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->arpCustomIntervalsChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->arpCustomTimesChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->arpIntervalsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->arpSlideRangeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->arpBPMChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->arpNoteSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->arpConstantSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->savePatternRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 23: _t->curPatternChanged((*reinterpret_cast< QDomElement(*)>(_a[1]))); break;
        case 24: _t->saveTimePatternRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 25: _t->curTimePatternChanged((*reinterpret_cast< QDomElement(*)>(_a[1]))); break;
        case 26: _t->learnIntervalsClicked(); break;
        case 27: _t->learnMidiIn((*reinterpret_cast< MidiEvent(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MidiHarmony::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MidiHarmony::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MidiHarmony,
      qt_meta_data_MidiHarmony, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiHarmony::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiHarmony::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiHarmony::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiHarmony))
        return static_cast<void*>(const_cast< MidiHarmony*>(this));
    return QWidget::qt_metacast(_clname);
}

int MidiHarmony::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
