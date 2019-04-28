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

        _Cparam = 1.0;
        _alpha = 0.3; // The exponent of outer time for which exploration occurs (i^\alpha)
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void ABE::initialize( vector< AlphaReal >& vals )
    {
        _p.resize( _numOfArms );
        _w.resize( _numOfArms );
        _tmpW.resize( _numOfArms );
        _time = 0; //time in WISH
        _out_time = 1; // outer counter.
        _inner_time = 0; // inner counter.
        _r_av.resize( _numOfArms ); //average reward vector in WISH



        fill( _p.begin(), _p.end(), 0.0 );
        fill( _w.begin(), _w.end(), -10.0 );
        _p[0] = 1.0; // Initially start with first arm being picked for exploration in ABE.
        _w[0] = 1.0;


        // fill( _p.begin(), _p.end(), 1.0 / _numOfArms );
        // fill( _w.begin(), _w.end(), 1.0 );
        fill( _r_av.begin(), _r_av.end(), 0.0 );

        copy( vals.begin(), vals.end(), _X.begin() );
        
        //one pull for all arm
        fill( _T.begin(), _T.end(), 0 );
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void ABE::updateithValue( int arm )
    {
        //double sum = 0.0;
        AlphaReal max = -numeric_limits<AlphaReal>::max();
        for( int i=0; i<_numOfArms; i++ ) 
        {
            //sum += _w[i];
            if ( max < _w[i] ) max = _w[i];
        }
        //double mean = sum / ( double ) _numOfArms;
        AlphaReal expSum = 0.0;
        
        for( int i=0; i<_numOfArms; i++ ) 
        {
            _tmpW[i] = _w[i] - max;
            expSum += exp( _tmpW[i] );
        }


        for( int i=0; i<_numOfArms; i++ ) 
        {
            //_p[i] = ( 1 - _gamma ) * exp( _w[i] / sum ) + ( _gamma / (double)getIterNum() );
            _p[i] = exp( _tmpW[i] ) / expSum ;
        }
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

        // Explore
        if ( _inner_time < (int)_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) ) { // Using Cparam
            int next_arm;

            if (armNum == (_numOfArms - 1)) { 
                // Inner loop iterated over K arms. Go back to arm 0.
                next_arm = 0;
            }
            else { 
                // Explore next arm.
                next_arm = armNum + 1;
            }

            fill( _w.begin(), _w.end(), -10.0 );
            _w[ next_arm ] = 1;
        }

        else {
            if (_inner_time >= (int)(_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) + _numOfArms * pow((double)2.0, _out_time)))
            {
                fill( _w.begin(), _w.end(), -10.0 );
                _w[0] = 1;
                _inner_time = 0;
                _out_time++;
            }

            else {
                // Exploit in next turn.
                for (int i = 0; i <_numOfArms; i++) {
                    _w[i] = _eta * sqrt(_time - 1) * _r_av[i];
                }                
            }
        }


        updateithValue( armNum ); // Verified that updateithValue just has to be called once!

        std::cout << "Reward Vector \n";
        for (int i = 0; i < _numOfArms; i++) {
            std::cout << _r_av[i] << '\n';
        }

    }



} // end namespace MultiBoost
