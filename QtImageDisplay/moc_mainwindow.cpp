/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Feb 26 11:20:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      38,   11,   11,   11, 0x08,
      70,   11,   11,   11, 0x08,
     110,  102,   11,   11, 0x08,
     143,   11,   11,   11, 0x08,
     179,   11,   11,   11, 0x08,
     205,   11,   11,   11, 0x08,
     234,   11,   11,   11, 0x08,
     264,  102,   11,   11, 0x08,
     304,   11,   11,   11, 0x08,
     333,   11,   11,   11, 0x08,
     363,   11,   11,   11, 0x08,
     389,   11,   11,   11, 0x08,
     419,   11,   11,   11, 0x08,
     449,   11,   11,   11, 0x08,
     484,   11,   11,   11, 0x08,
     523,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionOpen_triggered()\0"
    "on_actionWrite_Path_triggered()\0"
    "on_actionWrite_Mask_triggered()\0checked\0"
    "on_actionScissor_triggered(bool)\0"
    "on_actionReset_Contours_triggered()\0"
    "on_actionQuit_triggered()\0"
    "on_actionAuthors_triggered()\0"
    "on_actionAbout_Qt_triggered()\0"
    "on_actionOverlay_Active_triggered(bool)\0"
    "on_actionZoom_In_triggered()\0"
    "on_actionZoom_Out_triggered()\0"
    "on_actionHelp_triggered()\0"
    "on_actionBlur_3x3_triggered()\0"
    "on_actionBlur_5x5_triggered()\0"
    "on_actionGaussian_Blur_triggered()\0"
    "on_actionGaussian_Blur_5x5_triggered()\0"
    "on_actionFinish_Contour_triggered()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->on_actionWrite_Path_triggered(); break;
        case 2: _t->on_actionWrite_Mask_triggered(); break;
        case 3: _t->on_actionScissor_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_actionReset_Contours_triggered(); break;
        case 5: _t->on_actionQuit_triggered(); break;
        case 6: _t->on_actionAuthors_triggered(); break;
        case 7: _t->on_actionAbout_Qt_triggered(); break;
        case 8: _t->on_actionOverlay_Active_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_actionZoom_In_triggered(); break;
        case 10: _t->on_actionZoom_Out_triggered(); break;
        case 11: _t->on_actionHelp_triggered(); break;
        case 12: _t->on_actionBlur_3x3_triggered(); break;
        case 13: _t->on_actionBlur_5x5_triggered(); break;
        case 14: _t->on_actionGaussian_Blur_triggered(); break;
        case 15: _t->on_actionGaussian_Blur_5x5_triggered(); break;
        case 16: _t->on_actionFinish_Contour_triggered(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
