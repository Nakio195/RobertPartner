#ifndef BARCODEPARSER_H
#define BARCODEPARSER_H

#include "SEB/Item.h"
#include <QString>

class BarCodeParser
{
    public:
        enum Command {None = 0, Add = 1, Remove};
        enum Types{Commands, Unitary, Grouped, Invalid};

    public:
        BarCodeParser();

        bool parse(QString code);

        bool isValid();
        void reset();

        bool hasPendingCommand();

        Types type();
        QPair<Command, int> command();

        ItemID item();
        QString textCommand();

    private:

        Types mType;

        QPair<Command, int> mCommand;
        ItemID mItem;

};

#endif // BARCODEPARSER_H
