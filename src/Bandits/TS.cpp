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

// #include "beta.h"
#include "TS.h"
#include <limits>
#include <set>
#include <math.h>
// #include <random>

// std::random_device rd; // use to seed the rng 
// std::mt19937 rng{rd()}; // rng


namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    TS::TS( void ) //: Exp3G()
    {
        // _gamma = 0.0;
        // _eta = 0.5;
    }



//----------------------------------------------------------------
//----------------------------------------------------------------

    double betaRV(int alpha, int beta) {
        // Generating beta RV with Uniform RVs
        AlphaReal X, Y;

        AlphaReal U;

        X = 0; // ~ Gamma(alpha, 1)
        for (int i = 0; i < alpha; i++) {
            U = rand() / (AlphaReal) RAND_MAX;
            X = X - log(U);
        }

        for (int i = 0; i < beta; i++) {
            U = rand() / (AlphaReal) RAND_MAX;
            Y = Y - log(U);
        }

        return X / (X + Y);
    }


    int TS::getNextAction()
    {
        vector <AlphaReal> theta(_numOfArms, 0.0);
        // theta.resize( _numOfArms );

        for (int i = 0; i < _numOfArms; i++) {
            theta[i] = betaRV(_alpha[i], _beta[i]);
            // theta[i] = bandit::beta_distribution<double>(_alpha[i], _beta[i])(rng);
        }

        // Next action is argmax theta
        int next_arm = *std::max_element(theta.begin(), theta.end());
        std::cout << "Next Arm: " << next_arm << std::endl;


        // double max_val = *std::max_element(theta.begin(), theta.end());
        // double eps = std::numeric_limits<double>::epsilon();
        // int next_arm;

        // for (int i = 0; i < _numOfArms; i++) {
        //     if (std::abs(max_val - theta[i]) <= eps) {
        //         next_arm = i;
        //         break;
        //     }
        // }

        return next_arm;
    }

//----------------------------------------------------------------
//----------------------------------------------------------------

    void TS::initialize( vector< AlphaReal >& vals )
    {
        // //copy the initial values to X
        // copy( vals.begin(), vals.end(), _X.begin() );

        //one pull for all arm
        fill( _T.begin(), _T.end(), 1 );

        fill(_alpha.begin(), _alpha.end(), 1);
        fill(_beta.begin(), _beta.end(), 1);

        setInitializedFlagToTrue();
    }

    // void Exp3G::updateithValue( int arm )
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
    //         _p[i] = ( 1 - _gamma ) * ( exp( _tmpW[i] ) / expSum ) + ( _gamma / (AlphaReal)getIterNum() );
    //     }
    // }

//----------------------------------------------------------------
//----------------------------------------------------------------
    void TS::receiveReward( int armNum, AlphaReal reward )
    {
        _T[ armNum ]++;
        // calculate the feedback value

        AlphaReal rand_val = rand() / (AlphaReal) RAND_MAX;
        if (rand_val >= reward) {
            _alpha[ armNum ] += 1;
        }
        else {
            _beta[ armNum ] += 1;
        }
        incIter();

        // std::bernoulli_distribution d(reward);
        // bool success = d(rng);

        // if (success) {
        //     _alpha[ armNum ] += 1;
        // }

        // else {
        //     _beta[ armNum ] += 1;
        // }

    }


} // end namespace MultiBoost

