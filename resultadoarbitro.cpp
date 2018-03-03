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
    double secs = 0.0;
    for (QTime r : resultado2000) {
        secs += r.second();
    }

    double minutos = secs / 60.0;
    double segundos = (secs - (minutos * 60.0)) / 100.0;

    return minutos + segundos;
}

double ResultadoArbitro::getBonificacionTotal() const
{        
    return bonificacion6x40 + bonificacion2000 + bonificacionPC;
}

double ResultadoArbitro::getTotal()
{
    return 0.0;
}
