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



#include "FTL.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    FTL::FTL( void ) : Exp3G()
    {
        _horizon = 100.0;

        _Cparam = 1;
        // _Cparam = 0.1;
        _alpha = 0.5; // The exponent of outer time for which exploration occurs (i^\alpha)
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void FTL::initialize( vector< AlphaReal >& vals )
    {
        // _p.resize( _numOfArms );
        // _w.resize( _numOfArms );
        // _tmpW.resize( _numOfArms );
        _time = 0; //time in WISH
        _inner_time = 0;
        _out_time = 1;

        _r_av.resize( _numOfArms ); //average reward vector in WISH
        _next_arm = 0;



        // fill( _p.begin(), _p.end(), 0 );
        // fill( _w.begin(), _w.end(), -10.0 );
        // _p[0] = 1.0;
        // _w[0] = 1.0;


        fill( _r_av.begin(), _r_av.end(), 0.0 );

        // copy( vals.begin(), vals.end(), _X.begin() );
        
        //one pull for all arm
        fill( _T.begin(), _T.end(), 0 );
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------

    int FTL::getNextAction()
    {
        return _next_arm;
    }


    void FTL::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        // int flag;


        incIter();
        _time++;
        _inner_time++;

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ]; // Simplified version of above.

        // Explore
        if ( _inner_time < (int)_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) ) { // Using Cparam
            // int next_arm;
            // flag = 0;

            if (armNum == (_numOfArms - 1)) { 
                // Inner loop iterated over K arms. Go back to arm 0.
                _next_arm = 0;
            }
            else { 
                // Explore next arm.
                _next_arm = armNum + 1;
            }
        }

        else {
            if (_inner_time >= (int)(_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) + _numOfArms * pow((double)2.0, _out_time)))
            {
                // Reset inner time and increment outer time.
                _inner_time = 0;
                _out_time++;

                // Determine whether the next is pure exploration phase or FTL phase
		        if ( _inner_time < (int)_numOfArms * floor(_Cparam * pow(_out_time, _alpha)) ) { // Using Cparam
		        	// flag = 0;
	                _next_arm = 0;
		        }
		        else { // FTL
		        	// flag = 1;
	                _next_arm = 0;
	                double max_val = _r_av[0];
	                for (int i = 1; i < _numOfArms; i++) {
	                    if (max_val < _r_av[i]) {
	                        max_val = _r_av[i];
	                        _next_arm = i;
	                    }
	                }	        	
		        }

            }

            else {
            	// flag = 1;
                // Follow the leader
                _next_arm = 0;
                double max_val = _r_av[0];
                for (int i = 1; i < _numOfArms; i++) {
                    if (max_val < _r_av[i]) {
                        max_val = _r_av[i];
                        _next_arm = i;
                    }
                }

            }
        }

    }



} // end namespace MultiBoost
