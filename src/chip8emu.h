#ifndef DECODER_H
#define DECODER_H

#include <QObject>

class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(QObject *parent = nullptr);

signals:

public slots:
    void toAsm (QByteArray &data);
};

#endif // DECODER_H
