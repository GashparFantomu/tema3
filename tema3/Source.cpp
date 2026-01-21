#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <sstream>
using namespace std;

struct Production {
    string left;
    int nrElemDeStersStiva = 0;
};

struct Grammar {
    vector<Production> productii;
    map<int, map<string, string>> tabelActiuni;
    map<int, map<string, int>> tabelSalt;
};

struct StackElement {
    string symbol;
    int state;
};

Grammar gr;

vector<string> split(string str) {
    vector<string> result;
    stringstream ss(str);
    string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

void loadGrammar() {
    ifstream fin("Reguli.txt");
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int index;
        string left;
        int len;

        ss >> index >> left >> len;

        if (index >= gr.productii.size()) {
            gr.productii.resize(index + 1);
        }

        gr.productii[index] = { left, len };
    }
    fin.close();
}

void loadTables() {
    ifstream finTA("TA.txt");
    string line;

    while (getline(finTA, line)) {
        if (line.empty()) continue;

        vector<string> parts = split(line);
        if (parts.size() < 3) continue;

        int stare = stoi(parts[0]);
        gr.tabelActiuni[stare][parts[1]] = parts[2];
    }
    finTA.close();

    ifstream finTS("TS.txt");
    while (getline(finTS, line)) {
        if (line.empty()) continue;

        vector<string> parts = split(line);
        if (parts.size() < 3) continue;

        int stare = stoi(parts[0]);
        int stareNoua = stoi(parts[2]);
        gr.tabelSalt[stare][parts[1]] = stareNoua;
    }
    finTS.close();
}

void printStack(stack<StackElement> stiva) {
    vector<StackElement> elements;
    while (!stiva.empty()) {
        elements.push_back(stiva.top());
        stiva.pop();
    }
    for (int i = elements.size() - 1; i >= 0; i--) {
        cout << elements[i].symbol << elements[i].state;
    }
}

vector<string> tokenize(string input) {
    vector<string> tokens;
    int i = 0;
    while (i < input.length()) {
        if (i + 1 < input.length() && input[i] == 'i' && input[i + 1] == 'd') {
            tokens.push_back("id");
            i += 2;
        }
        else {
            tokens.push_back(string(1, input[i]));
            i++;
        }
    }
    return tokens;
}

bool analyze(string input) {
    stack<StackElement> stiva;
    stiva.push({ "$", 0 });

    vector<string> tokens = tokenize(input);
    tokens.push_back("$");
    int indexSimbolCurentDinInput = 0;

    cout << "\n=== Analiza sirului: " << input << " ===\n";
    cout << "Stiva\t\t\tInput\t\tActiune\n";
    cout << "----------------------------------------------------\n";

    while (true) {
        int stare = stiva.top().state;
        string simbol = tokens[indexSimbolCurentDinInput];

        printStack(stiva);
        cout << "\t\t";
        for (int k = indexSimbolCurentDinInput; k < tokens.size(); k++) {
            cout << tokens[k];
        }
        cout << "\t\t";

        string actiune = "";
        if (gr.tabelActiuni[stare].count(simbol)) {
            actiune = gr.tabelActiuni[stare][simbol];
        }
        else if (gr.tabelActiuni[stare].count("DEFAULT")) {
            actiune = gr.tabelActiuni[stare]["DEFAULT"];
        }
        else {
            cout << "EROARE\n";
            return false;
        }

        cout << actiune << "\n";

        if (actiune[0] == 'd') {
            int stareNoua = stoi(actiune.substr(1));
            stiva.push({ simbol, stareNoua });
            if (simbol != "$") indexSimbolCurentDinInput++;
        }
        else if (actiune[0] == 'r') {
            int nrProductie = stoi(actiune.substr(1));
            Production prod = gr.productii[nrProductie];

            for (int i = 0; i < prod.nrElemDeStersStiva; i++) {
                stiva.pop();
            }

            int stareAnterioara = stiva.top().state;
            int stareNoua = gr.tabelSalt[stareAnterioara][prod.left];
            stiva.push({ prod.left, stareNoua });
        }
        else if (actiune == "acc") {
            cout << "\nSir ACCEPTAT!\n";
            return true;
        }
    }
}

void generateTables();

int main() {
    generateTables();

    loadGrammar();
    loadTables();

    string input;
    cout << "Introduceti sirul: ";
    cin >> input;

    analyze(input);

    return 0;
}