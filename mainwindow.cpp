#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <libremidi/libremidi.hpp>
#include <thread>
#include <QEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMediaPlayer>

#include<iostream>
#include<fstream>

[[noreturn]] static void usage()
{
  // Error function in case of incorrect command-line
  // argument specifications.
  std::cout << "\nusage: qmidiin <port>\n";
  std::cout << "    where port = the device to use (default = 0).\n\n";
  exit(0);
}

using namespace std::literals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<Sound>();
    qRegisterMetaType<Profile>();

//    qRegisterMetaTypeStreamOperators<Sound>("Sound");
//    qRegisterMetaTypeStreamOperators<Profile>("Profile");

    tableModel = new TableModel(this);
    ui->tableView->setModel(tableModel);

    //libremidi::midi_in *libremidi = new libremidi::midi_in;
    umidi = std::make_shared<libremidi::midi_in>();
    auto ptr = umidi.get();//umidi = new libremidi::midi_in;
    //ptr = new libremidi::midi_in;

    if( chooseMidiPort(ptr))
    {
        std::cout << "Midi port successfully opened" << std::endl;
    }

    ptr->ignore_types(false, false, false);

    /*libremidi.set_callback([&](const libremidi::message& message) {
        auto nBytes = message.size();
        for (auto i = 0U; i < nBytes; i++)
          std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
        if (nBytes > 0)
          std::cout << "stamp = " << message.timestamp << std::endl;
      });

    libremidi.ignore_types(false, false, false);*/


   // addSound("c", "d", 0, 0);

   // tableModel->insertColumn(tableModel->columnCount()+1);
   // tableModel->addData(QVariantList() << "1111" << 111);

    midiThread = new MidiThread(umidi.get());
    midiThread->moveToThread(&workerThread);
    connect(&workerThread, &QThread::started, midiThread, &MidiThread::run);
    connect(midiThread, &MidiThread::finished, &workerThread, &QThread::quit);
    connect(midiThread, &MidiThread::finished, midiThread, &MidiThread::deleteLater);
    connect(&workerThread, &QThread::finished, midiThread, &MidiThread::deleteLater);



    ui->fileNameLine->installEventFilter(this);

    //connect(ui->fileNameLine, &QLineEdit::)

    connect(midiThread, &MidiThread::emitMidiCall, this, &MainWindow::bla);

    connect(midiThread, &MidiThread::emitPlaySound, this, &MainWindow::PlaySound);

    connect(ui->addButton, &QPushButton::pressed, this, &MainWindow::addSoundButton);

    connect(ui->addProfileButton, &QPushButton::pressed, this, &MainWindow::createNewProfile);

    connect(ui->saveProfile, &QPushButton::pressed, this, &MainWindow::saveProfiles);

    connect(ui->profileComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboBoxSelection);

    workerThread.start();




    /*
     * try {

     static std::atomic_bool done{};
       signal(SIGINT, [](int) { done = true; });

       // Periodically check input queue.
       std::cout << "Reading MIDI from port " << libremidi.get_port_name(0) << " ... quit with Ctrl-C.\n";
       while (!done)
       {
         auto msg = libremidi.get_message();
         auto nBytes = msg.size();

         for (auto i = 0U; i < nBytes; i++)
           std::cout << "Byte " << i << " = " << (int)msg[i] << ", ";

         if (nBytes > 0)
           std::cout << "stamp = " << msg.timestamp << std::endl;

         std::this_thread::sleep_for(10ms);
       }
    }




    catch (const libremidi::midi_exception& error)
    {
      std::cerr << error.what() << std::endl;
    }*/

    loadProfiles();
}

