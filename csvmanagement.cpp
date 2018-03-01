#include "csvmanagement.h"

CSVManagement::CSVManagement()
{

}

void CSVManagement::createReport(QList<Referee*> referees)
{
    QString filename="c:\\aaa\\report.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << " ,";
        int totalLaps = referees.at(0)->laps.size();
        for (int i = 0; i < totalLaps; i++) {
            stream << "Lap " << QString::number(i + 1) << ", ";
        }
        stream << "\n";

        for (int i = 0; i < referees.size(); i++) {
            Referee* referee = referees.at(i);
            stream << QString::number(i) << ", ";
            for (int j = 0; j < referee->laps.size(); j++) {
                stream << referee->laps.at(j)->timeEnd.toString().toLatin1() << ", ";
            }
            stream << "\n";
        }
    }
    file.close();
}
