/**
 * Author: Tinashe Kucherera
 * Date: 2024-06-20
 * Description: The Entry point for my Trivia Game Application.
 */


 /**
  * This is the Startup Displaye menu here 
  * 
  */
 #include <iostream>
 using namespace std;

 class MainMenu {
    public:
        short numPlayers;
        string category;
        short start;

        void display_title() {
            cout << endl << endl;
            cout << "Welcome to the NC A&T Trivia Game!" << endl << endl << endl;
        }

        void show_options() {
            cout << "1. Input the  Number of Players: ";
            cin >> numPlayers;
            cout << endl;

            cout << "2. Choose Category (1- General Knowledge, 2- CDSE Department, 3- History): ";
            cin >> category;
            cout << endl;

            cout << "Are you ready to start? 1 to start 0 to close: ";
            cin >> start;
            cout << endl;

            if (start == 1) {
                start_game();
            } else {
                cout << "Exiting the game. Goodbye!" << endl;
            }
        }

        void show_choices() {
            cout << "You have chosen " << numPlayers << " players and category " << category << "." << endl;
        }

        void start_game() {
            cout << "Starting the game..." << endl;
        }


 };

 /**
  * Below is the class for reading data from the json file. 
  */
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

 class Data {
    
    public:
        string category;

        json data;

        int load_questions(const string& category) {
            ifstream f("questions.json");
            if (!f.is_open()) {
                cerr << "Error opening file!" << endl;
                return 1;
            }

            try {
                json all_data;
                f >> all_data;

                // Check if category exists
                if (!all_data["categories"].contains(category)) {
                    cerr << "Category not found!" << endl;
                    return 1;
                }

                json cat = all_data["categories"][category];

                cout << "Category: " << cat["name"] << "\n";

                data = cat["questions"];

                return 0;

            } catch (json::parse_error& e) {
                cerr << "JSON parse error: " << e.what() << endl;
                return 1;
            }
        }
 };

 /**
  * Leaderboard and Scoring
  * Initialize the Leaderboard and track Scoring
  */
 #include <vector>

class Player {
    public:
        string name;
        int score; 

    Player(string n) : name(n), score(0) {}

    void displayInfo() const {
        cout << "            " << name << ":  " << score << endl;
    }
};

class LeaderBoard {
    public:
        int numPlayers;

        vector<Player> playerScores;

        void init(int numPlayers) {
            for (int c=1; c <= numPlayers; c++ ){
                string playerName = "player" + to_string(c);

                playerScores.emplace_back(playerName);
            }
        }

        void update_score(int player){
            // because vectors are zero indexed need to move back
            playerScores[player-1].score += 10;
        }


        void display() {
            if (playerScores.empty()) {
                cout << "No players to display." << endl;
                return;
            }

            // Sort a local copy so original order is preserved
            vector<Player> sorted = playerScores;

            sort(sorted.begin(), sorted.end(), [](const Player& a, const Player& b) {
                return a.score > b.score; // highest score first
            });

            cout << "          Leaderboard:" << endl;

            int rank = 1;
            for (const auto& p : sorted) {
                // cout << rank++ << ". ";
                p.displayInfo();
            }
        }

};



 class Game {

    public:
        /**
         * This is how the game works 
         * We have the number of rounds we will say 5 for now 
         * with each round each user get a unique if they get it right 
         * they get a score increment else their score is the same 
         * move to the next question
         * when done move on display the winner. 
         */
        int numPlayers;

        int numRounds = 5;

        json data;

        
        void begin(int numPlayers, int numRounds, json data) {
            cout << "Game has begun!" << endl;
            // create and initialize a leaderboard for this game
            LeaderBoard leaderboard;
            leaderboard.numPlayers = numPlayers;
            leaderboard.init(numPlayers);

            int qIndex = 0;
            for (int round = 1; round <= numRounds; ++round){
                cout << "round " << round << endl;
                cout << endl;
                for (int player = 1; player <= numPlayers; player++){
                    // Grab a qustion for player 
                    // will add logi fo uniquenes later 
                    cout << "Player " << player << endl;
                    int response {};
                    qIndex++;
                    string question = data[qIndex]["question"];
                    cout << question << endl;
                    for (int i = 0; i< data[qIndex]["choises"].size(); ++i){
                        cout << i << ": " << data[qIndex]["choises"][i] << endl;
                    }
                    cout << "Enter your Answer: ";
                    cin >> response;
                    while (!(cin >> response) || response < 0 || response > 4) {
                        cout << "Invalid input. Try again: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    if (response == data[qIndex]["answer"]){
                        cout << "Your answer: "  << response << " Correct " << endl;
                        leaderboard.update_score(player);
                    } else {
                        cout << "Your answer: " << response << " Wrong" << endl;
                        cout << "Correct answer: " << data[qIndex]["answer"] << endl;
                    }
                    leaderboard.display();
                    cout << endl;
                    cout << endl;
                    
                }

            }
        }
 };


int main() {
    MainMenu menu;
    menu.display_title();
    menu.show_options();


    Data data;
    data.load_questions(menu.category);

    Game game;
    game.begin(menu.numPlayers, 3, data.data);
    //cout << data.data;
    
    return 0;
}