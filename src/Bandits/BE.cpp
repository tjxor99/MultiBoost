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



#include "BE.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    BE::BE( void ) : Exp3G()
    {
        // _gamma = 0.1;
        _gamma = 0.0;
        _eta = 0.4;
        _horizon = 100.0;
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void BE::initialize( vector< AlphaReal >& vals )
    {
        _p.resize( _numOfArms );
        _w.resize( _numOfArms );
        _tmpW.resize( _numOfArms );
        _time = 0; //time in WISH
        _r_av.resize( _numOfArms ); //average reward vector in WISH


        fill( _p.begin(), _p.end(), 1.0 / _numOfArms );
        fill( _w.begin(), _w.end(), 0.0 );

        // init to 0
        // fill( _r_av.begin(), _r_av.end(), 0.0 );
        // fill( _T.begin(), _T.end(), 0 );

        // Init to 1
        fill( _r_av.begin(), _r_av.end(), 1.0 );
        fill( _T.begin(), _T.end(), 1.0 );

        copy( vals.begin(), vals.end(), _X.begin() );
        
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    void BE::updateithValue( int arm )
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


    void BE::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        incIter();
        _time++;

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ]; // Simplified version of above.
        for (int i = 0; i <_numOfArms; i++) {
            _w[i] = _eta * sqrt(_time - 1) * _r_av[i];
        }                


        updateithValue( armNum ); // Verified that updateithValue just has to be called once!
    }



} // end namespace MultiBoost
