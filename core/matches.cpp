/**
 * @author VaL Doroshchuk
 * @copyright Alex X. Liu, Ke Shen, Eric Torng, VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <list>

#include "HEngine_sn.h"

using namespace hengine;
/*
//typedef std::unordered_set<Number> NumList;
typedef std::list<Number> NumList;

bool sortBinCmp( const BinStr &i, const BinStr &j )
{
    return ( i < j );
}

bool sortNumCmp( const Number &i, const Number &j )
{
    return ( i < j );
}

void sortBinTable( BinTable &table )
{
    sort( table.begin(), table.end(), sortBinCmp );
}

void sortNumTable( NumTable &table )
{
    sort( table.begin(), table.end(), sortNumCmp );
}
*/

int main( int argc, char **argv )
{
    float  userTime;
    struct rusage startTime, stopTime;

    getrusage( RUSAGE_SELF, &startTime );

    if ( argc != 4 )
    {
        std::cout << "Usage: " << argv[0] << " <k> <data file> <query file>" << std::endl << std::endl;

        return 1;
    }

    unsigned k = atoi( argv[1] );
    std::string line;

    std::cout << "Reading the dataset ........ ";
    fflush( stdout );

    NumTable db;

    std::ifstream dict;
    dict.open( argv[2], std::ifstream::in );
    while ( getline( dict, line ) )
    {
        Number h;

        std::istringstream reader( line );
        reader >> h;
        db.push_back( h );
    }

    NumTable q;

    std::ifstream qdict;
    qdict.open( argv[3], std::ifstream::in );
    while ( getline( qdict, line ) )
    {
        Number h;

        std::istringstream reader( line );
        reader >> h;
        q.push_back( h );
    }

    std::cout <<  "done. " << db.size() << " db hashes and " << q.size() << " query hashes." << std::endl;
    std::cout << "Building with " << k << " hamming distance bound ....... ";
    fflush( stdout );

    HEngine_sn e( db, k );

    std::cout << "done." << std::endl;


    getrusage( RUSAGE_SELF, &stopTime );
    userTime =
                ( (float) ( stopTime.ru_utime.tv_sec  - startTime.ru_utime.tv_sec ) ) +
                ( (float) ( stopTime.ru_utime.tv_usec - startTime.ru_utime.tv_usec ) ) * 1e-6;

    std::cout << std::endl;
    std::cout << "Building time: " << userTime << " seconds" << std::endl;
    std::cout << std::endl;

    std::cout << "Searching HEngine matches ......." << std::endl;

    getrusage( RUSAGE_SELF, &startTime );
    int c = 0;
    for ( auto &h: q )
    {
        Matches res = e.query( h );
        c += res.size();
        /*for ( auto &d : res )
        {
            //std::cout << "[" << d.second << "] "<< std::endl;// << HEngine::binStr2Number( h ) << " -> " << HEngine::binStr2Number( d.first ) << std::endl;
            c++;
        }*/

    }

    getrusage( RUSAGE_SELF, &stopTime );
    userTime =
                ( (float) ( stopTime.ru_utime.tv_sec  - startTime.ru_utime.tv_sec ) ) +
                ( (float) ( stopTime.ru_utime.tv_usec - startTime.ru_utime.tv_usec ) ) * 1e-6;

    std::cout << "found " << c << " total matches, Query time: " << userTime << " seconds" << std::endl << std::endl;
    std::cout << std::endl;


    std::cout << "Searching linear matches ......." << std::endl;
    getrusage( RUSAGE_SELF, &startTime );
    c = 0;
    for ( auto &item: db )
    {
        for ( auto &h: q )
        {
            unsigned d = HEngine::getHammingDistance( h, item );
            if ( d <= k )
            {
                c++;
            }
        }
    }
    getrusage( RUSAGE_SELF, &stopTime );
    userTime =
                ( (float) ( stopTime.ru_utime.tv_sec  - startTime.ru_utime.tv_sec ) ) +
                ( (float) ( stopTime.ru_utime.tv_usec - startTime.ru_utime.tv_usec ) ) * 1e-6;

    std::cout << "found " << c  << " total matches. Linear query time: " << userTime << " seconds" << std::endl << std::endl;
    std::cout << std::endl;

    return 0;
}
