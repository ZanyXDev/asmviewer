#include <QDebug>
#include "decoder.h"

Decoder::Decoder(QObject *parent) : QObject(parent)
{

}

void Decoder::toAsm(QByteArray &data)
{
    qDebug() << "load: " << data.size() << " bytes";
}
