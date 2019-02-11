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

        int getNfaStateIndex(string state, NFA nfa) {
            for(int i = 0; i < nfa.nStates; i++) {
                if(nfa.states[i] == state) {
                    return i;
                }
            }
            return -1;
            
        }
        void newTransition(string newState, NFA nfa) {
            vector < string > transition;
            for(int i = 0; i < nfa.nInputs; i++) {
                string state = "";
                transition.push_back(state);
            }
            

            for(int i = 0; i < newState.length(); i++) {
                int stateIndex = getNfaStateIndex(newState.substr(i, 1), nfa);
                for(int j = 0; j < nfa.nInputs; j++) {
                    string state = nfa.transitions[stateIndex][j];
                    if (state!="0") {
                        for(int k = 0; k < state.length(); k++) {
                            string sub = state.substr(k, 1);
                            if (transition[j].find(sub) == std::string::npos) {
                                transition[j] += sub;
                            }

                        }
                        

                        // if (transition[j].find(state) != std::string::npos) {
                           
                        // }
                        // else {
                        //     cout << "Pre transition is: " << transition[j] << endl;
                        //     transition[j] += state;
                        //     cout << "The transition is: " << transition[j] << endl;
                        // }
                    }
                }
            }

            for(int i = 0; i < nfa.nInputs; i++) {
                if(transition[i] == "") {
                    transition[i] = "T";
                    trapFlag = true;
                }
                else {
                    if(find(states.begin(), states.end(), transition[i]) == states.end() ) {
                        if(find(queue.begin(), queue.end(), transition[i]) == queue.end()) {
                            queue.push_back(transition[i]);
                        }
                    }
                }
            }

            transitions.push_back(transition);
        }

        void convertNfaToDfa(NFA nfa) {
            inState = nfa.inState;
            nInputs = nfa.nInputs;
            inputs = nfa.inputs;

            queue.push_back(nfa.inState);
            while(queue.size() > 0) {
                string newState = queue.back();
                queue.pop_back();
                states.push_back(newState);
                newTransition(newState, nfa);
            }

            if(trapFlag) {
                vector < string > transition;
                for(int i = 0; i < nfa.nInputs; i++) {
                    transition.push_back("T");
                }
                states.push_back("T");
                transitions.push_back(transition);
            }
            nStates = states.size();

            for(int i = 0; i < nStates; i++) {
                for(int j = 0; j < nfa.nFstates; j++) {
                    if (states[i].find(nfa.finalStates[j]) != string::npos) {
                        finalStates.push_back(states[i]);
                        break;
                    }
                }
            }
            nFstates = finalStates.size();
            cout <<"The DFA formed is:\n" << endl;
            printDfa();
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

};

int main() {
    NFA nfa;
    DFA dfa;
    const char *inName = "input.txt";

    const char *outName = "out.txt";
    
    nfa.deserialise(inName);
    cout << "\nConverting NFA to DFA ... \n" << endl;
    dfa.convertNfaToDfa(nfa);
    dfa.serialise(outName);

}