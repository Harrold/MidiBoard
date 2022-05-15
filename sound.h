#ifndef SOUND_H
#define SOUND_H

/*MIT License

Copyright (c) 2022 Sebastian Schneider

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMetaType>

class Sound
{
public:
    Sound();
    Sound(QString soundName, QString fileName, int byte0, int byte1);

    QString SoundName() const { return m_soundName; }
    void SoundName(QString soundName) { m_soundName = soundName; }
    QString FileName() const { return m_fileName; }
    void FileName(QString fileName) { m_fileName = fileName; }
    QString Bytes() const { return m_bytes; }

    void Bytes(QString bytes)
    {
        m_bytes = bytes;
        m_byte0 = m_bytes.split(":").at(0).toInt();
        m_byte1 = m_bytes.split(":").at(1).toInt();
    }


    int Byte0() const { return m_byte0; }
    int Byte1() const { return m_byte1; }

    bool operator==(const Sound &other) const
        {
            return m_fileName == other.m_fileName && m_soundName == other.m_soundName
                    && m_byte0 == other.m_byte0 && m_byte1 == other.m_byte1;
        }

    friend QDataStream& operator << (QDataStream &os, const Sound &p);
    friend QDataStream& operator >> (QDataStream &os, Sound &p);

    QString m_soundName;
    QString m_fileName;
    QString m_bytes;
    int m_byte0;
    int m_byte1;
private:


};

Q_DECLARE_METATYPE(Sound)



#endif // SOUND_H
