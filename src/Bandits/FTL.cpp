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


// TODO: Verify for correctness!

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
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void FTL::initialize( vector< AlphaReal >& vals )
    {
        _time = 0; //time in WISH

        _r_av.resize( _numOfArms ); //average reward vector in WISH
        _next_arm = -1;


        // Init to 1
        fill( _r_av.begin(), _r_av.end(), 1.0 );
        fill( _T.begin(), _T.end(), 1.0 );

        // fill( _r_av.begin(), _r_av.end(), 0.0 );
        // fill( _T.begin(), _T.end(), 0 );
        
        //one pull for all arm
        
        setInitializedFlagToTrue();
    }
//----------------------------------------------------------------
//----------------------------------------------------------------

    int FTL::getNextAction()
    {
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

        return _next_arm;
    }


    void FTL::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        incIter();
        _time++;

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1)*_r_av[ armNum ] + reward) / _T[ armNum ]; // Simplified version of above.


    }



} // end namespace MultiBoost
