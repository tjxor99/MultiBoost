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


#include "AFTL.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    AFTL::AFTL( void ) : Exp3G()
    {
        _horizon = 100.0;

        _Cparam = 0.5;
        // _Cparam = 0.1;
        _alpha = 0.7; // The exponent of outer time for which exploration occurs (i^\alpha)
    }


    void AFTL::initLearningOptions(const nor_utils::Args& args) 
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

    void AFTL::initialize( vector< AlphaReal >& vals )
    {
        _T.resize( _numOfArms );
        fill( _T.begin(), _T.end(), 0 );
        // _p.resize( _numOfArms );
        // _w.resize( _numOfArms );
        // _tmpW.resize( _numOfArms );
        _time = 0; //time in WISH
        _inner_time = 0;
        _out_time = 1;

        _r_av.resize( _numOfArms ); //average reward vector in WISH
        _next_arm = -1;



        // fill( _p.begin(), _p.end(), 0 );
        // fill( _w.begin(), _w.end(), -10.0 );
        // _p[0] = 1.0;
        // _w[0] = 1.0;


        // fill( _r_av.begin(), _r_av.end(), 0.0 );
        fill( _r_av.begin(), _r_av.end(), 1.0 );

        // copy( vals.begin(), vals.end(), _X.begin() );
        
        //one pull for all arm
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------

    int AFTL::getNextAction()
    {
        // Explore
        if ( _inner_time < (int)_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) ) { // Using Cparam
            _next_arm += 1;
            _next_arm = _next_arm % _numOfArms;
        }

        // Exploit
        else {                
            double max_val = *std::max_element(_r_av.begin(), _r_av.end());
            double eps = std::numeric_limits<double>::epsilon();

            vector< AlphaReal > argmaxes; //average reward in WISH
            for (int i = 0; i < _numOfArms; i++) {
                if (std::abs(max_val - _r_av[i]) <= eps) {
                    argmaxes.push_back(i); // i \in \argmax r
                }
            }
            int len = argmaxes.size();

            AlphaReal rand_val = rand() / (AlphaReal) RAND_MAX;
            AlphaReal interval = 1. / (AlphaReal)len;
            AlphaReal lb = 0;
            AlphaReal ub = interval;
            for (int i = 0; i < len; i++) {
                if ((rand_val >= lb) && (rand_val < ub)) {
                    _next_arm = argmaxes[i];
                    break;
                }
                lb += interval;
                ub += interval;
            }
        }

        return _next_arm;
    }


    void AFTL::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        incIter();
        _time++;
        _inner_time++;

        if (_inner_time >= (int)(_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) + _numOfArms * pow((double)2.0, _out_time))) {
            _out_time++;
        }

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ];

    }



} // end namespace MultiBoost
