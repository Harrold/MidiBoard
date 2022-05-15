#include "midithread.h"

#include <QMediaPlayer>

MidiThread::MidiThread(libremidi::midi_in *midiin, QObject *parent) : QObject(parent), midiin(midiin)
{

}

MidiThread::~MidiThread()
{
    //delete midiin;
}

void MidiThread::run()
{
    midiin->set_callback([this](const libremidi::message& message) {
        auto nBytes = message.size();
        auto status = message.get_message_type();
        if( status == libremidi::message_type::NOTE_ON)
        {
          //  std::cout << "Note On" << std::endl;
            for (auto i = 0U; i < nBytes; i++)
            {
                  std::cout << "Byte " << i << " = " << (int)message[i] << ", " << std::endl;
            }

            // we check if the numbers of message[0] and message[1] already exist in one of the sound list
            if( m_sounds.size() > 0 ) {
            foreach(Sound sound, m_sounds )
            {
                if( sound.Byte0() == (int)message[0] && sound.Byte1() == (int)message[1])
                {
                    std::cout << "Emitting sound" << std::endl;
                    emit emitPlaySound(sound);
                    // we already have a sound, let's just play it
                   /* QMediaPlayer player;
                    player.setMedia(QUrl::fromLocalFile(sound.FileName()));
                    player.setVolume(50);
                    player.play();*/
                }
                else
                {
                    QString bytes = QString::number((int)message[0]).append(":").append(QString::number((int)message[1]));
                    emit emitMidiCall(bytes);
                }
            }
            }
            else
            {
                std::cout << "Sounds are empty" << std::endl;
                QString bytes = QString::number((int)message[0]).append(":").append(QString::number((int)message[1]));
                emit emitMidiCall(bytes);
            }


        }

        if( status == libremidi::message_type::NOTE_OFF)
        {
            std::cout << "Note OFF" << std::endl;
        }

      });


  /*  try {

     static std::atomic_bool done{};
       signal(SIGINT, [](int) { done = true; });

       // Periodically check input queue.
       std::cout << "Reading MIDI from port " << midiin->get_port_name(0) << " ... quit with Ctrl-C.\n";
       while (!done)
       {
         auto msg = midiin->get_message();
         auto nBytes = msg.size();

         for (auto i = 0U; i < nBytes; i++)
           std::cout << "Byte " << i << " = " << (int)msg[i] << ", ";

         if (nBytes > 0)
           std::cout << "stamp = " << msg.timestamp << std::endl;

         emit emitMidiCall("FINISHED");

         std::this_thread::sleep_for(10ms);
       }
    }
    catch (const libremidi::midi_exception& error)
    {
      std::cerr << error.what() << std::endl;
    }*/
}
