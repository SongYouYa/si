#pragma once

class CPContextHandler : public QObject
{
public:
    CPContextHandler(QObject* parent = nullptr);

    static bool initContext();
};
