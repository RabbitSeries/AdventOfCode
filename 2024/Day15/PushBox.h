#include "bits/stdc++.h"
using namespace std;

// const int CELLWALL = -1;
// const int CELLEMPTY = -2;
// const int BOT = -3;
// const int BUF = 4096;

class PushBox {

    // Simple data member is able to initialize
    const int CELLWALL = -1;
    const int CELLEMPTY = -2;
    const int BOT = -3;
    const int BUF = 4096;


    typedef long long ll;
    typedef struct pos {
        pos() {};
        pos( int a, int b ) : x( a ), y( b ) {}
        int x, y;
    }pos;

    typedef struct boxPos {
        boxPos() {}
        boxPos( pos a, pos b ) : l( a ), r( b ) {}
        pos l, r;
    }bos;


    pos getNextPos( char const c, pos const curPos ) {
        pos nextPos;
        switch( c ) {
        case '^':
            nextPos = pos( curPos.x - 1, curPos.y );
            break;
        case '>':
            nextPos = pos( curPos.x, curPos.y + 1 );
            break;
        case 'v':
            nextPos = pos( curPos.x + 1, curPos.y );
            break;
        case '<':
            nextPos = pos( curPos.x, curPos.y - 1 );
            break;
        default:
            cout << "Error situation." << endl;
            break;
        }
        return nextPos;
    }

    boxPos getBoxPosAtPos( vector<boxPos> const id2BoxPos, vector<vector<int>> const pos2Id, pos const curPos ) {
        return id2BoxPos[pos2Id[curPos.x][curPos.y]];
    }

    bool isWALL( vector<vector<int>> const pos2Id, pos point ) {
        return pos2Id[point.x][point.y] == CELLWALL;
    }
    bool isEmpty( vector<vector<int>> const pos2Id, pos point ) {
        return pos2Id[point.x][point.y] == CELLEMPTY;
    }

    bool isBox( vector<vector<int>> const& pos2Id, pos point ) {
        return pos2Id[point.x][point.y] >= 0;
    }

    char getMoveMent() {
        char c;
        while( 1 ) {
            c = getchar();
            if( c == '\033' ) { // Detext escape character
                getchar();     // jump '['
                switch( getchar() ) { // read the third character of direction key
                case 'A': return '^';
                case 'B': return 'v';
                case 'C': return '>';
                case 'D': return '<';
                default: std::cout << "Unknown key" << std::endl; break;
                }
            }
            else if( c == 27 ) { // ESC pressed
                return 0;
            }
            else {
                // std::cout << "Normal key: " << c << std::endl;
                continue;
            }
        }
    }

    void readMap( vector<vector<int>>& arcade, vector<boxPos>& id2BoxPos, pos& start ) {
        ifstream input( "Day15/input.txt" );
        string buf = "\0";
        while( getline( input, buf ) ) {
            vector<int> row;
            for( char c : buf ) {
                if( c != '\n' && c != '\0' ) {
                    switch( c ) {
                    case '#':
                        row.push_back( CELLWALL );
                        row.push_back( CELLWALL );
                        break;
                    case '.':
                        row.push_back( CELLEMPTY );
                        row.push_back( CELLEMPTY );
                        break;
                    case 'O':
                        row.push_back( id2BoxPos.size() );
                        row.push_back( id2BoxPos.size() );
                        id2BoxPos.push_back( boxPos( pos( arcade.size(), row.size() - 2 ), pos( arcade.size(), row.size() - 1 ) ) );
                        break;
                    case '@':
                        start = pos( arcade.size(), row.size() );
                        row.push_back( CELLEMPTY );
                        row.push_back( CELLEMPTY );
                        break;
                    default:
                        break;
                    }
                }
            }
            arcade.push_back( row );
        }
    }

    ll sumCoordinates( vector<boxPos> const id2BoxPos, vector<vector<int>> const acrade ) {
        ll res = 0;
        for( auto boxPos : id2BoxPos ) {
            res += 100 * boxPos.l.x + boxPos.l.y;
        }
        return res;
    }

    int countWall( vector<vector<int>> const acrade ) {
        int wallCnt = 0;
        for( int i = 0; i < acrade.size(); i++ ) {
            for( int j = 0; j < acrade[i].size(); j++ ) {
                if( acrade[i][j] == CELLWALL ) {
                    // cout << "Pos" << i << "," << j << endl;
                    wallCnt++;
                }

            }

        }
        return wallCnt;
    }

