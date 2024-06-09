/****************************************************************************
** Meta object code from reading C++ file 'AdminWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/AdminWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdminWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
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
struct qt_meta_stringdata_CLASSAdminWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSAdminWindowENDCLASS = QtMocHelpers::stringData(
    "AdminWindow",
    "addTutorial",
    "",
    "addTutorialToTable",
    "Tutorial",
    "tutorial",
    "updateTutorialInTable",
    "row",
    "deleteTutorial",
    "updateTutorial",
    "displayTutorial",
    "displayPlot",
    "undoUI",
    "redoUI",
    "undoShortCut",
    "redoShortCut"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAdminWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x0a,    1 /* Public */,
       3,    1,   81,    2, 0x10a,    2 /* Public | MethodIsConst  */,
       6,    2,   84,    2, 0x10a,    4 /* Public | MethodIsConst  */,
       8,    0,   89,    2, 0x0a,    7 /* Public */,
       9,    0,   90,    2, 0x0a,    8 /* Public */,
      10,    0,   91,    2, 0x0a,    9 /* Public */,
      11,    0,   92,    2, 0x0a,   10 /* Public */,
      12,    0,   93,    2, 0x0a,   11 /* Public */,
      13,    0,   94,    2, 0x0a,   12 /* Public */,
      14,    0,   95,    2, 0x0a,   13 /* Public */,
      15,    0,   96,    2, 0x0a,   14 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    7,    5,
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

Q_CONSTINIT const QMetaObject AdminWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSAdminWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAdminWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAdminWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AdminWindow, std::true_type>,
        // method 'addTutorial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addTutorialToTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Tutorial &, std::false_type>,
        // method 'updateTutorialInTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Tutorial &, std::false_type>,
        // method 'deleteTutorial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateTutorial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayTutorial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayPlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'undoUI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'redoUI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'undoShortCut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'redoShortCut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AdminWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AdminWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addTutorial(); break;
        case 1: _t->addTutorialToTable((*reinterpret_cast< std::add_pointer_t<Tutorial>>(_a[1]))); break;
        case 2: _t->updateTutorialInTable((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Tutorial>>(_a[2]))); break;
        case 3: _t->deleteTutorial(); break;
        case 4: _t->updateTutorial(); break;
        case 5: _t->displayTutorial(); break;
        case 6: _t->displayPlot(); break;
        case 7: _t->undoUI(); break;
        case 8: _t->redoUI(); break;
        case 9: _t->undoShortCut(); break;
        case 10: _t->redoShortCut(); break;
        default: ;
        }
    }
}

const QMetaObject *AdminWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AdminWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAdminWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AdminWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
