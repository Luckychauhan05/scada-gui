/****************************************************************************
** Meta object code from reading C++ file 'alarms.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../pages/alarms.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alarms.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_AlarmManagerPage_t {
    uint offsetsAndSizes[24];
    char stringdata0[17];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[15];
    char stringdata5[20];
    char stringdata6[8];
    char stringdata7[14];
    char stringdata8[7];
    char stringdata9[9];
    char stringdata10[14];
    char stringdata11[25];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AlarmManagerPage_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AlarmManagerPage_t qt_meta_stringdata_AlarmManagerPage = {
    {
        QT_MOC_LITERAL(0, 16),  // "AlarmManagerPage"
        QT_MOC_LITERAL(17, 17),  // "alarmStatsChanged"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 12),  // "activeAlarms"
        QT_MOC_LITERAL(49, 14),  // "criticalAlarms"
        QT_MOC_LITERAL(64, 19),  // "criticalAlarmRaised"
        QT_MOC_LITERAL(84, 7),  // "message"
        QT_MOC_LITERAL(92, 13),  // "eventOccurred"
        QT_MOC_LITERAL(106, 6),  // "source"
        QT_MOC_LITERAL(113, 8),  // "severity"
        QT_MOC_LITERAL(122, 13),  // "generateAlarm"
        QT_MOC_LITERAL(136, 24)   // "acknowledgeSelectedAlarm"
    },
    "AlarmManagerPage",
    "alarmStatsChanged",
    "",
    "activeAlarms",
    "criticalAlarms",
    "criticalAlarmRaised",
    "message",
    "eventOccurred",
    "source",
    "severity",
    "generateAlarm",
    "acknowledgeSelectedAlarm"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AlarmManagerPage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x06,    1 /* Public */,
       5,    1,   49,    2, 0x06,    4 /* Public */,
       7,    3,   52,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    0,   59,    2, 0x08,   10 /* Private */,
      11,    0,   60,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    6,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AlarmManagerPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AlarmManagerPage.offsetsAndSizes,
    qt_meta_data_AlarmManagerPage,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AlarmManagerPage_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AlarmManagerPage, std::true_type>,
        // method 'alarmStatsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'criticalAlarmRaised'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'eventOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'generateAlarm'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'acknowledgeSelectedAlarm'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AlarmManagerPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AlarmManagerPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->alarmStatsChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->criticalAlarmRaised((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->eventOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->generateAlarm(); break;
        case 4: _t->acknowledgeSelectedAlarm(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AlarmManagerPage::*)(int , int );
            if (_t _q_method = &AlarmManagerPage::alarmStatsChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AlarmManagerPage::*)(const QString & );
            if (_t _q_method = &AlarmManagerPage::criticalAlarmRaised; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AlarmManagerPage::*)(const QString & , const QString & , const QString & );
            if (_t _q_method = &AlarmManagerPage::eventOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *AlarmManagerPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlarmManagerPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AlarmManagerPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AlarmManagerPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AlarmManagerPage::alarmStatsChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AlarmManagerPage::criticalAlarmRaised(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AlarmManagerPage::eventOccurred(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
