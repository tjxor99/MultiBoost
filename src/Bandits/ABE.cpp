/*
 *
 *    MultiBoost - Multi-purpose boosting package
 *
 *    Copyright (C)        AppStat group
 *                         Laboratoire de l'Accelerateur Lineaire
 *                         Universite Paris-Sud, 11, CNRS
 *
 *    This file is part of the MultiBoost library
 *
 *    This library is free software; you can redistribute it
 *    and/or modify it under the terms of the GNU General Public
 *    License as published by the Free Software Foundation
 *    version 2.1 of the License.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, 5th Floor, Boston, MA 02110-1301 USA
 *
 *    Contact: : multiboost@googlegroups.com
 *
 *    For more information and up-to-date version, please visit
 *
 *                       http://www.multiboost.org/
 *
 */



#include "ABE.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    ABE::ABE( void ) : Exp3G()
    {
        // _gamma = 0.1;
        _gamma = 0.0;
        _eta = 0.4;
        _horizon = 100.0;

        _Cparam = 0.2;
        _alpha = 0.7; // The exponent of outer time for which exploration occurs (i^\alpha)
    }

//----------------------------------------------------------------
//----------------------------------------------------------------
    void ABE::initLearningOptions(const nor_utils::Args& args) 
    {
        if ( args.hasArgument( "c" ) ){
            _Cparam = args.getValue<AlphaReal>("c", 0 );
        } 
        if ( args.hasArgument( "alpha" ) ){
            _alpha = args.getValue<AlphaReal>("alpha", 0 );
        } 
    }


//----------------------------------------------------------------
//----------------------------------------------------------------

    void ABE::initialize( vector< AlphaReal >& vals )
    {
        _p.resize( _numOfArms );
        _w.resize( _numOfArms );
        _tmpW.resize( _numOfArms );
        _T.resize( _numOfArms );
        _r_av.resize( _numOfArms ); //average reward vector in WISH

        _time = 1; //time in WISH
        _out_time = 1; // outer counter.
        _inner_time = 0; // inner counter.
        _next_arm = -1;


        fill( _p.begin(), _p.end(), 1.0 / _numOfArms );
        fill( _w.begin(), _w.end(), 0.0 );
        fill( _r_av.begin(), _r_av.end(), 0.0 );
        fill( _T.begin(), _T.end(), 0 );
        // fill( _r_av.begin(), _r_av.end(), 1.0 );
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void ABE::updateithValue()
    {
        AlphaReal max = -numeric_limits<AlphaReal>::max();
        for( int i=0; i<_numOfArms; i++ ) 
        {
            _w[i] = _eta * sqrt(_time - 1) * _r_av[i];
            if ( max < _w[i] ) max = _w[i];
        }
        AlphaReal expSum = 0.0;
        
        for( int i=0; i<_numOfArms; i++ ) 
        {
            _tmpW[i] = _w[i] - max;
            expSum += exp( _tmpW[i] );
        }


        for( int i=0; i<_numOfArms; i++ ) 
        {
            _p[i] = exp( _tmpW[i] ) / expSum ;
        }
    }


    int ABE::getNextAction()
    {
        if ( _inner_time < (int)_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) ) { // Using Cparam
            _next_arm += 1;
            _next_arm = _next_arm % _numOfArms;
        }

        else {
            updateithValue();

            vector< AlphaReal > cumsum( getArmNumber()+1 );
            int i;

            cumsum[0] = 0.0;
            for( int i=0; i < getArmNumber(); i++ )
            {
                cumsum[i+1] = cumsum[i] + _p[i];
            }

            for( i=0; i < getArmNumber(); i++ )
            {
                cumsum[i+1] /= cumsum[  getArmNumber() ];
            }

            AlphaReal r = rand() / (AlphaReal) RAND_MAX;

            for( i=0; i < getArmNumber(); i++ )
            {
                if ( (cumsum[i] <= r) && ( r<=cumsum[i+1] )  ) {
                    _next_arm = i;
                    break;
                }
            }

        }

        // std::cout << "Flag: " << flag << std::endl;
        // std::cout << "Avg Reward:\ns";
        // for(int i = 0; i < _numOfArms; i++) {
        //     std::cout << _r_av[i] << std::endl;
        // }
        // std::cout << "Probabilities:\n";
        // for(int i = 0; i < _numOfArms; i++) {
        //     std::cout << _p[i] << std::endl;
        // }

        return _next_arm;
    }

    void ABE::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        incIter();
        _time++;
        _inner_time++;

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ]; // Simplified version of above.

        if (_inner_time >= (int)(_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) + _numOfArms * pow((double)2.0, _out_time)))
        {
            _inner_time = 0;
            _out_time++;
        }


    }



} // end namespace MultiBoost
