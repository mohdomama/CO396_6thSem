#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

class NFA {
    public:
        vector< string > states;
        vector< string > inputs;
        vector< vector < string > > transitions;
        string inState;
        vector < string > finalStates;
        int nStates, nFstates, nInputs;
    
        void deserialise(const char *fileName) {
            ifstream fin;
            fin.open(fileName);
            if(!fin) {
                cerr << "Error in opening the file" << endl;
                return; // if this is main
            }
            
            // States
            fin >> nStates;
            for(int i = 0; i < nStates; i++) {
                string state;
                fin >> state;
                states.push_back(state);
            }
            
            // Inputs
            fin >> nInputs;
            for(int i = 0; i < nInputs; i++) {
                string input;
                fin >> input;
                inputs.push_back(input);
            }

            // State Transition Table
            for(int i = 0; i < nStates; i++) {
                vector< string > transition;
                for(int j = 0; j < nInputs; j++) {
                    string state;
                    fin >> state;
                    transition.push_back(state);
                }
                transitions.push_back(transition);
                
            }
            
            // Initial State
            fin >> inState;

            // Final States
            fin >> nFstates;
            for(int i = 0; i < nFstates; i++) {
                string state;
                fin >> state;
                finalStates.push_back(state);
            }
            
            cout << "The Nfa is: " <<endl;
            printNfa();
            
            fin.close();
        }

        void printNfa() {
            cout << "States:" << endl;
            for(int i = 0; i < nStates; i++) {
                cout << states[i] << " ";
            }
            cout << endl;

            cout << "Inputs:" << endl;
            for(int i = 0; i < nInputs; i++) {
                cout << inputs[i] << " ";
            }
            cout << endl;

            cout << "State Transition Table:" << endl;
            for(int i = 0; i < nStates; i++) {
                cout << states[i] << ":\t";
                for(int j = 0; j < nInputs; j++) {
                    cout << transitions[i][j] << "\t";
                }
                cout << endl;
            }
            
            cout << "Initial State:" << endl;
            cout << inState << endl;

            cout << "Final States:" << endl;
            for(int i = 0; i < nFstates; i++) {
                cout << finalStates[i] << " ";
            }
            cout << endl;
        }
};

class DFA {
    public:
        vector< string > states;
        vector< string > inputs;
        vector< vector < string > > transitions;
        string inState;
        vector < string > finalStates;
        int nStates, nFstates, nInputs;

        vector< string > queue;
        bool trapFlag;

        DFA () {
            trapFlag = false;
        }

        bool areEquivalent(int state1, int state2) {
            bool condition1 = find(finalStates.begin(), finalStates.end(), states[state1]) == finalStates.end(); // Not Present
            bool condition2 = find(finalStates.begin(), finalStates.end(), states[state2]) != finalStates.end(); // Present
            if (condition1 && condition2) {
                return false;
            }
            if (!condition1 && !condition2) {
                return false;
            }
            
            for(int i = 0; i < nInputs; i++) {
                string x, y;
                x = transitions[state1][i];
                y = transitions[state2][i];
                if (x != y) {
                    return false;
                }

                bool condition1 = find(finalStates.begin(), finalStates.end(), x) == finalStates.end(); // Not Present
                bool condition2 = find(finalStates.begin(), finalStates.end(), y) != finalStates.end(); // Present
                if (condition1 && condition2) {
                    return false;
                }
                if (!condition1 && !condition2) {
                    return false;
                }

            }

            return true;
            
        }

        void minimise2() {
            int array[nStates][nStates];
            for(int i = 1; i < nStates; i++) {
                for(int j = 0; j <= i; j++) {
                    string x, y;
                    x = states[i];
                    y = states[j];

                    bool condition1 = find(finalStates.begin(), finalStates.end(), x) == finalStates.end(); // Not Present
                    bool condition2 = find(finalStates.begin(), finalStates.end(), y) != finalStates.end(); // Present
                    
                }
                
            }
            
        }

        void minimise() {
            bool match = false;
            int match1, match2;
            do {
                match = false;
                for(int i = 0; i < nStates - 1; i++) {
                    for(int j = i + 1; j < nStates; j++) {
                        match = areEquivalent(i, j);
                        if (match) {
                            cout << "Matched states: " << i << " " << j << endl; 
                            match1 = i;
                            match2 = j;
                            break;
                        }
                    }

                    if (match) {
                        break;
                    }
                }

                if (match) {
                    joinStates(match1, match2);
                }

            } while(match);
            
        }

