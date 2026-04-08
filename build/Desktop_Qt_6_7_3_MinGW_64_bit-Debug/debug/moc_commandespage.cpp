/****************************************************************************
** Meta object code from reading C++ file 'commandespage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pages/commandespage.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandespage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSCommandesPageENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSCommandesPageENDCLASS = QtMocHelpers::stringData(
    "CommandesPage",
    "onAdd",
    "",
    "onUpdate",
    "onDelete",
    "onExportPdf",
    "onSelectionChanged",
    "onSearchChanged",
    "onSortChanged",
    "onComputeTotal"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCommandesPageENDCLASS[] = {

 // content:
      12,       // revision
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

Q_CONSTINIT const QMetaObject CommandesPage::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseGestionPage::staticMetaObject>(),
    qt_meta_stringdata_CLASSCommandesPageENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCommandesPageENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCommandesPageENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CommandesPage, std::true_type>,
        // method 'onAdd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDelete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportPdf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSortChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onComputeTotal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void CommandesPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CommandesPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAdd(); break;
        case 1: _t->onUpdate(); break;
        case 2: _t->onDelete(); break;
        case 3: _t->onExportPdf(); break;
        case 4: _t->onSelectionChanged(); break;
        case 5: _t->onSearchChanged(); break;
        case 6: _t->onSortChanged(); break;
        case 7: _t->onComputeTotal(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *CommandesPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandesPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCommandesPageENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return BaseGestionPage::qt_metacast(_clname);
}

int CommandesPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseGestionPage::qt_metacall(_c, _id, _a);
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
