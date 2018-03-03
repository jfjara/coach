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

    return 2000.0 / secs;
}

double ResultadoArbitro::getBonificacionTotal() const
{        
    return bonificacion6x40 + bonificacion2000 + bonificacionPC;
}

double ResultadoArbitro::getTotal()
{
    return 0.0;
}
