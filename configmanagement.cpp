#include "configmanagement.h"

ConfigManagement::ConfigManagement()
{

}

void ConfigManagement::saveReferees(QList<Referee*> referees)
{
    QFile file("/config/bd.dat");
    QTextStream stream(&file);
    for (int i = 0; i < referees.size(); i++) {
        stream << referees.at(i)->dni << ",";
        stream << referees.at(i)->name << ",";
        stream << referees.at(i)->age << "\n";
    }
    file.close();
}

QList<Referee*> ConfigManagement::loadReferees()
{
    QList<Referee*> result;

    QFile file("/config/bd.dat");
    if(!file.open(QIODevice::ReadOnly)) {
        return result;
        //QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        Referee* referee = new Referee();
        referee->dni = fields.at(0);
        referee->name = fields.at(1);
        referee->age = fields.at(2).toInt();
        result.push_back(referee);
    }
    file.close();
    return result;
}
