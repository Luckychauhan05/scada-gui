/****************************************************************************
** Meta object code from reading C++ file 'alumclariflocculatorview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../widgets/alumclariflocculatorview.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alumclariflocculatorview.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_AlumClariflocculatorView_t {
    uint offsetsAndSizes[20];
    char stringdata0[25];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[13];
    char stringdata5[16];
    char stringdata6[15];
    char stringdata7[15];
    char stringdata8[15];
    char stringdata9[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AlumClariflocculatorView_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AlumClariflocculatorView_t qt_meta_stringdata_AlumClariflocculatorView = {
    {
        QT_MOC_LITERAL(0, 24),  // "AlumClariflocculatorView"
        QT_MOC_LITERAL(25, 13),  // "animateArrows"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 15),  // "animateRotation"
        QT_MOC_LITERAL(56, 12),  // "animateWater"
        QT_MOC_LITERAL(69, 15),  // "animateFlowDots"
        QT_MOC_LITERAL(85, 14),  // "onPump1Started"
        QT_MOC_LITERAL(100, 14),  // "onPump1Stopped"
        QT_MOC_LITERAL(115, 14),  // "onPump2Started"
        QT_MOC_LITERAL(130, 14)   // "onPump2Stopped"
    },
    "AlumClariflocculatorView",
    "animateArrows",
    "",
    "animateRotation",
    "animateWater",
    "animateFlowDots",
    "onPump1Started",
    "onPump1Stopped",
    "onPump2Started",
    "onPump2Stopped"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AlumClariflocculatorView[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    0,   64,    2, 0x08,    3 /* Private */,
       5,    0,   65,    2, 0x08,    4 /* Private */,
       6,    0,   66,    2, 0x08,    5 /* Private */,
       7,    0,   67,    2, 0x08,    6 /* Private */,
       8,    0,   68,    2, 0x08,    7 /* Private */,
       9,    0,   69,    2, 0x08,    8 /* Private */,

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

Q_CONSTINIT const QMetaObject AlumClariflocculatorView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_AlumClariflocculatorView.offsetsAndSizes,
    qt_meta_data_AlumClariflocculatorView,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AlumClariflocculatorView_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AlumClariflocculatorView, std::true_type>,
        // method 'animateArrows'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'animateRotation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'animateWater'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'animateFlowDots'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPump1Started'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPump1Stopped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPump2Started'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPump2Stopped'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AlumClariflocculatorView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AlumClariflocculatorView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->animateArrows(); break;
        case 1: _t->animateRotation(); break;
        case 2: _t->animateWater(); break;
        case 3: _t->animateFlowDots(); break;
        case 4: _t->onPump1Started(); break;
        case 5: _t->onPump1Stopped(); break;
        case 6: _t->onPump2Started(); break;
        case 7: _t->onPump2Stopped(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *AlumClariflocculatorView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlumClariflocculatorView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AlumClariflocculatorView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int AlumClariflocculatorView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