        void joinStates(int state1, int state2) {
            string newState = states[state1] + states[state2];
            bool isFinal = false;
            if (find(finalStates.begin(), finalStates.end(), states[state1]) != finalStates.end()) {
                isFinal = true;
            }

            for(int i = 0; i < nStates; i++) {
                for(int j = 0; j < nInputs; j++) {
                    if (transitions[i][j] == states[state1] || transitions[i][j] == states[state2]) {
                        transitions[i][j] = newState;
                    }
                }
            }
            
            transitions.erase(transitions.begin() + state2);
            
            

            if(isFinal) {
                finalStates.erase(remove(finalStates.begin(), finalStates.end(), states[state1]), finalStates.end());
                finalStates.erase(remove(finalStates.begin(), finalStates.end(), states[state2]), finalStates.end());
                finalStates.push_back(newState);
                nFstates -= 1;
            }

            states[state1] = newState;
            states.erase(states.begin() + state2);
            nStates -= 1;
        
            

        }

        void printDfa() {
            cout << "States:" << endl;
            for(int i = 0; i < nStates; i++) {
                cout << states[i] << " ";
            }
            cout << endl;

            cout << "Inputs:" << endl;
            for(int i = 0; i < nInputs; i++) {
                cout << inputs[i] << " ";
            }
            cout << endl;

            cout << "State Transition Table:" << endl;
            for(int i = 0; i < nStates; i++) {
                cout << states[i] << ":\t";
                for(int j = 0; j < nInputs; j++) {
                    cout << transitions[i][j] << "\t";
                }
                cout << endl;
            }
            
            cout << "Initial State:" << endl;
            cout << inState << endl;

            cout << "Final States:" << endl;
            for(int i = 0; i < nFstates; i++) {
                cout << finalStates[i] << " ";
            }
            cout << endl;
        }

        void serialise(const char* fileName) {
            ofstream fout;
            fout.open(fileName);
            if(!fout) {
                cerr << "Error in opening the file" << endl;
                return; // if this is main
            }
            

            // States
            fout << nStates << endl;
            for(int i = 0; i < nStates; i++) {
                fout << states[i] <<"\t";
            }
            fout << endl;
            
            // Inputs
            fout << nInputs << endl;
            for(int i = 0; i < nInputs; i++) {
                fout << inputs[i] << "\t";
            }
            fout << endl;
            fout << endl;

            // State Transition Table
            for(int i = 0; i < nStates; i++) {
                for(int j = 0; j < nInputs; j++) {
                    fout << transitions[i][j] << "\t";
                }
                fout << endl;
            }
            fout << endl;
            
            // Initial State
            fout << inState << endl;

            // Final States
            fout << nFstates << endl;
            for(int i = 0; i < nFstates; i++) {
                fout << finalStates[i] << "\t";
            }
            fout << endl;

        }

        void deserialise(const char *fileName) {
            ifstream fin;
            fin.open(fileName);
            if(!fin) {
                cerr << "Error in opening the file" << endl;
                return; // if this is main
            }
            
            // States
            fin >> nStates;
            for(int i = 0; i < nStates; i++) {
                string state;
                fin >> state;
                states.push_back(state);
            }
            
            // Inputs
            fin >> nInputs;
            for(int i = 0; i < nInputs; i++) {
                string input;
                fin >> input;
                inputs.push_back(input);
            }

            // State Transition Table
            for(int i = 0; i < nStates; i++) {
                vector< string > transition;
                for(int j = 0; j < nInputs; j++) {
                    string state;
                    fin >> state;
                    transition.push_back(state);
                }
                transitions.push_back(transition);
                
            }
            
            // Initial State
            fin >> inState;

            // Final States
            fin >> nFstates;
            for(int i = 0; i < nFstates; i++) {
                string state;
                fin >> state;
                finalStates.push_back(state);
            }
            
            fin.close();
        }

};

int main() {
    DFA dfa;
    const char *inName = "input.txt";

    const char *outName = "out.txt";
    
    dfa.deserialise(inName);
    dfa.printDfa();
    dfa.minimise();
    dfa.printDfa();

}