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


// Follow the Perturbed Leader, Laplacian noise, 0 init (since regret guarantees are for 0 init).

#include "FPL.h"
#include <limits>
#include <set>
#include <math.h>

namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    FPL::FPL( void ) : Exp3G()
    {
        // Should I use the theoretically plausible eta?
        // This requires the horizon to be known. what should we do?
        // _eta = 4 / sqrt(_numOfArms * T);

        // For now, -eta = 0.4
        _eta = 0.4;
        _gamma = 0.;
        _horizon = 100.0;
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    void FPL::initialize( vector< AlphaReal >& vals )
    {
        _time = 0; //time in WISH

        _r_av.resize( _numOfArms ); //average reward vector in WISH
        _Z.resize( _numOfArms );
        _Xhat.resize( _numOfArms );

        // Init to 0
        fill( _r_av.begin(), _r_av.end(), 0.0 );
        fill( _T.begin(), _T.end(), 0.0 );
                
        setInitializedFlagToTrue();
    }

    double FPL::sgn( double x )
    {
        if ( x > 0 ) return 1;
        if ( x < 0 ) return -1;
        return 0;
    }
//----------------------------------------------------------------
//----------------------------------------------------------------

    int FPL::getNextAction()
    {
        AlphaReal U;
        for (int i = 0; i < _numOfArms; i++) {
            // Draw noise Z \in \reals^d from Laplace density eta/2 exp(-eta norm(z))
            U = std::rand() / (AlphaReal) RAND_MAX - 0.5;

            _Z[i] = - sgn(U) / _eta * log(1 - 2 * abs(U));
            _Xhat[i] = _r_av[i] + _Z[i];
        }

        double max_val = *std::max_element(_Xhat.begin(), _Xhat.end());
        double eps = std::numeric_limits<double>::epsilon();

        // Find the set of argmaxes
        vector< AlphaReal > argmaxes; //average reward in WISH
        for (int i = 0; i < _numOfArms; i++) {
            if (std::abs(max_val - _Xhat[i]) <= eps) {
                argmaxes.push_back(i); // i \in \argmax r
            }
        }

        // check for duplicates of argmaxes. If there are dupliates, randomized break
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


    void FPL::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        incIter();
        _time++;

        // Update average reward of arm pulled.
        _r_av[ armNum ] = ((_T[ armNum ] - 1) * _r_av[ armNum ] + reward) / _T[ armNum ]; // Simplified version of above.


    }



} // end namespace MultiBoost
