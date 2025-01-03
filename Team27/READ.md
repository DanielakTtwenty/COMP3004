TEAM 27 

MEMBERS
    Daniel Akinyemi     
    James Ebbah-Idala   
    Chiedu Ezinwa       

BREAKDOWN OF RESPONSIBILITIES
    The entirety was done as a whole (ideas and execution). "denas-prototype" was a reference for our project
    
    Implementations & Code 
        Daniel Akinyemi - Button functionality and metrics computations (values and Graphs)
        Chiedu Ezinwa - Cohenrence level light display, power button functionality, Database for history 
        James Ebbah-Idala - Button functionality, Graphs display, charge and Button functionality


Files Uploaded
    STATEDIAGRAM.pdf
    UseCases.pdf
    TraceabilityMatrix.pdf
    UML.pdf
    
    Sequences diagram
        BreathPacerSetting.pdf
        challengeSettings.pdf
        History.pdf
        PowerOff.pdf
        powerOn.pdf
        StartSession.pdf
        Service.pdf

    Team 27 
        Coherence_InputData
        history.txt
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        menu.cpp
        menu.h
        qcustomplot.cpp
        qcustomplot.h
        Team27.pro
        Team27.pro.user

    Tests Shown in Video 
        Run a session
        Show 10 breath pacer is on at power
        Show you can view session and delete all sessions
        Show you can change breath pacer levels
        Account for extreme coherence cases


Functionality 
    The program reads an input file line by line to compute a coherence score
    Then checks wha level the score belongs to and send an out put as a coloured widget 
    All the values are printed to the screen, as well as a graph that is updated every 5 seconds

    All the button, frames and widgets are pre made using the QT frame work
    When the program runs battery level is reducing until the user charges the batter (using the bettery button)

    A widget chnaged colour depending on the coherence score

    When the session start the user has an option to end the session which saves all the data to History 
    There is an option to delete history. This DELETES all the saved data from old sessions


    The level breath pacer level selected determines the number of coloured widgets below the screen

    FOLLOW THESE PATHS FOR FILES:
        for history: /home/student/Desktop/Team27/history.txt 
        for coherence: /home/student/Desktop/Team27/Cohenrence_InputData

