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
                    }
                }
            }

            for(int i = 0; i < nfa.nInputs; i++) {
                if(transition[i] == "") {
                    transition[i] = "T";
                    trapFlag = true;
                }
                else {
                    bool arePerm = false;
                    for(int j = 0; j < states.size(); j++) {
                        
                        if (arePermutation(transition[i], states[j])) {
                            arePerm = true;
                            break;
                        }
                    }

                    if(!arePerm){
                        for(int j = 0; j < queue.size(); j++) {
                            if(arePermutation(transition[i], queue[j])) {
                                arePerm = true;
                            break;
                            }
                        }
                    }

                    if(!arePerm) {
                        queue.push_back(transition[i]);
                    } 
                }
            }

            transitions.push_back(transition);
        }

        bool arePermutation(string str1, string str2) { 
            // Get lenghts of both strings 
            int n1 = str1.length(); 
            int n2 = str2.length(); 
        
            // If length of both strings is not same, 
            // then they cannot be Permutation 
            if (n1 != n2) 
                return false; 
        
            // Sort both strings 
            sort(str1.begin(), str1.end()); 
            sort(str2.begin(), str2.end()); 
        
            // Compare sorted strings 
            for (int i = 0; i < n1;  i++) 
            if (str1[i] != str2[i]) 
                return false; 
        
            return true; 
        } 

        void convertNfaToDfa(NFA nfa) {
            inState = nfa.inState;
            nInputs = nfa.nInputs;
            inputs = nfa.inputs;

            // Main conversion logic
            queue.push_back(nfa.inState);
            while(queue.size() > 0) {
                string newState = queue.back();
                queue.pop_back();
                states.push_back(newState);
                newTransition(newState, nfa);
            }

            // Add a trap state if DFA requires one
            if(trapFlag) {
                vector < string > transition;
                for(int i = 0; i < nfa.nInputs; i++) {
                    transition.push_back("T");
                }
                states.push_back("T");
                transitions.push_back(transition);
            }
            nStates = states.size();

            // Set final states of the DFA
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