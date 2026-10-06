/****************************************************************************
** Meta object code from reading C++ file 'RenderWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RenderWidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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
struct qt_meta_tag_ZN12RenderWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto RenderWidget::qt_create_metaobjectdata<qt_meta_tag_ZN12RenderWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RenderWidget",
        "BeginScaledDrag",
        "",
        "whichButton",
        "x",
        "y",
        "ContinueScaledDrag",
        "EndScaledDrag"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'BeginScaledDrag'
        QtMocHelpers::SignalData<void(int, float, float)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Float, 4 }, { QMetaType::Float, 5 },
        }}),
        // Signal 'ContinueScaledDrag'
        QtMocHelpers::SignalData<void(float, float)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 4 }, { QMetaType::Float, 5 },
        }}),
        // Signal 'EndScaledDrag'
        QtMocHelpers::SignalData<void(float, float)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 4 }, { QMetaType::Float, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RenderWidget, qt_meta_tag_ZN12RenderWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RenderWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QOpenGLWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12RenderWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12RenderWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12RenderWidgetE_t>.metaTypes,
    nullptr
} };

void RenderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RenderWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->BeginScaledDrag((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[3]))); break;
        case 1: _t->ContinueScaledDrag((*reinterpret_cast<std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        case 2: _t->EndScaledDrag((*reinterpret_cast<std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RenderWidget::*)(int , float , float )>(_a, &RenderWidget::BeginScaledDrag, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RenderWidget::*)(float , float )>(_a, &RenderWidget::ContinueScaledDrag, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (RenderWidget::*)(float , float )>(_a, &RenderWidget::EndScaledDrag, 2))
            return;
    }
}

const QMetaObject *RenderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12RenderWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int RenderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void RenderWidget::BeginScaledDrag(int _t1, float _t2, float _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void RenderWidget::ContinueScaledDrag(float _t1, float _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void RenderWidget::EndScaledDrag(float _t1, float _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}
QT_WARNING_POP