    void pushBox( vector<pos>& id2Pos, vector<vector<int>>& pos2Id, pos& curPos, char control ) {
        // printGUI( pos2Id, curPos );
        stack<pos> boxPath;
        pos nextPos = getNextPos( control, curPos );
        while( isBox( pos2Id, nextPos ) ) {
            boxPath.push( nextPos );
            nextPos = getNextPos( control, nextPos );
        }
        if( pos2Id[nextPos.x][nextPos.y] == CELLEMPTY ) {
            while( !boxPath.empty() ) {
                // printGUI( pos2Id, curPos );
                pos preBox = boxPath.top();
                boxPath.pop();
                int preBoxId = pos2Id[preBox.x][preBox.y];
                id2Pos[preBoxId] = nextPos;
                pos2Id[nextPos.x][nextPos.y] = preBoxId;
                nextPos = preBox;
                // printGUI( pos2Id, curPos );
            }
            pos2Id[nextPos.x][nextPos.y] = CELLEMPTY;
            curPos = nextPos;
            // printGUI( pos2Id, curPos );
        }
    }

    void play( vector<pos>& id2Pos, vector<vector<int>>& pos2Id, pos& curPos, queue<char> control ) {
        while( !control.empty() ) {
            char c = control.front();
            control.pop();
            // printGUI( pos2Id, curPos );
            pos nextPos = getNextPos( c, curPos );
            if( pos2Id[nextPos.x][nextPos.y] == CELLWALL ) {
                ;
            }
            else if( pos2Id[nextPos.x][nextPos.y] == CELLEMPTY ) {
                curPos = nextPos;
            }
            else {
                // Push boxes
                pushBox( id2Pos, pos2Id, curPos, c );
                break;
            }

        }
    }

    ll sumCoordinates( vector<pos> const id2Pos, vector<vector<int>> const pos2Id ) {
        ll res = 0;
        for( auto line : pos2Id ) {
            for( auto id : line ) {
                if( id >= 0 )
                    res += 100 * id2Pos[id].x + id2Pos[id].y;
            }
        }
        return res;
    }


    int opId = 0;

    vector<boxPos> getNextBoxVertical( vector<boxPos> const id2BoxPos, vector<vector<int>> const arcade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
        map<int, int> nextLevelID;
        vector<boxPos> nextLevel;
        for( int i = 0; i < curLevel.size() && !isBlocked; i++ ) {
            boxPos curBox = curLevel[i];
            {
                pos l = getNextPos( control, curBox.l ), r = getNextPos( control, curBox.r );
                int nextIDL = arcade[l.x][l.y], nextIDR = arcade[r.x][r.y];
                if( nextIDL == CELLWALL || nextIDR == CELLWALL ) {
                    isBlocked = true;
                    nextLevel.clear();
                    // Unable to move.
                    return nextLevel;
                }
                // Avoid dulplicated eppend.
                if( nextIDL != CELLEMPTY )
                    nextLevelID[nextIDL]++;
                if( nextIDR != CELLEMPTY )
                    nextLevelID[nextIDR]++;
            }
        }
        for( auto m : nextLevelID ) {
            nextLevel.push_back( id2BoxPos[m.first] );
        }
        return nextLevel;
    }

    vector<boxPos> getNextBoxHorizental( vector<boxPos> const id2BoxPos, vector<vector<int>> const  arcade, vector<boxPos> const curLevel, char const control, bool& isBlocked ) {
        assert( curLevel.size() == 1 );
        vector<boxPos> nextLevel;
        boxPos curBox = curLevel[0];
        pos nextBoxPos;
        if( control == '<' ) {
            nextBoxPos = getNextPos( control, curBox.l );
        }
        else {
            nextBoxPos = getNextPos( control, curBox.r );
        }
        if( isBox( arcade, nextBoxPos ) ) {
            nextLevel.push_back( getBoxPosAtPos( id2BoxPos, arcade, nextBoxPos ) );

        }
        else if( isWALL( arcade, nextBoxPos ) ) {
            isBlocked = true;

        }
        else {
            assert( nextLevel.size() == 0 );
        }
        return nextLevel;

    }

    void pushBoxVertical( vector<boxPos> const boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, char const control ) {
        for( boxPos b : boxList ) {
            pos l = b.l, r = b.r;
            int boxId = arcade[l.x][l.y];
            boxPos& curBox = id2BoxPos[boxId];
            curBox.l = getNextPos( control, l );
            curBox.r = getNextPos( control, r );
            arcade[l.x][l.y] = CELLEMPTY;
            arcade[r.x][r.y] = CELLEMPTY;
            arcade[curBox.l.x][curBox.l.y] = boxId;
            arcade[curBox.r.x][curBox.r.y] = boxId;
        }
    }

    void pushBoxHorizontal( vector<boxPos>& boxList, vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, char const control ) {
        assert( boxList.size() == 1 );
        for( boxPos b : boxList ) {
            pos l = b.l, r = b.r;
            int boxId = arcade[l.x][l.y];
            boxPos& curBox = id2BoxPos[boxId];
            curBox.l = getNextPos( control, l );
            curBox.r = getNextPos( control, r );
            if( control == '>' )
                arcade[l.x][l.y] = CELLEMPTY;
            else
                arcade[r.x][r.y] = CELLEMPTY;
            arcade[curBox.l.x][curBox.l.y] = boxId;
            arcade[curBox.r.x][curBox.r.y] = boxId;
        }
    }

