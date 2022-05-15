#include "profile.h"

Profile::Profile()
{

}

Profile::Profile(QString profileName) : m_profileName(profileName)
{

}

void Profile::addSound(Sound sound)
{
    m_sounds.push_back(sound);
}

/*QDataStream & operator << (QDataStream &stream, const Profile &profile)
{
    //return stream << sound.m_soundName << sound.m_fileName << sound.m_bytes << QString::number(sound.m_byte0) << QString::number(sound.m_byte1);
    //stream.writeRawData(reinterpret_cast<const char*>(&profile), sizeof(profile));

}*/

/*QDataStream & operator >> (QDataStream &stream, Profile &profile)
{
    //return stream << sound.m_soundName << sound.m_fileName << sound.m_bytes << QString::number(sound.m_byte0) << QString::number(sound.m_byte1);
    //stream >> profile.m_profileName;
    stream.readRawData(reinterpret_cast<char*>(&profile), sizeof(profile));

    return stream;
}*/

/*std::ostream& operator << (std::ostream& os, const Profile &p) {
 // Output to screen
    os << p.m_profileName.toStdString();
    for( const auto &sound : p.m_sounds )
    {
        os << sound;
    }
    return os;
}*/
