#include "hellostring.h"
#include "qstring.h"
#include "QDebug"

HelloString::HelloString() {}


void HelloString::hellostring(){
    QString s1 = "first String";
    QString s2 = "first string";  // case sensitive

    qDebug() << "equals" << (s1 == s2);
    qDebug() << "equals" << (s1.compare(s2,Qt::CaseInsensitive) == 0);  // Hilfe, Qt::CaseInsensitive

    qDebug() << "size:" << s1.size();  // s1.length() ist gleich
    qDebug() << "lenght:" << s1.length();

    s1 += " extended";  // String konkatenieren

    // String Listen
    QStringList sList = s1.split(" ");
    for (const QString &s3 : sList) {
        qDebug () << "range-based-for" << s3;
    }
    // Alternativen: https://doc.qt.io/qt-6/containers.html#iterating-over-containers

    // Konvertierung QString zu C-String
    QString testString = "myString";
    qDebug() << (int)testString.toStdString().c_str()[7];  // gibt int-wert von "g" aus (=103), Hex 67
    qDebug() << testString.toLatin1()[7]; // mit ISO 8859-1 encoding
    qDebug() << testString.toUtf8()[7]; // mit UTF-8 encoding
    qDebug() << testString.toLocal8Bit()[7]; // mit UTF-8 encoding

    // Konvertierung C-String zu QString
    const char myCString[10] = "myCString";
    QString myQString(myCString);

    // Konvertierung int zu QString
    int testInt = 17;
    QString myMovie = QString::number(testInt).append("te Staffel");
    qDebug() << myMovie;

    // Konvertierung QString zu int
    QString testNumberInString = "4711";
    bool ok = false;
    int myNumber = testNumberInString.toInt(&ok);
    if (ok) {
        qDebug() << "parsing string was successful\n"\
            "myNumber:" << myNumber;
    }
}