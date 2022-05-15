#ifndef MIDITHREAD_H
#define MIDITHREAD_H

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

#include <QThread>
#include <libremidi/libremidi.hpp>
#include "sound.h"
#include <QVector>

using namespace std::literals;

class MidiThread : public QObject
{
    Q_OBJECT
public:
    MidiThread(libremidi::midi_in *midiin, QObject *parent = nullptr);
    virtual ~MidiThread();

    void callbackListener();
    void run();

    void setSounds(QVector<Sound> sounds) { m_sounds = sounds; }

signals:
    void emitMidiCall(const QString &value);
    void emitPlaySound(const Sound sound);
    void finished();

protected:


private:
    libremidi::midi_in *midiin;
    QVector<Sound> m_sounds;
};

#endif // MIDITHREAD_H
