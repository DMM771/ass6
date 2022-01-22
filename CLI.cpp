#include "CLI.h"

CLI::CLI(DefaultIO *dio1)
{
    float p = 7;
    option = &p;
    dio = dio1;
    this->cli = new cliSet();
    commands[0] = new Upload(dio, cli);
    commands[1] = new Settings(dio, cli);
    commands[2] = new Detect(dio, cli);
    commands[3] = new Display(dio, cli);
    commands[4] = new Analyze(dio, cli);
    commands[5] = new Exit(dio, cli);
}

//activate the menu and execute every command
void CLI::start()
{
    while (*option != 6)
    {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < 6; i++)
        {
            string ss = commands[i]->getName();
            dio->write(ss);
        }
        dio->read(option);
        int x = *option - 1;
        commands[x]->execute();
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