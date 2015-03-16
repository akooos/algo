#ifndef DEFS
#define DEFS

#if (__cplusplus < 201103L)
//class constant
#define constant static const
#else
#define constant const
#endif

#ifdef QT_CORE_LIB

#include <QApplication>

#ifdef QT_NO_DEBUG
//Hosszabb távon idegesítő debuggolás során ha mindig bejön a login felület.
//Ne látszódjanak qDebug() üzenetek.
#define QT_NO_DEBUG_OUTPUT
#define ITT(s)
#else
    #include <QDebug>
    #define ITT(s) qDebug() << QString(__FILE__) + ":" + QString::number(__LINE__) << s;
#endif
#endif //QT_CORE_LIB


/* Stroustrup */
template<class T, class B> struct Derived_from {
    static void constraints(T* p) { B* pb = p; }
    Derived_from() { void(*p)(T*) = constraints; }
};

template<class T1, class T2> struct Can_copy {
    static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
    Can_copy() { void(*p)(T1,T2) = constraints; }
};

template<class T1, class T2 = T1> struct Can_compare {
    static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
    Can_compare() { void(*p)(T1,T2) = constraints; }
};

template<class T1, class T2, class T3 = T1> struct Can_multiply {
    static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
    Can_multiply() { void(*p)(T1,T2,T3) = constraints; }
};

#endif // DEFS

