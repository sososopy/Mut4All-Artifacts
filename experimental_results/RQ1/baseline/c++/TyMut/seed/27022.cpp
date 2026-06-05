
template<typename T>
struct strdata_t
{
    QByteArrayData data[5]{
        QT_MOC_LITERAL_CUSTOM(0, 0, 12), // "NotifyingMap"
        QT_MOC_LITERAL_CUSTOM(1, 13, 5), // "added"
        QT_MOC_LITERAL_CUSTOM(2, 19, 0), // ""
        QT_MOC_LITERAL_CUSTOM(3, 20, typeLength<T>()), // "T"
        QT_MOC_LITERAL_CUSTOM(4, 21 + typeLength<T>(), 7) // "removed"
    };
    std::array<char, 29 + typeLength<T>()> stringdata0 =
            concat("NotifyingMap\0added\0\0", T::className, "\0removed");
};



static const uint staticMetaData[31]{
 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

template<typename T>
constexpr const QMetaObject NotifyingMap<T>::staticMetaObject{
    { &QObject::staticMetaObject,
      strdata_t<T>{}.data,
      staticMetaData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR }
};
