#include <iostream>
#include <unordered_map> 
#include <cctype>
#include <string>
#include <map>

using namespace std;

unordered_map<char,char> PlugBoard;
void setPlugBoard(char A, char B){
    PlugBoard.insert(make_pair(A,B));
    PlugBoard.insert(make_pair(B,A));
}
char runPlugBoard(char c){
    if(PlugBoard.find(c) != PlugBoard.end()){
        return PlugBoard[c];
    }
    return c;
}

map<char,char> rotors[3];
int rotorIndexes[3];
string rotorValues[3] = {"ekmflgdqvzntowyhxuspaibrcj", "ajdksiruxblhwtmcqgznpyfvoe", "bdfhjlcprtxvznyeiwgakmusqo"};
//                        abcdefghijklmnopqrstuvwxyz    abcdefghijklmnopqrstuvwxyz    abcdefghijklmnopqrstuvwxyz
void createRotors() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 26; j++) {
            char A = 97 + j;
            char B = rotorValues[i][j];
            rotors[i].insert(make_pair(A, B));
        }
    }
}
void advanceRotors() {
    // Advance the rightmost rotor
    rotorIndexes[0] = (rotorIndexes[0] + 1) % 26;

    // Check if the rightmost rotor completes a full rotation
    if (rotorIndexes[0] == 0) {
        // Advance the middle rotor
        rotorIndexes[1] = (rotorIndexes[1] + 1) % 26;

        // Check if the middle rotor completes a full rotation
        if (rotorIndexes[1] == 0) {
            // Advance the leftmost rotor
            rotorIndexes[2] = (rotorIndexes[2] + 1) % 26;
        }
    }
}

map<char, char> reflector;
string reflectorValues = "yruhqsldpxngokmiebfzcwvjat";
//                        abcdefghijklmnopqrstuvwxyz
void createReflector() {
    for (int j = 0; j < 26; j++) {
        char A = 97 + j;
        char B = reflectorValues[j];
        reflector.insert(make_pair(A, B));
    }
}
char applyReflector(char c) {
    return reflector[c];
}

int CurrentState = 0;
string Enigma(string input);

void printMenu(){
    cout << "[Menu]\n";
    cout << "0 - Exit\n";
    cout << "1 - Type\n";
    cout << "2 - Set Plug Board\n";
    cout << "3 - Set Rotors\n";
    int input;
    cin >> input;
    if(input > 0 && input <= 3){
        CurrentState = input;
    }else if(input == 0){
        CurrentState = -1;
    }else{
        cout << "Invalid Input\n";
    }
}

void printType(){
    cout << "[Type]\n";
    cout << "0 - Menu\n";
    while(true){
        string input;
        cin >> input;
        if(input == "0"){
            CurrentState = 0;
            break;
        }
        cout << Enigma(input) << endl << endl;
    }
}

void printPlugBoard(){
    cout << "[Set Plug Board]\n";
    cout << "0 - Menu\n";
    cout << "1 - Clear Plug Board\n";
    cout << "Enter Plug Board settings in letter pairs (a - z): 'ot', 'LE', 'tM'\n";
    while(true){
        string input;
        cin >> input;
        string temp;
        for(char  c : input){temp += tolower(c);}
        input = temp;
        if(input == "0"){
            CurrentState = 0;
            break;
        } else if (input == "1"){
            PlugBoard.clear();
            cout << "Plug Board Cleared\n";
            continue;
        }
        if(input.length() != 2 || input[0] < 'a' || input[0] > 'z' || input[1] < 'a' || input[1] > 'z'){
            cout << "Invalid Input\n";
            continue;
        }
        setPlugBoard(input[0], input[1]);
    }
}

void printRotors(){
    cout << "[Set Rotors]\n";
    cout << "0 - Menu\n";
    cout << "Enter Rotor possitions in order 1-2-3 (1 - 26): '3 22 9', '13 6 8' \n";

    int nums[3];
    cin >> nums[0];
    if(nums[0] == 0){
        CurrentState = 0;
        return;
    }
    cin >> nums[1] >> nums[2];

    rotorIndexes[0] = nums[0] - 1;
    rotorIndexes[1] = nums[1] - 1;
    rotorIndexes[2] = nums[2] - 1;

    cout << "Rotor Positions set to " << rotorIndexes[0] + 1 << " " << rotorIndexes[1] + 1 << " " << rotorIndexes[2] + 1 << "\n"; 
    CurrentState = 0;
}

int run() {
    while (CurrentState != -1) {
        switch (CurrentState) {
            case 0:
                printMenu();
                break;
            case 1:
                printType();
                break;
            case 2:
                printPlugBoard();
                break;
            case 3:
                printRotors();
                break;
            default:
                cout << "Unknown state, returning to Menu...\n";
                CurrentState = 0;
                break;
        }
    }
    return 0; // Exit the program
}

string Enigma(string input) {
    string output;
    for (char c : input) {
        if (!isalpha(c)) {
            output += c;
            continue;
        }
        char lower_c = tolower(c);

        // Advance rotors
        advanceRotors();
        cout << "Rotors after advance: " << rotorIndexes[0] << " " << rotorIndexes[1] << " " << rotorIndexes[2] << endl;

        // Apply plug board
        char encrypted_c = runPlugBoard(lower_c);
        cout << "After plug board: " << encrypted_c << endl;

        // Forward pass through rotors
        for (int i = 0; i < 3; i++) {
            char offset_c = encrypted_c + rotorIndexes[i];
            encrypted_c = rotors[i][offset_c];
        }
        cout << "After rotors: " << encrypted_c << endl;

        // Apply reflector
        encrypted_c = applyReflector(encrypted_c);
        cout << "After reflector: " << encrypted_c << endl;

        // Backward pass through rotors
        for (int i = 2; i >= 0; i--) {
            char offset_c = encrypted_c + rotorIndexes[i];
            encrypted_c = rotors[i][offset_c];
        }
      
        cout << "After reverse rotors: " << encrypted_c << endl;

        // Apply plug board again
        encrypted_c = runPlugBoard(encrypted_c);

        output += encrypted_c;
    }
    return output;
}


void testEncryptionDecryption() {
    // Reset rotors and plugboard
    rotorIndexes[0] = rotorIndexes[1] = rotorIndexes[2] = 0;
    PlugBoard.clear();
    cout << "Rotor Positions set: " << rotorIndexes[0] << " " << rotorIndexes[1] << " " << rotorIndexes[2] << "\n"; 

    string originalText = "a";
    string encryptedText = Enigma(originalText);

    rotorIndexes[0] = rotorIndexes[1] = rotorIndexes[2] = 0;
    cout << "Rotor Positions set: " << rotorIndexes[0] << " " << rotorIndexes[1] << " " << rotorIndexes[2] << "\n"; 

    string decryptedText = Enigma(encryptedText);

    cout << "Original: " << originalText << endl;
    cout << "Encrypted: " << encryptedText << endl;
    cout << "Decrypted: " << decryptedText << endl;
}

int main(int argc, char *argv[]){
    createRotors();
    rotorIndexes[0] = 0;
    rotorIndexes[1] = 0;
    rotorIndexes[2] = 0;

    createReflector();

    if(argc > 1){
        testEncryptionDecryption();
        return 0;
    }

    return run();
}