MainWindow::~MainWindow()
{

        workerThread.quit();

        if( !workerThread.wait(3000) ) {
            workerThread.terminate();
            workerThread.wait();
        }
        else {
            workerThread.terminate();
            workerThread.deleteLater();
        }
        delete tableModel;
       // delete midiThread;
        delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->fileNameLine && event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *k = static_cast<QMouseEvent *> (event);
            if( k->button() == Qt::LeftButton ) {
              std::cout << "Left click" << std::endl;

              QString filename =  QFileDialog::getOpenFileName(
                        this,
                        "Open Document",
                        QDir::currentPath(),
                        "All files (*.*) ;; Audio files (*.mp3 *.ogg)");

                  if( !filename.isNull() )
                  {
                    std::cout << "selected file path : " << filename.toStdString();
                    ui->fileNameLine->setText(filename);
                    return true;
                  }
            }
          }
          return false;
}

void MainWindow::addSoundButton()
{
    QString name = ui->nameLine->text();
    QString file = ui->fileNameLine->text();
    QString mbyte0 = ui->byte1Box->text();
    QString mbyte1 = ui->byte2Box->text();
    int byte0 = mbyte0.toInt();
    int byte1 = mbyte1.toInt();
    addSound(name, file, byte0, byte1);

  //  Sound sound(name, file, byte0, byte1);
   // addSound(sound);

    //midi.append(":");
    //midi.append(ui->byte2Box->text());
}

void MainWindow::bla(const QString &name)
{
    std::cout << "Name: " << name.toStdString() << std::endl;
    QString byte0 = name.split(":").at(0);
    QString byte1 = name.split(":").at(1);
    ui->byte1Box->setText(byte0);
    ui->byte2Box->setText(byte1);
}

void MainWindow::PlaySound(const Sound sound)
{
    std::cout << "Playing" << std::endl;
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(sound.FileName()));
    player->setVolume(30);
    std::cout << "Calling play" << std::endl;
    player->play();
}

bool MainWindow::chooseMidiPort(libremidi::midi_in *libremidi)
{
    std::string portName;
      unsigned int i = 0, nPorts = libremidi->get_port_count();
      if (nPorts == 0)
      {
        std::cout << "No input ports available!" << std::endl;
        return false;
      }

      if (nPorts == 1)
      {
        std::cout << "\nOpening " << libremidi->get_port_name() << std::endl;
      }
      else
      {
        for (i = 0; i < nPorts; i++)
        {
          portName = libremidi->get_port_name(i);
          std::cout << "  Input port #" << i << ": " << portName << '\n';
        }
      }

      libremidi->open_port(i);

      return true;
}

void MainWindow::addSound(Sound sound)
{
    tableModel->addSound(sound);
}

void MainWindow::choseFileName()
{

}

void MainWindow::addSound(const QString &name, const QString &fileName, int byte0, int byte1)
{
    if (!tableModel->getSounds().contains({ name, fileName, byte0, byte1 })) {
        std::cout << "Inserting" << std::endl;
        QString bytes = QString(QString::number(byte0)).append(":").append(QString::number(byte1));
            tableModel->insertRows(0, 1, QModelIndex());

            QModelIndex index = tableModel->index(0, 0, QModelIndex());
            tableModel->setData(index, name, Qt::EditRole);

            index = tableModel->index(0, 1, QModelIndex());
            tableModel->setData(index, fileName, Qt::EditRole);

            index = tableModel->index(0, 2, QModelIndex());
            tableModel->setData(index, bytes, Qt::EditRole);

            //tableModel->setData(index, )
        } else {
            QMessageBox::information(this, tr("Duplicate Name"),
                tr("The name \"%1\" already exists.").arg(name));
        }

    std::cout << tableModel->getSounds().size() << std::endl;
    midiThread->setSounds(tableModel->getSounds());

    for( auto &profiles : this->m_profileList)
    {
        if( profiles == currentProfile )
        {
            profiles.addSoundList(tableModel->getSounds());
            break;
        }
    }

    currentProfile.addSoundList(tableModel->getSounds());
    std::cout << "Successfully added a song" << std::endl;

}

