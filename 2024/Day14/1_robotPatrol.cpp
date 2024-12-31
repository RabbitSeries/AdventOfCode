#include "bits/stdc++.h"
using namespace std;
#define BUFFER_SIZE 1024
#define WIDTH 101
#define HEIGHT 103
typedef vector<vector<int>> room;
typedef struct robot {
    robot();
    robot( int leftDis, int topDis, int horizenVel, int verticalVel ) :
        curPos( leftDis, topDis ),
        vel( horizenVel, verticalVel ) {};
    pair<int, int> curPos;
    pair<int, int> vel;
}robot;
typedef vector<robot> specs;
typedef long long ll;
ll quadrantCount( specs s ) {
    // vector<vector<bool>> stacked( HEIGHT, vector<bool>( WIDTH, false ) );
    int wmiddle = WIDTH / 2, hmiddle = HEIGHT / 2;
    ll q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for( auto rbt : s ) {
        /**
         * i : distance to top
         * j : distance to left
         */
        int i = rbt.curPos.second;
        int j = rbt.curPos.first;
        // 214325760
        // if( !stacked[i][j] ) {
        if( i < hmiddle && j < wmiddle ) {
            q1++;
        }
        else if( i > hmiddle && j < wmiddle ) {
            q2++;
        }
        else if( i > hmiddle && j > wmiddle ) {
            q3++;
        }
        else if( i < hmiddle && j > wmiddle ) {
            q4++;
        }
        // }
        // stacked[i][j] = true;
    }
    return q1 * q2 * q3 * q4;
}

void printMap( specs s ) {
    // system( "cls" );

    vector<vector<int>> room( HEIGHT, vector<int>( WIDTH, -1 ) );
    int id = 0;
    for( robot r : s ) {
        room[r.curPos.second][r.curPos.first] = id++;
    }
    for( auto line : room ) {
        for( auto id : line ) {
            if( id >= 0 ) {
                // cout << id % 10;
                cout << "*";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void step( specs& s ) {
    for( auto& rbt : s ) {
        rbt.curPos.first = ( WIDTH + rbt.curPos.first + rbt.vel.first ) % WIDTH;
        rbt.curPos.second = ( HEIGHT + rbt.curPos.second + rbt.vel.second ) % HEIGHT;
    }
    return;
}

int main() {
    FILE* input = fopen( "input.txt", "r" );
    // p=40,80 v=-83,31
    regex re( "p=([0-9]+),([0-9]+).*v=(-?[0-9]+),(-?[0-9]+)" );
    smatch m;
    char buf[BUFFER_SIZE] = "\0";
    vector<robot> specs;
    while( !feof( input ) && fgets( buf, BUFFER_SIZE, input ) ) {
        string tmp( buf );
        regex_search( tmp, m, re );
        if( m.size() == 5 ) {
            specs.push_back( robot( stoi( m[1] ), stoi( m[2] ), stoi( m[3] ), stoi( m[4] ) ) );
        }
    }
    int elapseTime = 0;
    int close = 0;
    // while( elapseTime++ < 100 ) {
    // This pattern is funny.
    while( 1 ) {
        if( elapseTime > close * 100 && elapseTime % 100 == 23 + close ) {
            close++;
            system( "clear" );
            printMap( specs );
            cout << elapseTime << endl;
        }
        step( specs );
        elapseTime++;
    }
    cout << quadrantCount( specs ) << endl;
}