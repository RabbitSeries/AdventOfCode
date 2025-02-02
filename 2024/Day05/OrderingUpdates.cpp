#include <bits/stdc++.h>
using namespace std;

bool isCorrectOrder( const vector<int>& update, const unordered_map<int, set<int>>& rules ) {
    for( int i = 0; i < update.size(); i++ ) {
        for( int j = 0; j < i; j++ ) {
            if( rules.count( update[i] ) != 0 ) {
                if( rules.at( update[i] ).count( update[j] ) != 0 )
                    return false;
            }
            else {
                break;
            }
        }
    }
    return true;
}

unordered_map<int, set<int>> readRules() {
    unordered_map<int, set<int>> rules;
    FILE* RuleInput = fopen( "RuleInput.txt", "r" );

    char linebuf[BUFSIZ];
    while( fgets( linebuf, BUFSIZ, RuleInput ) ) {
        stringstream ss( linebuf );
        int num1 = 0, num2 = 0; char sep = '\0';
        ss >> num1 >> sep >> num2;
        rules[num1].insert( num2 );
    }
    return move( rules );

}

vector<vector<int>> readUpdates() {
    vector<vector<int>> updates;
    char linebuf[BUFSIZ];
    FILE* UpdatesInput = fopen( "UpdatesInput.txt", "r" );
    while( fgets( linebuf, BUFSIZ, UpdatesInput ) ) {
        stringstream ss( linebuf );
        int tmp = 0;
        vector<int> update;
        while( ss >> tmp ) {
            update.push_back( tmp );
            char comma = '\0';
            if( ( ss >> comma ) && comma == ',' ) {
                continue;
            }
        }
        if( update.size() > 0 )
            updates.push_back( update );
    }
    return move( updates );
}


void Solution1() {
    unordered_map<int, set<int>> rules = readRules();
    vector<vector<int>> updates = readUpdates();
    int sum = 0;
    for( const auto& update : updates ) {
        if( isCorrectOrder( update, rules ) ) {
            int middle = update.size() / 2;
            sum += update[middle];
        }
    }
    cout << "Solution 1: " << sum << endl;
}

void reOrder( vector<int>& update, const unordered_map<int, set<int>>& rules ) {
    for( int i = 0; i < update.size(); i++ ) {
        for( int j = i; j < update.size(); j++ ) {
            bool ahead = true;
            for( int k = i; k < update.size(); k++ ) {
                if( k == j ) { continue; }
                if( rules.count( update[k] ) != 0 && rules.at( update[k] ).count( update[j] ) != 0 ) { ahead = false; break; }
            }
            if( ahead ) {
                swap( update[i], update[j] );
                break;
            }
        }
    }
}

int Solution2() {
    unordered_map<int, set<int>> rules = readRules();
    vector<vector<int>> updates = readUpdates();
    int sum = 0;
    for( vector<int> update : updates ) {
        if( !isCorrectOrder( update, rules ) ) {
            reOrder( update, rules );
            int middle = update.size() / 2;
            sum += update[middle];
        }
    }
    cout << "Solution 2: " << sum << endl;
    return 0;
}

int main() {
    Solution1();
    Solution2();
    return 0;
}