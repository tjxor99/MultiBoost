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



#include "Gumbel.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    Gumbel::Gumbel( void ) : Exp3G()
    {
        _Cparam = 0.25; // Assume logedge s.t. rewards are in [0, 1]
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void Gumbel::initialize( vector< AlphaReal >& vals )
    {
        _p.resize( _numOfArms );
        _w.resize( _numOfArms );
        _tmpW.resize( _numOfArms );
        _r_av.resize( _numOfArms ); //average reward vector in WISH

        _Cparam = 0.25;



        fill( _p.begin(), _p.end(), 0.0 );
        fill( _w.begin(), _w.end(), -10.0 );

        fill( _r_av.begin(), _r_av.end(), 0.0 );

        copy( vals.begin(), vals.end(), _X.begin() );
        
        fill( _T.begin(), _T.end(), 0 );
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------
    int Gumbel::getNextAction()
    {
        AlphaReal U = rand() / (AlphaReal) RAND_MAX; // Uniform Random Variable
        AlphaReal Z;

        // Standard Gumbel Random Variable Z = -ln(-ln(U)) where U is a standard Uniform RV. See reference below.
        // https://ncss-wpengine.netdna-ssl.com/wp-content/themes/ncss/pdf/Procedures/NCSS/Data_Simulation.pdf
        // http://www.randomservices.org/random/special/ExtremeValue.html.
        Z = -log(-log(U)); 
        AlphaReal maxval = _r_av[0] + _Cparam * sqrt(1. / _T[0]) * Z;
        int maxarg = 0;

        AlphaReal tmp;
        for(int i = 1; i < _numOfArms; i++) {
            Z = -log(-log(U)); 
            tmp = _r_av[i] + _Cparam * sqrt(1. / _T[i]) * Z;
            if (maxval < tmp) {
                maxval = tmp;
                maxarg = i;
            }
        }
        // cout << maxarg << std::endl; // Using --verbose 3, I can verify that the arm being selected is the same as "Column has been selected: %"

        return maxarg;
    }

    // void Gumbel::updateithValue( int arm )
    // {
    //     //double sum = 0.0;
    //     AlphaReal max = -numeric_limits<AlphaReal>::max();
    //     for( int i=0; i<_numOfArms; i++ ) 
    //     {
    //         //sum += _w[i];
    //         if ( max < _w[i] ) max = _w[i];
    //     }
    //     //double mean = sum / ( double ) _numOfArms;
    //     AlphaReal expSum = 0.0;
        
    //     for( int i=0; i<_numOfArms; i++ ) 
    //     {
    //         _tmpW[i] = _w[i] - max;
    //         expSum += exp( _tmpW[i] );
    //     }


    //     for( int i=0; i<_numOfArms; i++ ) 
    //     {
    //         //_p[i] = ( 1 - _gamma ) * exp( _w[i] / sum ) + ( _gamma / (double)getIterNum() );
    //         _p[i] = exp( _tmpW[i] ) / expSum ;
    //     }
    // }

//----------------------------------------------------------------
//----------------------------------------------------------------

    void Gumbel::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;

        incIter();

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ];
        // Gumbel::updateithValue( armNum ); // Verified that updateithValue just has to be called once!

    }



} // end namespace MultiBoost
