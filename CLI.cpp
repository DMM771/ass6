#include "CLI.h"

CLI::CLI(DefaultIO *dio1)
{
    dio = dio1;
    this->cli = new cliSet();
    cliCmds.push_back(new Upload(dio));
    cliCmds.push_back(new Settings(dio));
    cliCmds.push_back(new Detect(dio));
    cliCmds.push_back(new Display(dio));
    cliCmds.push_back(new Analyze(dio));
    cliCmds.push_back(new Exit(dio));
}

//activate the menu and execute every command
void CLI::start()
{
    State s;
    int in = -1;
    while (in != 5)
    {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        string numStr = "1.";
        for (int i = 0; i < cliCmds.size(); i++)
        {
            numStr = "1.";
            numStr[0] = ((char)(i + 1) + '0');
            dio->write(numStr);
            dio->write(cliCmds[i]->desc + "\n");
        }
        string input = dio->read();
        in = input[0]-'0'-1;
        if(in >= 0 && in <= 6)
            cliCmds[in]->execute(&s);
    }
}

CLI::~CLI()
{
    for (int i = 0; i < cliCmds.size(); i++)
    {
        delete cliCmds[i];
    }
}