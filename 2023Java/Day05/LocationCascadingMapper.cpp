#include <bits/stdc++.h>
#include "../util/StringSpliter.h"
using namespace std;
typedef unsigned long long ull;

void readFile( vector<ull>& seedId, vector<vector<pair<pair<ull, ull>, ull>>>& mapperList ) {
    ifstream input( "input.txt" );
    string buf;
    while( getline( input, buf ) ) {
        if( buf.find( "seeds" ) != string::npos ) {
            stringstream ss( split( buf, ":" )[1] );
            ull id;
            while( ss >> id ) {
                seedId.push_back( id );
            }
        }

        if( buf.find( "map" ) != string::npos ) {
            mapperList.push_back( vector<pair<pair<ull, ull>, ull>>() );
            while( getline( input, buf ) ) {
                if( buf.size() <= 1 ) {
                    break;
                }
                vector<string> infoStr = split( buf, "\\s" );
                vector<ull> info( infoStr.size() );
                transform( infoStr.begin(), infoStr.end(), info.begin(), []( string s ) {
                    ull tmp;
                    stringstream( s ) >> tmp;
                    return tmp;
                } );

                pair<pair<ull, ull>, ull> mapInfo( { {info[0],info[1]},info[2] } );

                mapperList.back().push_back( mapInfo );
            }
            // Lets sort here, once for all.
            sort( mapperList.back().begin(), mapperList.back().end(), []( pair<pair<ull, ull>, ull> p1, pair<pair<ull, ull>, ull> p2 ) {
                return p1.first.second < p2.first.second;
            } );
        }
    }
}

typedef pair<ull, ull> Interval;

vector<Interval> SearchInterval( vector<pair<pair<ull, ull>, ull>> const& IntervalMapList, Interval const& itv ) {

    vector<Interval> resList;
    ull itvStart = itv.first, itvEnd = itv.second;

    while( itvStart <= itvEnd ) {
        bool splited = false;
        for( auto mapRange : IntervalMapList ) {

            ull mapStart = mapRange.first.second;
            ull mapEnd = mapStart + mapRange.second - 1;
            ull mapToStart = mapRange.first.first;
            ull mapToEnd = mapToStart + mapRange.second - 1;

            assert( mapToEnd - mapToStart == mapEnd - mapStart );
            if( itvStart >= mapStart && itvStart <= mapEnd ) {
                splited = true;
                if( itvEnd <= mapEnd ) {
                    resList.push_back( Interval( itvStart - mapStart + mapToStart,
                        itvEnd - mapStart + mapToStart ) );

                    return resList;
                } else {

                    resList.push_back( Interval( itvStart - mapStart + mapToStart, mapToEnd ) );
                    itvStart = mapEnd + 1;
                    break;
                }
            } else if( itvEnd >= mapStart && itvEnd <= mapEnd ) {
                splited = true;
                resList.push_back( Interval( mapToStart, itvEnd - itvStart + mapToStart ) );
                itvEnd = mapStart - 1;
                break;
            }

        }
        if( !splited ) {
            resList.push_back( Interval( itvStart, itvEnd ) );
            return resList;
        }

    }
    return resList;
}

void Solution2() {
    ull res = LONG_LONG_MAX;
    vector<ull> seedId;
    vector<vector<pair<pair<ull, ull>, ull>>> mapperList;

    readFile( seedId, mapperList );

    for( int i = 0; i < seedId.size(); i += 2 ) {

        vector<Interval> curLevelList;

        curLevelList.push_back( Interval( seedId[i], seedId[i] + seedId[i + 1] - 1 ) );

        for( auto const& searchList : mapperList ) {
            vector<Interval> nextLevelList;
            for( Interval const& itv : curLevelList ) {
                for( auto nextItv : SearchInterval( searchList, itv ) )
                    nextLevelList.push_back( nextItv );
            }
            curLevelList = move( nextLevelList );
        }

        for_each( curLevelList.begin(), curLevelList.end(), [ & ]( Interval itv ) {
            res = min( itv.first, res );
        } );

    }
    cout << "Solution 2: " + to_string( res ) << endl;

}

int main() {
    Solution2();
}