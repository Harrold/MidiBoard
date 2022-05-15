#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QMainWindow>
#include <vector>
#include "tablemodel.h"
#include <QMessageBox>
#include <QComboBox>
#include "libremidi/libremidi.hpp"
#include "midithread.h"
#include "profile.h"

#include <QDomDocument>
#include <QDomElement>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void addSound(const QString &name, const QString &fileName, int byte0, int byte1);
    void addSound(Sound sound);
    void bla(const QString &name);
    void choseFileName();
    void addSoundButton();
    void createNewProfile();
    void comboBoxSelection(int index);
    void saveProfile(Profile profile);

private slots:
    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;

    void PlaySound(const Sound sound);

    bool chooseMidiPort(libremidi::midi_in *midiin);


    void saveProfiles();

    void loadProfiles();

    void retrievElements(QDomElement root, QString tag);

    TableModel *tableModel;
    MidiThread *midiThread;
    QThread workerThread;
    //libremidi::midi_in *midiin;
    std::shared_ptr<libremidi::midi_in> umidi;
    QVector<Profile> m_profileList; // a list of stored profiles, this is loaded upon program startup
    Profile currentProfile; // the last selected profile
};
#endif // MAINWINDOW_H