void MainWindow::comboBoxSelection(int index)
{
    std::cout << "selected index " << index << std::endl;



    QVariant data = ui->profileComboBox->itemData(index);
    if( data.isValid() )
    {
        std::cout << "Data is valid" << std::endl;
        qDebug() << "VAR " << data;
    }
    tableModel->clearSounds();
    auto profile = qvariant_cast<Profile>(data);
    currentProfile = profile;
    tableModel->addSounds(currentProfile.getSounds());
    midiThread->setSounds(currentProfile.getSounds());
}

void MainWindow::createNewProfile()
{
   /* std::fstream wf("profile", std::ios::out | std::ios::binary);

    if( !wf.is_open() )
    {
        QMessageBox::information(this, tr("Cant save"), tr("Can't save the file"));
    }*/

    Profile profile(ui->profileNameEdit->text());
    profile.addSoundList(tableModel->getSounds());
    std::cout << "TableModel has " << tableModel->getSounds().size() << " sounds" << std::endl;

    m_profileList.push_back(profile);
    ui->profileComboBox->addItem(ui->profileNameEdit->text(), QVariant::fromValue(profile));
   // wf << profile;

}

void MainWindow::saveProfile(Profile profile)
{
   std::fstream wf("profiles", std::ios::out | std::ios::binary);

   if( !wf.is_open() )
   {
       QMessageBox::information(this, tr("Cant save"), tr("Can't save the file"));
   }

  // wf << profile;

}

void MainWindow::saveProfiles()
{
    //std::fstream wf("profiles", std::ios::out | std::ios::binary);

    QDomDocument doc;
    QDomElement root = doc.createElement("Profiles");
    doc.appendChild(root);

    for( const auto &profile : qAsConst(m_profileList))
    {
        // profile tag
        QDomElement profileElement = doc.createElement("Profile");
        root.appendChild(profileElement);
        // name tag
        QDomElement profileName = doc.createElement("Name");
        profileElement.appendChild(profileName);
        // profile name
        QDomText nameTextElement = doc.createTextNode(profile.ProfileName());
        profileName.appendChild(nameTextElement);

        // sounds tag
        QDomElement soundsTag = doc.createElement("Sounds");
        profileElement.appendChild(soundsTag);
        for( const auto &sounds : profile.getSounds())
        {
            // sound in the sounds tag
            QDomElement soundTag = doc.createElement("Sound");
            soundsTag.appendChild(soundTag);

            // sound name in the sound tag
            QDomElement soundName = doc.createElement("SoundName");
            soundTag.appendChild(soundName);
            QDomText sElement = doc.createTextNode(sounds.SoundName());
            soundName.appendChild(sElement);

            // filename
            QDomElement fileName = doc.createElement("FileName");
            soundTag.appendChild(fileName);
            QDomText fElement = doc.createTextNode(sounds.FileName());
            fileName.appendChild(fElement);

            // bytes
            QDomElement bytes = doc.createElement("Bytes");
            soundTag.appendChild(bytes);
            QDomText bytesElement = doc.createTextNode(sounds.Bytes());
            bytes.appendChild(bytesElement);

            // byte0
            QDomElement byte0 = doc.createElement("Byte0");
            soundTag.appendChild(byte0);
            QDomText byte0Element = doc.createTextNode(QString::number(sounds.Byte0()));
            byte0.appendChild(byte0Element);

            // byte1
            QDomElement byte1 = doc.createElement("Byte1");
            soundTag.appendChild(byte1);
            QDomText byte1Element = doc.createTextNode(QString::number(sounds.Byte1()));
            byte1.appendChild(byte1Element);
        }
    }


    QFile file("profile");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open the file for writing failed";
        }
        else
        {
            QTextStream stream(&file);
            stream << doc.toString();
            file.close();
            qDebug() << "Writing is done";
        }
    /*QFile wf("profiles");

    if( !wf.open(QIODevice::WriteOnly) )
    {
        QMessageBox::information(this, tr("Cant save"), tr("Can't save the file"));
    }

    QDataStream stream( &wf );

    for( const auto &profile : qAsConst(m_profileList) )
    {
        stream << profile;
    }

    wf.close();*/
}

