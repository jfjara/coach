#ifndef RESULTADOARBITRO_H
#define RESULTADOARBITRO_H

#include <QTime>
#include "referee.h"

class ResultadoArbitro
{
public:
    ResultadoArbitro();


    Referee* arbitro;
    QList<QTime> lista6x40;
    QTime promedio = QTime(0,0);
    QList<QTime> resultado2000;
    QTime resultadoPC;

    double bonificacion6x40;
    double bonificacion2000;
    double bonificacionPC;
    double bonificacion5x40;

    double getPromedio6x40();
    double getBonificacionTotal() const;
    double getTotal();
    double getResultado2000();


};

#endif // RESULTADOARBITRO_H
