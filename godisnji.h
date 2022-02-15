#ifndef GODISNJI_H
#define GODISNJI_H

#include "Godisnji_global.h"
#include <QDate>

class GODISNJI_EXPORT Godisnji
{
private:
    QDate pocetak;
    QDate kraj;

public:
    Godisnji() = default;
    ~Godisnji() = default;

    Godisnji(QDate _pocetak, QDate _kraj);;

    int izracunDana();;

    bool prekoracenje(int _remaining);


};

#endif // GODISNJI_H
