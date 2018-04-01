#include "resultadoarbitro.h"


ResultadoArbitro::ResultadoArbitro()
{

}

double ResultadoArbitro::getPromedio6x40()
{
    return 0.0;
}

double ResultadoArbitro::getResultado2000()
{
    if (resultado2000.size() <= 0) {
        return 0.0;
    }
    double secs = 0.0;
    QTime r = resultado2000.last();
   //for (QTime r : resultado2000) {
        secs += (double)r.minute() * 60.0;
        secs += r.second();
    //}
    int minutos = secs / 60.0;
    double segundos = (secs - (minutos * 60.0)) / 100.0;
    return (double)r.minute() + ((double)r.second()/100.0);
    //return (double)minutos + segundos;
}

double ResultadoArbitro::getBonificacionTotal() const
{        
    return bonificacion6x40 + bonificacion2000 + bonificacionPC;
}

double ResultadoArbitro::getTotal()
{
    return 0.0;
}
