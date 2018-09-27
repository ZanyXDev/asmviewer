#include <QDebug>
#include <QElapsedTimer>
#include <QCoreApplication>

#include "chip8emu.h"

Chip8Emu::Chip8Emu(QObject *parent) : QObject(parent)
{    
    m_stopped = false;
    m_mode = Chip8Emu::Emulation;

    PC = START_ADDR;               // set mem offset counter

    delay_timer = 0;               // clear delay timer;
    sound_timer = 0;               // clear sound timer;
    opcode_count = 0 ;
    m_memory.fill(0x0,RAM_SIZE);   // clear 4k ram memory
}

void Chip8Emu::loadData2Memory(QByteArray &data)
{
    qDebug() << "load: " << data.size() << " bytes";
    if ( !data.isEmpty() &&
         ( data.size() <= RAM_SIZE - START_ADDR)  ) {
        m_memory.insert(START_ADDR,data);
    }
}


void Chip8Emu::startEmulation()
{

    QElapsedTimer et;
    et.start();

    opcode_count = 0;
    int cycles_per_second;

    while (!m_stopped)
    {
        QCoreApplication::processEvents ( QEventLoop::AllEvents );

        if (m_mode == Chip8Emu::SuperMode)
        {
            cycles_per_second = 10; // execute 600 opcodes per second
        }
        else
        {
            cycles_per_second = 30; // 1800 opcodes per second
        }
        if (opcode_count < cycles_per_second)
        {
            executeNextOpcode();
            opcode_count++;
        }

        //decrease timers every 1/60sec and redraw screen
        if (et.hasExpired(1000/60))
        {
            decreaseTimers();
            et.restart();
            //emit updateScreen(m_screen);
            opcode_count = 0;
        }
        //if (emu->stop == true) closeRom();
    }

}

void Chip8Emu::decreaseTimers()
{
    if (delay_timer > 0)
    {
        --delay_timer;
    }
}

QString Chip8Emu::executeNextOpcode()
{
    unsigned short opCode;
    unsigned char N;    // 4 bit constants
    QString asmTextString;

    opCode  = ( m_memory.at(PC) << 8) | m_memory.at(PC+1);
    switch  ( (opCode & 0xF000) >> 12)
    { // high
     case 0x0:
        /**
         * 0nnn SYS nnn Перейти на машинный код RCA 1802 по адресу nnn.
         * Эта инструкция была только в самой первой реализации CHIP-8.
         * В более поздних реализациях и эмуляторах не используется.
         * 00Cn SCD n Прокрутить изображение на экране на n строк вниз
         * 00FB SCR   Прокрутить изображение на экране на 4 пикселя вправо в режиме 128x64,
         *            либо на 2 пикселя в режиме 64x32
         * 00FC SCL   Прокрутить изображение на экране на 4 пикселя влево в режиме 128x64,
         *            либо на 2 пикселя в режиме 64x32
         * 00FD EXIT  Завершить программу
         * 00FF HIGH  Включить расширенный режим экрана. Переход на разрешение 128x64
         * 00FE LOW   Выключить расширенный режим экрана. Переход на разрешение 64x32
         * 00E0 CLS   Очистить экран
         * 00EE RET   Возвратиться из подпрограммы
         */

        break;
    default:
        qDebug() << "opCode: " << opCode << " invalid.";
        break;
    }

    asmTextString.append(QString().setNum(opCode) )
    ;

    PC+=2;
    return asmTextString;

}

void Chip8Emu::stopEmulation()
{
    m_stopped = true;
}

void Chip8Emu::startDisassembler()
{
    QStringList text;
    while (PC < m_memory.size() - 2){
        qDebug() << "PC:" << PC ;
        text.append( executeNextOpcode() );
    }
    if ( !text.isEmpty() ){
        emit assemblerTextListing (text);
    }
}
