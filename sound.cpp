#include "sound.h"
#include <QDataStream>

Sound::Sound()
{

}

Sound::Sound(QString soundName, QString fileName, int byte0, int byte1) :
    m_soundName(soundName), m_fileName(fileName), m_byte0(byte0), m_byte1(byte1)
{

    m_bytes.append(QString::number(m_byte0)).append(":").append(QString::number(m_byte1));

}

QDataStream &operator<<(QDataStream &stream, const Sound &sound)
{
   // stream.writeRawData(reinterpret_cast<const char*>(&sound), sizeof(sound));
    stream << sound.m_soundName;
    stream << sound.m_fileName;
    stream << sound.m_bytes;
    stream << sound.m_byte0;
    stream << sound.m_byte1;
    return stream;
}

QDataStream &operator >> (QDataStream &stream, Sound &sound)
{
  //  stream.readRawData(reinterpret_cast<char*>(&sound), sizeof(sound));
    stream >> sound.m_soundName;
    stream >> sound.m_fileName;
    stream >> sound.m_bytes;
    stream >> sound.m_byte0;
    stream >> sound.m_byte1;
    return stream;
}

/*std::ostream& operator << (std::ostream& os, const Sound &p) {
 // Output to screen
    os << p.m_soundName.toStdString() << "\n";
    os << p.m_fileName.toStdString() << "\n";
    os << p.m_bytes.toStdString() << "\n";
    os << p.m_byte1 << "\n";
    os << p.m_byte0 << "\n";
    return os;
}*/