void MainWindow::loadProfiles()
{
    QDomDocument document;

        // Open a file for reading
        QFile file("profile");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open the file for reading.";
            return;
        }
        else
        {
            // loading
            if(!document.setContent(&file))
            {
                qDebug() << "Failed to load the file for reading.";
                return;
            }
            file.close();
        }

        // Getting root element
            QDomElement root = document.firstChildElement();

            // retrievelements(QDomElement root, QString tag, QString att)
            retrievElements(root, "Profile");

            // get the Name
           // qDebug() << "Size: " << root.elementsByTagName("Name").at(0).toElement().text();

            //qDebug() << "\nGoing deeper level - getting the 'Room'";
            //QDomNodeList dorms = root.elementsByTagName("Sounds");

        file.close();
}

void MainWindow::retrievElements(QDomElement root, QString tag)
{
    QDomNodeList nodes = root.elementsByTagName(tag);

  //  qDebug() << "# nodes = " << nodes.count();
    for(int i = 0; i < nodes.count(); i++)
    {
        QDomNode elm = nodes.at(i);
        if(elm.isElement())
        {
            QDomElement e = elm.toElement();
           // qDebug() << "Tag1 " << e.tagName();
            Profile profile;
            profile.ProfileName(e.elementsByTagName("Name").at(0).toElement().text());
           // qDebug() << "Name: " << e.elementsByTagName("Name").at(0).toElement().text();
            QDomNodeList soundsList = e.elementsByTagName("Sound");
          //  qDebug() << "SoundsList: " << soundsList.count();
            for( int j = 0; j < soundsList.count(); j++ )
            {
                QDomNode nn = soundsList.at(j);
                if( nn.isElement() )
                {
                 //   qDebug() << "Sound Tag Child Nodes: " << nn.childNodes().size();
                    QDomNodeList soundChildren = nn.childNodes();
                    Sound sound;
                    for( int k = 0; k < soundChildren.size(); k++ )
                    {
                        QDomNode ss = soundChildren.at(k);
                        if( ss.isElement() )
                        {
                            QDomElement bla = ss.toElement();
                            if( bla.tagName() == "SoundName" )
                            {
                                sound.SoundName(bla.text());
                            }
                            else if( bla.tagName() == "FileName" )
                            {
                                sound.FileName(bla.text());
                            }
                            else if( bla.tagName() == "Bytes" )
                            {
                                sound.Bytes(bla.text());
                            }
                           // qDebug() << bla.tagName()  << " " << bla.text();
                        }
                    }
                    profile.addSound(sound);
                    QDomElement ee = nn.toElement();
               //     qDebug() << "Tag2 " << ee.tagName();
                }
               /* QDomNode nn = e.childNodes().at(j);
                if( nn.isElement() )
                {
                    QDomElement ee = nn.toElement();
                    qDebug() << "Tag2 " << ee.tagName();
                    if( ee.tagName() == "Name" )
                    {

                    }
                }*/
            }
            //qDebug() << " Text " << e.text();
            m_profileList.push_back(profile);
            ui->profileComboBox->addItem(profile.ProfileName(), QVariant::fromValue(profile));
        }
    }
    /*std::cout << "Profile List Size: " << m_profileList.size() << std::endl;
    for( const auto &profile : qAsConst(m_profileList) )
    {
        qDebug() << "Output lol";
        qDebug() << profile.ProfileName();
        for( const auto &sound : profile.getSounds() )
        {
            qDebug() << "SoundName: " << sound.SoundName();
        }
    }*/
    //std::cout << m_profileList.at(0).getSounds().at(0).SoundName();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

