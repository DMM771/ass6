#include "CLI.h"
//324680438
//313306367
// Nitzan Fisher & David Monheit

CLI::CLI(DefaultIO *inputOutput)
{
    defaultInputOutput = inputOutput;
    cliCmds.push_back(new UploadCmd(defaultInputOutput));
    cliCmds.push_back(new SettingsCmd(defaultInputOutput));
    cliCmds.push_back(new DetectCmd(defaultInputOutput));
    cliCmds.push_back(new DisplayCmd(defaultInputOutput));
    cliCmds.push_back(new Analyze(defaultInputOutput));
    cliCmds.push_back(new ExitCmd(defaultInputOutput));
}

//activate the menu and execute every command
void CLI::start()
{
    int max = 5;
    State state;
    int currentOption = 12;
    char option = 'c';
    char zero = '0';
    while (currentOption != max)
    {
        defaultInputOutput->write("Welcome to the Anomaly Detection Server.\n");
        defaultInputOutput->write("Please choose an option:\n");
        string numStr = "1.";
        for (int i = 0; i < cliCmds.size(); i++) {
            numStr = "1.";
            option = i + 1;
            numStr[0] = (option + '0');
            defaultInputOutput->write(numStr);
            defaultInputOutput->write(cliCmds[i]->desc + "\n");
        }
        string input = defaultInputOutput->read();
        currentOption = input[0] - zero;
        currentOption -= 1;
        if(currentOption >= 0 && currentOption <= 6)
            cliCmds[currentOption]->execute(&state);
    }
}

CLI::~CLI()
{
    for (int i = 0; i < cliCmds.size(); i++)
    {
        delete cliCmds[i];
    }
}