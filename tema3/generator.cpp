#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int nrSimbParteDreapta(string rhs) {
    if (rhs == "/* empty */") return 0;
    stringstream ss(rhs);
    string word;
    int count = 0;
    while (ss >> word) count++;
    return count;
}

void generateTables() {
    ifstream fin("test.output");
    ofstream fTA("TA.txt");
    ofstream fTS("TS.txt");
    ofstream fReg("Reguli.txt");

    string line;
    string parteaStangaCurentaProductie = "";
    int currentState = -1;
    bool inGrammarSection = false;

    //cout << "Generare tabele din test.output..." << endl;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string word1, word2;
        ss >> word1;

        if (word1 == "Grammar") {
            inGrammarSection = true;
            continue;
        }
        if (word1 == "Terminals,") {
            inGrammarSection = false;
            continue;
        }

        if (inGrammarSection) {
            int ruleNum;
            try { ruleNum = stoi(word1); }
            catch (...) { continue; }

            if (ruleNum == 0) continue;

            string separator;
            ss >> separator;
            string rhsString;

            if (separator == "|") {
                getline(ss, rhsString);
            }
            else {
                if (separator.back() == ':') separator.pop_back();
                parteaStangaCurentaProductie = separator;
                getline(ss, rhsString);
            }

            int len = nrSimbParteDreapta(rhsString);
            fReg << ruleNum << " " << parteaStangaCurentaProductie << " " << len << endl;
        }

        if (word1 == "State") {
            ss >> currentState;
            continue;
        }

        if (currentState != -1 && !inGrammarSection) {
            string symbol = word1;
            ss >> word2;

            if (symbol == "$end") symbol = "$";
            if (symbol.size() >= 3 && symbol.front() == '\'' && symbol.back() == '\'') {
                symbol = symbol.substr(1, symbol.size() - 2);
            }

            if (word2 == "shift,") {
                string trash, stateNum;
                ss >> trash >> trash >> trash >> trash >> stateNum;//trash
                fTA << currentState << " " << symbol << " d" << stateNum << endl;
            }
            else if (word2 == "go") {
                string trash, stateNum;
                ss >> trash >> trash >> stateNum;
                fTS << currentState << " " << symbol << " " << stateNum << endl;
            }
            else if (word2 == "reduce") {
                string trash, ruleNum;
                ss >> trash >> trash >> ruleNum;
                if (symbol == "$default") {
                    fTA << currentState << " DEFAULT r" << ruleNum << endl;
                }
                else {
                    fTA << currentState << " " << symbol << " r" << ruleNum << endl;
                }
            }
            else if (word2 == "accept") {
                if (symbol == "$default") symbol = "DEFAULT";
                fTA << currentState << " " << symbol << " acc" << endl;
            }
        }
    }

    fin.close();
    fTA.close();
    fTS.close();
    fReg.close();

    cout << "done generating\n" << endl;
}