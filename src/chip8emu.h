#ifndef CHIP8EMU_H
#define CHIP8EMU_H

#include <QObject>
#include <QPair>
#include <QVector>

QT_BEGIN_NAMESPACE
class QByteArray;
class QStringList;
QT_END_NAMESPACE


#define RAM_SIZE 4096
#define START_ADDR 0x200

class Chip8Emu : public QObject
{
    Q_OBJECT
public:
    explicit Chip8Emu(QObject *parent = nullptr);

    enum WorkModeFlag {
        Emulation = 0x0000,
        Debuging = 0x0001,
        SuperMode = 0x0003,
        SuperModeDebug =  Debuging | SuperMode,
        UnDefined = 0x0020
    };
    Q_DECLARE_FLAGS(WorkMode, WorkModeFlag)

signals:
    void showDecodeOpCode(QString &text);
    void assemblerTextListing(QStringList &text);
    void finishExecute();

public slots:
    void loadData2Memory(QByteArray &data);
    void startEmulation();
    void stopEmulation();
    void startDisassembler();

private:
    void decreaseTimers();
    void executeNextOpcode();

    QVector <QPair <int,QString> > progText;
    QPair <int, QString> currentLine;

    unsigned short PC;   // mem offset counter
    QByteArray m_memory; // 4k ram memory

    bool m_stopped;
    short delay_timer;         // delay timer;
    short sound_timer;         // sound timer;
    int  opcode_count;
    int m_mode;
    bool m_enableListing;

};

#endif // CHIP8EMU_H