    void pushBox( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos, char control ) {
        // if( opId == 2579 )
            // printGUI( arcade, curPos );
        bool isBlocked = false;
        stack<vector<boxPos>> boxPath;
        vector<boxPos> curLevel;
        // NextPos of start
        pos nextPos = getNextPos( control, curPos );
        curLevel.push_back( getBoxPosAtPos( id2BoxPos, arcade, nextPos ) );
        boxPath.push( curLevel );
        while( !isBlocked && !curLevel.empty() ) {
            vector<boxPos> nextLevel;
            if( control == 'v' || control == '^' ) {
                nextLevel = getNextBoxVertical( id2BoxPos, arcade, curLevel, control, isBlocked );
            }
            else {
                nextLevel = getNextBoxHorizental( id2BoxPos, arcade, curLevel, control, isBlocked );

            }
            if( !isBlocked ) {
                if( !nextLevel.empty() )
                    boxPath.push( nextLevel );
                curLevel = nextLevel;
            }
            else {
                nextLevel.clear();
                curLevel.clear();
            }
        }
        if( !isBlocked ) {
            while( !boxPath.empty() ) {
                vector<boxPos> preBoxList = boxPath.top();
                boxPath.pop();
                if( control == '^' || control == 'v' ) {
                    pushBoxVertical( preBoxList, id2BoxPos, arcade, control );
                }
                else {
                    pushBoxHorizontal( preBoxList, id2BoxPos, arcade, control );
                }
            }
            curPos = nextPos;
        }
    }

    bool freshGUI( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos ) {
        char c = getMoveMent();
        if( c == 'q' ) {  // q exit
            return false;
        }
        // printGUI( arcade, curPos );
        pos nextPos = getNextPos( c, curPos );
        if( arcade[nextPos.x][nextPos.y] == CELLWALL ) {
            return true;
        }
        else if( arcade[nextPos.x][nextPos.y] == CELLEMPTY ) {
            curPos = nextPos;
        }
        else {
            // Push boxes
            pushBox( id2BoxPos, arcade, curPos, c );
        }
        return true;
    }

    void play( vector<boxPos>& id2BoxPos, vector<vector<int>>& arcade, pos& curPos, queue<char> control ) {
        // printGUI( arcade, curPos );
        while( !control.empty() ) {
            char c = control.front();
            control.pop();
            // printGUI( arcade, curPos );
            pos nextPos = getNextPos( c, curPos );
            if( arcade[nextPos.x][nextPos.y] == CELLEMPTY ) {
                curPos = nextPos;
            }
            else if( arcade[nextPos.x][nextPos.y] != CELLWALL ) {
                // Push boxes
                pushBox( id2BoxPos, arcade, curPos, c );
            }
            opId++;
            // printGUI( arcade, curPos );
        }
        // printGUI( arcade, curPos );
    }
public:
    void Solution1() {
        vector<pos> id2Pos;
        vector<vector<int>> pos2Id;
        char buf[BUF] = "\0";
        pos start;
        FILE* input = fopen( "Day15/input.txt", "r" );
        while( !feof( input ) && fgets( buf, BUF, input ) ) {
            vector<int> row;
            for( char c : buf ) {
                if( c != '\n' && c != '\0' ) {
                    switch( c ) {
                    case '#':
                        row.push_back( CELLWALL );
                        break;
                    case '.':
                        row.push_back( CELLEMPTY );
                        break;
                    case 'O':
                        row.push_back( id2Pos.size() );
                        id2Pos.push_back( pos( pos2Id.size(), row.size() - 1 ) );
                        break;
                    case '@':
                        row.push_back( CELLEMPTY );
                        start = pos( pos2Id.size(), row.size() - 1 );
                        break;
                    default:
                        break;
                    }
                }
            }
            pos2Id.push_back( row );
        }
        FILE* move = fopen( "Day15/move.txt", "r" );
        queue<char> control;
        while( !feof( move ) && fgets( buf, BUF, move ) ) {
            for( char c : buf ) {
                if( c != '\n' && c != '\0' ) { control.push( c ); }
            }
        }
        play( id2Pos, pos2Id, start, control );
        cout << "Solution 1: " << sumCoordinates( id2Pos, pos2Id ) << endl;
    }


    void Solution2() {
        vector<boxPos> id2BoxPos;
        vector<vector<int>> arcade;
        pos start;
        readMap( arcade, id2BoxPos, start );
        ifstream move( "Day15/move.txt" );
        queue<char> control;
        string buf = "\0";
        while( getline( move, buf ) ) {
            for( char c : buf ) {
                if( c != '\n' && c != '\0' ) { control.push( c ); }
            }
        }
        play( id2BoxPos, arcade, start, control );
        cout << "Solution 2: " << sumCoordinates( id2BoxPos, arcade ) << endl;
    }
};