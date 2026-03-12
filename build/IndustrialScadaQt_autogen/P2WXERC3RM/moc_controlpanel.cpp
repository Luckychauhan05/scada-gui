/****************************************************************************
** Meta object code from reading C++ file 'controlpanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../pages/controlpanel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ControlPanelPage_t {
    uint offsetsAndSizes[28];
    char stringdata0[17];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[8];
    char stringdata5[9];
    char stringdata6[11];
    char stringdata7[10];
    char stringdata8[11];
    char stringdata9[12];
    char stringdata10[14];
    char stringdata11[23];
    char stringdata12[23];
    char stringdata13[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ControlPanelPage_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ControlPanelPage_t qt_meta_stringdata_ControlPanelPage = {
    {
        QT_MOC_LITERAL(0, 16),  // "ControlPanelPage"
        QT_MOC_LITERAL(17, 13),  // "eventOccurred"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 6),  // "source"
        QT_MOC_LITERAL(39, 7),  // "message"
        QT_MOC_LITERAL(47, 8),  // "severity"
        QT_MOC_LITERAL(56, 10),  // "startPump1"
        QT_MOC_LITERAL(67, 9),  // "stopPump1"
        QT_MOC_LITERAL(77, 10),  // "openValveA"
        QT_MOC_LITERAL(88, 11),  // "closeValveA"
        QT_MOC_LITERAL(100, 13),  // "emergencyStop"
        QT_MOC_LITERAL(114, 22),  // "openAlumPump1Faceplate"
        QT_MOC_LITERAL(137, 22),  // "openAlumPump2Faceplate"
        QT_MOC_LITERAL(160, 20)   // "refreshFromSimulator"
    },
    "ControlPanelPage",
    "eventOccurred",
    "",
    "source",
    "message",
    "severity",
    "startPump1",
    "stopPump1",
    "openValveA",
    "closeValveA",
    "emergencyStop",
    "openAlumPump1Faceplate",
    "openAlumPump2Faceplate",
    "refreshFromSimulator"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ControlPanelPage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   68,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   75,    2, 0x08,    5 /* Private */,
       7,    0,   76,    2, 0x08,    6 /* Private */,
       8,    0,   77,    2, 0x08,    7 /* Private */,
       9,    0,   78,    2, 0x08,    8 /* Private */,
      10,    0,   79,    2, 0x08,    9 /* Private */,
      11,    0,   80,    2, 0x08,   10 /* Private */,
      12,    0,   81,    2, 0x08,   11 /* Private */,
      13,    0,   82,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ControlPanelPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ControlPanelPage.offsetsAndSizes,
    qt_meta_data_ControlPanelPage,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ControlPanelPage_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ControlPanelPage, std::true_type>,
        // method 'eventOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'startPump1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopPump1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openValveA'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeValveA'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'emergencyStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openAlumPump1Faceplate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openAlumPump2Faceplate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshFromSimulator'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ControlPanelPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanelPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->eventOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 1: _t->startPump1(); break;
        case 2: _t->stopPump1(); break;
        case 3: _t->openValveA(); break;
        case 4: _t->closeValveA(); break;
        case 5: _t->emergencyStop(); break;
        case 6: _t->openAlumPump1Faceplate(); break;
        case 7: _t->openAlumPump2Faceplate(); break;
        case 8: _t->refreshFromSimulator(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControlPanelPage::*)(const QString & , const QString & , const QString & );
            if (_t _q_method = &ControlPanelPage::eventOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ControlPanelPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanelPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanelPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanelPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ControlPanelPage::eventOccurred(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
