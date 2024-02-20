#include "BarCodeParser.h"

BarCodeParser::BarCodeParser()
{
    reset();
}


bool BarCodeParser::parse(QString code)
{
    qDebug() << code;
    if(code.startsWith("^#") && code.endsWith("#$"))
    {
        if(code.contains("{")) // It is a command
        {
            mType = Types::Commands;

            QString Command = code.sliced(3, 4);
            QString Param = code.sliced(8, 6);

            bool success = false;

            mCommand.second = Param.toInt(&success);

            if(Command == "0001")
                mCommand.first = Command::Add;
            else if(Command == "0002")
                mCommand.first = Command::Remove;
            else
                success = false;

            if(!success)
                reset();
        }

        else if(code.contains("[")) // Can be a group or a unitary
        {
            if(code.contains('|')) // It is a unitary item
            {
                mType = Types::Unitary;
                QString Item = code.sliced(3, 11);
                QString Unit = code.sliced(15, 11);

                bool success = false;
                mItem.id = Item.toInt(&success);
                mItem.unit = Unit.toInt(&success);

                if(!success)
                    reset();
            }

            else // It is a group item
            {
                mType = Types::Grouped;
                QString Item = code.sliced(3, 11);

                bool success = false;
                mItem.id = Item.toInt(&success);

                if(!success)
                    reset();
            }
        }

        else
            reset();

    }
    else
        reset();

    return isValid();
}

bool BarCodeParser::isValid()
{
    return mType != Types::Invalid;
}

void BarCodeParser::reset()
{
    mType = Types::Invalid;
    mCommand = QPair<Command, int>(None, 0);
    mItem = ItemID();
}

BarCodeParser::Types BarCodeParser::type()
{
    return mType;
}


QString BarCodeParser::textCommand()
{
    QString text;

    if(hasPendingCommand())
    {
        switch(mCommand.first)
        {
            case Add:
                text += QString("Ajouter ");
                break;
            case Remove:
                text += QString("Enlever ");
                break;
            default:
                break;
        }

        if(mCommand.second == 999999)
            text += "tout ...";
        else
            text += QString::number(mCommand.second) + QString(" ...");
    }

    else
        return QString();

    return text;
}

bool BarCodeParser::hasPendingCommand()
{
    return mCommand.first != Command::None;
}

QPair<BarCodeParser::Command, int> BarCodeParser::command()
{
    QPair<BarCodeParser::Command, int> command = mCommand;
    mCommand = QPair<Command, int>(None, 0);

    return command;
}

ItemID BarCodeParser::item()
{
    return mItem;
}
