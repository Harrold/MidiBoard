#include "midithread.h"

#include <QMediaPlayer>
#include <ostream>

MidiThread::MidiThread(libremidi::midi_in *midiin, QObject *parent) : QObject(parent), midiin(midiin)
{

}

MidiThread::~MidiThread()
{
    //delete midiin;
}

std::ostream& operator<<(std::ostream& os, const libremidi::meta_event_type Token)
{
  switch(Token)
  {
    case libremidi::meta_event_type::CHANNEL_PREFIX:
      os << "CHANNEL_PREFIX";
    break;
    case libremidi::meta_event_type::COPYRIGHT:
      os << "copyright";
    break;
  case libremidi::meta_event_type::SEQUENCE_NUMBER:
    os << "out";
  break;
  case libremidi::meta_event_type::TEXT:
    os << "text";
  break;
  case libremidi::meta_event_type::TRACK_NAME:
    os << "trackname";
  break;
  case libremidi::meta_event_type::INSTRUMENT:
    os << "instrument";
  break;
  case libremidi::meta_event_type::PATCH_NAME:
    os << "patchname";
  break;
  case libremidi::meta_event_type::LYRIC:
    os << "lyric";
  break;
  case libremidi::meta_event_type::MARKER:
    os << "marer";
  break;
  case libremidi::meta_event_type::CUE:
    os << "cue";
  break;
  case libremidi::meta_event_type::DEVICE_NAME:
    os << "devicename";
  break;
  case libremidi::meta_event_type::MIDI_PORT:
    os << "midiport";
  break;
  case libremidi::meta_event_type::END_OF_TRACK:
    os << "endoftrack";
  break;
  case libremidi::meta_event_type::TEMPO_CHANGE:
    os << "tempochange";
  break;
  case libremidi::meta_event_type::SMPTE_OFFSET:
    os << "smpteoffset";
  break;
  case libremidi::meta_event_type::TIME_SIGNATURE:
    os << "timesignature";
  break;
  case libremidi::meta_event_type::KEY_SIGNATURE:
    os << "keysignature";
  break;
  case libremidi::meta_event_type::PROPRIETARY:
    os << "proprietary";
  break;
  case libremidi::meta_event_type::UNKNOWN:
    os << "unknown";
  break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const libremidi::message_type Token)
{
  switch(Token)
  {
    case libremidi::message_type::ACTIVE_SENSING:
      os << "ACTIVE_SENSING";
    break;
    case libremidi::message_type::AFTERTOUCH:
      os << "AFTERTOUCH";
    break;
  case libremidi::message_type::CONTINUE:
    os << "CONTINUE";
  break;
  case libremidi::message_type::CONTROL_CHANGE:
    os << "CONTROL_CHANGE";
  break;
  case libremidi::message_type::EOX:
    os << "EOX";
  break;
  case libremidi::message_type::INVALID:
    os << "INVALID";
  break;
  case libremidi::message_type::NOTE_OFF:
    os << "NOTE_OFF";
  break;
  case libremidi::message_type::NOTE_ON:
    os << "NOTE_ON";
  break;
  case libremidi::message_type::PITCH_BEND:
    os << "PITCH_BEND";
  break;
  case libremidi::message_type::POLY_PRESSURE:
    os << "POLY_PRESSURE";
  break;
  case libremidi::message_type::PROGRAM_CHANGE:
    os << "PROGRAM_CHANGE";
  break;
  case libremidi::message_type::RESERVED1:
    os << "RESERVED1";
  break;
  case libremidi::message_type::RESERVED2:
    os << "RESERVED2";
  break;
  case libremidi::message_type::RESERVED3:
    os << "RESERVED3";
  break;
  case libremidi::message_type::RESERVED4:
    os << "RESERVED4";
  break;
  case libremidi::message_type::SONG_POS_POINTER:
    os << "SONG_POS_POINTER";
  break;
  case libremidi::message_type::SONG_SELECT:
    os << "SONG_SELECT";
  break;
  case libremidi::message_type::START:
    os << "START";
  break;
  case libremidi::message_type::STOP:
    os << "STOP";
  break;
  case libremidi::message_type::SYSTEM_EXCLUSIVE:
    os << "SYSTEM_EXCLUSIVE";
  break;
  case libremidi::message_type::SYSTEM_RESET:
    os << "SYSTEM_RESET";
  break;
  case libremidi::message_type::TIME_CLOCK:
    os << "TIME_CLOCK";
  break;
  case libremidi::message_type::TIME_CODE:
    os << "TIME_CODE";
  break;
  case libremidi::message_type::TUNE_REQUEST:
    os << "TUNE_REQUEST";
  break;
  }
  return os;
}

void MidiThread::run()
{
    midiin->set_callback([this](const libremidi::message& message) {
        auto nBytes = message.size();
        auto status = message.get_message_type();

        auto met = message.get_meta_event_type();
       // std::cout << "Event: " << met;
      //  std::cout << "Status: " << status;

        if( status == libremidi::message_type::NOTE_ON)
        {
          //  std::cout << "Note On" << std::endl;
            /*for (auto i = 0U; i < nBytes; i++)
            {
                  std::cout << "Byte " << i << " = " << (int)message[i] << ", " << std::endl;
            }*/

            // we check if the numbers of message[0] and message[1] already exist in one of the sound list
            if( m_sounds.size() > 0 ) {
            foreach(Sound sound, m_sounds )
            {
                if( sound.Byte0() == (int)message[0] && sound.Byte1() == (int)message[1])
                {
                 //   std::cout << "Emitting sound" << std::endl;
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

        else if( status == libremidi::message_type::NOTE_OFF)
        {
           // std::cout << "Note OFF" << std::endl;
        }
        else if( status == libremidi::message_type::CONTROL_CHANGE)
        {
          /*  std::cout << "Control change" << std::endl;
            for (auto i = 0U; i < nBytes; i++)
                        {
                              std::cout << "Byte " << i << " = " << (int)message[i] << ", " << std::endl;
                        }*/
            if( (int)message[0] == 176 && (int)message[1] == 16 && (int)message[2] == 1)
            {
                // adjust the volume louder
                emit emitUpVolume(1);
            }
            else if( (int)message[0] == 176 && (int)message[1] == 16 && (int)message[2] == 127)
            {
                // adjust the volume quieter
                emit emitLowerVolume(-1);
            }
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
