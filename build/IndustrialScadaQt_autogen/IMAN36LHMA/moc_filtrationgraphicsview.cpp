/****************************************************************************
** Meta object code from reading C++ file 'filtrationgraphicsview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../widgets/filtrationgraphicsview.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filtrationgraphicsview.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_FiltrationGraphicsView_t {
    uint offsetsAndSizes[10];
    char stringdata0[23];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_FiltrationGraphicsView_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_FiltrationGraphicsView_t qt_meta_stringdata_FiltrationGraphicsView = {
    {
        QT_MOC_LITERAL(0, 22),  // "FiltrationGraphicsView"
        QT_MOC_LITERAL(23, 13),  // "setAlarmState"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 7),  // "alarmOn"
        QT_MOC_LITERAL(46, 15)   // "resetPlantState"
    },
    "FiltrationGraphicsView",
    "setAlarmState",
    "",
    "alarmOn",
    "resetPlantState"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_FiltrationGraphicsView[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x0a,    1 /* Public */,
       4,    0,   29,    2, 0x0a,    3 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject FiltrationGraphicsView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_FiltrationGraphicsView.offsetsAndSizes,
    qt_meta_data_FiltrationGraphicsView,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_FiltrationGraphicsView_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FiltrationGraphicsView, std::true_type>,
        // method 'setAlarmState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'resetPlantState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void FiltrationGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FiltrationGraphicsView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setAlarmState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->resetPlantState(); break;
        default: ;
        }
    }
}

const QMetaObject *FiltrationGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FiltrationGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FiltrationGraphicsView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int FiltrationGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
