#include "tablemodel.h"

#include <iostream>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

TableModel::TableModel(const QVector<Sound> &sounds, QObject *parent)
    : QAbstractTableModel(parent), m_sounds(sounds)
{

}


int TableModel::rowCount(const QModelIndex &parent) const
{
   return parent.isValid() ? 0 : m_sounds.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_sounds.insert(position, { QString(), QString(), 0, 0 });

    endInsertRows();
    return true;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

        if (index.row() >= m_sounds.size() || index.row() < 0)
            return QVariant();

        if (role == Qt::DisplayRole) {
            const auto &sound = m_sounds.at(index.row());

            switch (index.column()) {
                case 0:
                    return sound.SoundName();
                case 1:
                    return sound.FileName();
                case 2:
                   // QString midiBytes = QString(QString::number(sound.Byte0())).append(":").append(QString::number(sound.Byte1()));
                   // std::cout << "Byttees: " << midiBytes.toStdString() << std::endl;
                   // return midiBytes;
                    return sound.Bytes();
            }
        }
        return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Name");
        } else if (section == 1) {
            return QString("File");
        } else if( section == 2 ) {
            return QString("Midi");
        }
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
            const int row = index.row();
            auto sound = m_sounds.value(row);

           // std::cout << &sound.m_fileName << std::endl;
           // std::cout << "Column: " << index.column() << std::endl;
            switch (index.column()) {

                case 0:
                    sound.SoundName(value.toString());
                    break;
                case 1:
                    sound.FileName(value.toString());
                    break;
                case 2:
                    sound.Bytes(value.toString());
                    break;
                default:
                    return false;
            }
            m_sounds.replace(row, sound);
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            return true;
        }

        return false;
}

void TableModel::addSound(const Sound sound)
{
    m_sounds.push_back(sound);
}

void TableModel::addSounds(const QVector<Sound> sounds)
{
    m_sounds = sounds;
}

const QVector<Sound> TableModel::getSounds() const
{
    return m_sounds;
}
