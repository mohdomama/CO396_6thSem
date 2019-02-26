#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


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
        
        int findIndex(string s, vector<string> vec) {
            for(int i = 0; i < vec.size(); i++) {
                if (vec[i] == s)
                    return i;
            }
            
        }
        bool toMark(int state1, int state2, bool **array) {
            for(int i = 0; i < nInputs; i++) {
                int x = findIndex(transitions[state1][i], states);
                int y = findIndex(transitions[state2][i], states);
                
                if (array[x][y]) {
                    return true;
                }
            }

            return false;
            
        }

        void minimise2() {
            bool **array;

            array = new bool *[nStates];

            for(int i = 0; i < nStates; i++) {
                array[i] = new bool[nStates];
            }

            for(int i = 0; i < nStates; i++) {
                for(int j = 0; j < nStates; j++) {
                    array[i][j] = false;
                }
            }
            
            for(int i = 1; i < nStates; i++) {
                for(int j = 0; j < nStates; j++) {
                    string x, y;
                    x = states[i];
                    y = states[j];

                    bool condition1 = find(finalStates.begin(), finalStates.end(), x) == finalStates.end(); // Not Present
                    bool condition2 = find(finalStates.begin(), finalStates.end(), y) != finalStates.end(); // Present
                    if (condition1 && condition2) {
                        array[i][j] = true;
                    }
                    if (!condition1 && !condition2) {
                        array[i][j] = true;
                    }
                }
            }


            bool match = false;    

            do {
                match = false;
                for(int i = 1; i < nStates; i++) {
                    for(int j = 0; j < i; j++) {
                        if (!array[i][j]) {
                            if(toMark(i, j, array)) {
                                
                                match = true;
                                array[i][j] = true;
                            };
                        }
                    }
                }
            
            } while (match);

            cout << "Table: " << endl;
            for(int i = 0; i < nStates; i++) {
                for(int j = 0; j <= i; j++) {
                    cout << array[i][j] << "  ";
                }
                cout << endl;
            }

            makePairs(array);   
        }

        void makePairs(bool **array) {
            int pairCheck[nStates];
            for(int i = 0; i < nStates; i++) {
                pairCheck[i] = -1;
            }

            vector <string> oldStates = states;
            
            vector < vector <int> > pairs;
            for(int i = 1; i < nStates; i++) {
                for(int j = 0; j < i; j++) {
                    if (!array[i][j]) {
                        if (pairCheck[i] == -1 && pairCheck[j] == -1) {
                            cout << "Pairing:" << endl;
                            pairCheck[i] = i;
                            pairCheck[j] = i;
                        }
                        else if(pairCheck[i] != -1 && pairCheck[j] == -1) {
                            cout << "Pairing:" << endl;
                            pairCheck[j] = pairCheck[i];
                        }
                        else if(pairCheck[i] == -1 && pairCheck[j] != -1) {
                            cout << "Pairing:" << endl;
                            pairCheck[i] = pairCheck[j];
                        }
                        
                    }
                    
                }
            }
            cout << "Pair check: \n";
            for(int i = 0; i < nStates; i++) {
                cout << states[i] << "\t";
            
            }
            cout << endl;
            for(int i = 0; i < nStates; i++) {
                cout << pairCheck[i] << "\t";
            
            }
            cout << endl;

            int ostates = nStates;  // nstates will change
            for(int i = 0; i < ostates; i++) {
                int elem = -1;
                int nexElem = -1;
                for(int j = 0; j < ostates; j++) {
                    if (pairCheck[j] == i) {
                        if (elem == -1) {
                            
                            elem = findIndex(oldStates[j], states);
                        }
                        else {
                            nexElem = findIndex(oldStates[j], states);
                            joinStates(elem, nexElem);
                        }
                    }
                    
                }
                
            }
            
            
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
    const char *inName = "input2.txt";

    const char *outName = "out.txt";
    
    dfa.deserialise(inName);
    dfa.printDfa();
    dfa.minimise2();
    dfa.printDfa();

}