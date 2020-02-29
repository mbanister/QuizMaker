#include <iostream>
#include <string>
#include <fstream>
# include <ctime>
# include <cstdlib>
#include <sstream>
using namespace std;
string get_player_name();
string** read_questions(string fileName);
void read_answers(string fileName, char[]);
bool quiz_check(int, int);
void show_question(string **question, int, int);
char player_try();
int play_game(int seed, int, string **question, char *answer, int, string);
void sort_score(string, int);
int main(int argc, char *argv[]) {
    //checks arguments
    if (argc != 4){
        cout << "Invalid Arguments";
        return 0;
    }
    int questCount, ansCount;
    int totalScore = 1;
    string playerName = get_player_name();
    string** question;
    question = read_questions(argv[1]);
    char answer [50];
    read_answers(argv[2], answer);
    //Gets number of questions and answers for use in functions
    ansCount = strlen(answer);
    questCount = sizeof(**question) / sizeof(*question);
    //Makes sure amount of questions equals amount of answers
    if (!quiz_check(questCount, ansCount)){
        return 0;
    }
    //converts seedstring to int for srand
    string seedString = argv[3];
    int seed;
    istringstream (seedString) >> seed;
    totalScore = play_game(seed, questCount, question, answer, totalScore, playerName);
    sort_score(playerName, totalScore);
    return 0;
}
string get_player_name(){
    string playername;
    //Gets player's name
    cout << "Enter name: ";
    cin >> playername;
    return playername;
}
string** read_questions(string fileName){
    string data;
    string** question = new string*[50];
    ifstream inFile;
    //Opens file once named file is found
    //Note: file name does not need .txt at end
    while(!inFile.is_open()){
        inFile.open(fileName);
        if (!inFile.is_open()){
            cout << "Error opening file" << endl;
        }
    }
    // Stores question in a 2d string array until end of file
    for (int i = 0; i < 50 && !inFile.eof(); i++) {
        question[i] = new string[5];
        for (int j = 0; j < 5; j++) {
            if (inFile.eof())
                continue;
            getline(inFile, data);
            if (!data.empty()){
               question[i][j] = data;
            } else {
                j--;
            }
        }
    }
    //closes file
    inFile.close();
    return question;
}
void read_answers(string fileName, char answer[50]) {
    char data;
    ifstream inFile;
    //Opens file once named file is found
    while(!inFile.is_open()){
        inFile.open(fileName);
        if (!inFile.is_open()){
            cout << "Error opening file" << endl;
        }
    }
    //stores answers in an array of characters
    for (int i = 0; i < 50; ++i) {
        answer[i] = '\0';
        if (inFile.eof()){
            continue;
        }
        //checks for end lines
        inFile.get(data);
        if (data == '\n'){
            inFile.get(data);
        }
        if (data != '\n'){
            answer[i] = data;
        }
    }
    inFile.close();
}
bool quiz_check ( int questcount, int anscount){
    if (questcount != anscount){
        cout << "Number of questions does not equal number of answers";
        return false;
    }
}
void show_question(string **question, int x, int y){
    char letter[6] = "XABCD";
    //Displays question
    cout << question[x][0] << endl;
    //Lists options preceded by letter
    for (int i = 1; i < 5; i++) {
        if (i != y) {
            cout << letter[i] << ". " << question[x][i] << endl;
        }
    }
}
char player_try(){
    char playerans;
    //Player enters answer
    cout << "Your choice? >";
    cin >> playerans;
    cout << endl;
    playerans = (char) toupper(playerans);
    return playerans;
}
int play_game(int seed, int questcount, string **question, char *answer, int totalscore,string playername){
    int curnum, questskip, streak = 10;
    int questnum[10], ansnum [10], playerscore [10];
    bool check;
    char playerans;
    string secondtry;
    srand(seed);
    //Randomizes question numbers without repeating
    for (int i=0;(i < 10) && (i < questcount); i++){
        do{
            curnum= rand() % (questcount);
            check=true;
            for (int j=0;j<i;j++)
                if (curnum == questnum[j]){
                    check=false;
                    break;
                }
        } while (!check);
        questnum[i]=curnum;
        ansnum[i] = curnum;
    }
    //Displays question
    for(int i = 0; (i < 10) && (i < questcount); i++){
        cout << playername << " Here's Question Number: " << i+1 << endl;
        show_question(question, questnum[i], questskip = 0);

        //un-comment for answers displayed in console after question
        //cout << answer[ansnum[i]] << endl;

        playerans = player_try();
        //Checks if player's answer is right and assigns score
        if (playerans == answer[ansnum[i]]){
            playerscore[i] = 3;
            cout << "Correct" << endl;
        }
        //If player is wrong, prompts for second try
        else if (playerans != answer[ansnum[i]]){
            cout << "INCORRECT" << endl;
            while(secondtry != "yes" && secondtry != "YES" && secondtry != "no" && secondtry != "NO") {
                cout << "Try again? Enter yes or no: ";
                cin >> secondtry;
            }
            if (secondtry == "yes"|| secondtry == "YES") {
                cout << " Here's Question Number: " << i + 1 << " (second try)" << endl;
                questskip = playerans - 64;
                show_question(question, questnum[i], questskip);
                playerans = player_try();
                //assigns score after wrong the first time
                if (playerans == answer[ansnum[i]]) {
                    cout << "Correct" << endl;
                    playerscore[i] = 2;
                }
                else if(playerans != answer[ansnum[i]]){
                    cout << "INCORRECT" << endl;
                    playerscore[i] = 0;
                }
            }
                else if (secondtry == "no"|| secondtry == "NO") {
                    playerscore[i] = 1;
                }
                secondtry = '\0';
        }

    }
    //assigns score
    for(int i = 0; i < 10; i++){
        //If correct first try, adds streak and increases streak
        if (playerscore[i] == 3){
            totalscore += streak;
            streak *= 10;
        }
            //If correct second try, adds half of streak and increases streak
        else if (playerscore[i] == 2){
            totalscore += streak * 0.5;
            streak *= 10;
        }
            //If wrong first try but does not try again, nothing happens
        else if (playerscore[i] == 1){
            totalscore += 0;
        }
            //If wrong both times, resets streak, shame upon family
        else if (playerscore[i] == 0){
            totalscore += 0;
            streak = 10;
        }

    }
    return totalscore;
}
void sort_score(string playername, int totalscore){
    string fileName, data, pastPlayer,topPlayer;
    int pastScore = 0, highScore = 0, playerRank = 1;
    fstream File;
    //Creates or opens score file
    while(!File.is_open()){
        cout << endl << "Please enter the name of the score file: ";
        cin >> fileName;
        File.open(fileName + ".txt" ,ios::out| ios::app);
        if (!File.is_open()){
            cout << "Error opening file" << endl;
        }
    }
    //Inputs player's name and score in file
    File << playername << '\t' << totalscore << '\n';
    //closes and rereads file
    File.close();
    File.open(fileName + ".txt" ,ios::in);
    //Finds top players score and sorts rank
    while (!File.eof()){
        File >> pastPlayer >> pastScore;
        if (pastScore > highScore){
            highScore = pastScore;
            topPlayer = pastPlayer;
        }
        if (pastScore > totalscore)
            playerRank += 1;
    }

    File.close();
    //Outputs user score and rank
    cout << playername << " achieved a score of " << totalscore << " points. \n"
         << playername << " ranked " << playerRank;
    if (playerRank == 1)
        cout << "st \n";
    else if (playerRank == 2)
        cout << "nd \n";
    else if (playerRank == 3)
        cout << "rd \n";
    else
        cout << "th \n";
    cout << "High score of " << highScore << " points achieved by " << topPlayer;

}
