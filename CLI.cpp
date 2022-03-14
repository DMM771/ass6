#include "CLI.h"

CLI::CLI(DefaultIO *dio1)
{
    dio = dio1;
    this->cli = new cliSet();
    commands[0] = new Upload(dio);
    commands[1] = new Settings(dio);
    commands[2] = new Detect(dio);
    commands[3] = new Display(dio);
    commands[4] = new Analyze(dio);
    commands[5] = new Exit(dio);
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
        for (int i = 0; i < 6; i++)
        {
            numStr = "1.";
            numStr[0] = ((char)(i + 1) + '0');
            dio->write(numStr);
            dio->write(commands[i]->desc+"\n");
        }
        string input = dio->read();
        in = input[0]-'0'-1;
        if(in >= 0 && in <= 6)
            commands[in]->execute(&s);
    }
}

CLI::~CLI()
{
    for (int i = 0; i < 6; i++)
    {
        delete commands[i];
    }
    delete commands;
}