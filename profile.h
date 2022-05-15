#ifndef PROFILE_H
#define PROFILE_H

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

#include <QVector>
#include <QDataStream>
#include "sound.h"


class Profile
{
public:
    Profile();
    Profile(QString profileName);

    const QString ProfileName() const { return m_profileName; }
    void ProfileName(const QString &name) { this->m_profileName = name; }
    void addSound(Sound sound);
    void addSoundList(QVector<Sound> sounds) { m_sounds = sounds; }
    const QVector<Sound> getSounds() const { return m_sounds; }
    void deleteSound(Sound sound) {} // unused for now
    void modifySound(Sound sound) {} // unused for now

   // friend std::ostream& operator << (std::ostream &os, const Profile &p);
    friend QDataStream &operator<<(QDataStream &stream, const Profile &profile)
    {
        stream << profile.m_profileName;
        stream << profile.m_sounds;
        /*for( const auto &sound : profile.m_sounds )
        {
            stream << sound;
        }*/
        return stream;
    }
    friend QDataStream &operator>>(QDataStream &stream, Profile &profile)
    {
        stream >> profile.m_profileName;
        stream >> profile.m_sounds;
        return stream;
    }

    friend bool operator==( const Profile &lhs, const Profile &rhs )
    {
        return lhs.ProfileName() == rhs.ProfileName() && lhs.getSounds() == rhs.getSounds();
    }

private:
    QString m_profileName;
    QVector<Sound> m_sounds;
};

Q_DECLARE_METATYPE(Profile)



#endif // PROFILE_H
