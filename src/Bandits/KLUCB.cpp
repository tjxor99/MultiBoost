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

#include "KLUCB.h"
#include <limits>
#include <set>
#include <math.h>


namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------


    KLUCB::KLUCB( void ) : Exp3G()
    {
      c = 0;
      delta = 1e-2; // tolerance for finding q
    }


//----------------------------------------------------------------
//----------------------------------------------------------------


    double KLdiv(double p, double q) {
      return p * log(p / q) + (1 - p) * log((1 - p) / (1 - q));
    }

    double findq(double S, double T, double UB) {
      double q, LHS;
      double delta = 1e-2;

      q = 1;
      LHS = T * KLdiv(S / T, q);
      while (LHS > UB) {
        q -= delta;
        LHS = T * KLdiv(S / T, q);
      }
      return q;

      // double q;
      // double LHS;

      // // Newton's method.
      // q = p + delta;

      // for (int i = 0; i < 20; i++) {
      //   next_q = q + (q - p) * q * (q - p) / (p + pow(q, 2) - 2 * p * q);
      //   LHS = _T[i] * KLdiv(p, next_q);
      //   if (LHS <= UB) {
      //     q = next_q;
      //   }
      //   else {
      //     break;
      //   }

      //   if ()
      // }

      // return q;
    }

    int KLUCB::getNextAction()
    {
      vector <AlphaReal> q(_numOfArms);
      AlphaReal UB;
      AlphaReal testval;

      UB = log(_time) + c * log(log(_time));
      for (int i = 0; i < _numOfArms; i++) {
        if (_T[i] == 0) {return i;} // Unexplored arm.

        q[i] = findq(_S[i], _T[i], UB);
      }

      int _next_arm;
      double max_val = *std::max_element(q.begin(), q.end());
      double eps = std::numeric_limits<double>::epsilon();

      vector< AlphaReal > argmaxes; //average reward in WISH
      for (int i = 0; i < _numOfArms; i++) {
          if (std::abs(max_val - q[i]) <= eps) {
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



//----------------------------------------------------------------
//----------------------------------------------------------------

    void KLUCB::initialize( vector< AlphaReal >& vals )
    {
        // //copy the initial values to X
        // copy( vals.begin(), vals.end(), _X.begin() );
      _T.resize( _numOfArms );
      _S.resize( _numOfArms );

      _time = 1;
        //one pull for all arm
        fill( _T.begin(), _T.end(), 0 );
        fill(_S.begin(), _S.end(), 0);

        setInitializedFlagToTrue();
    }

    // void Exp3G::updateithValue( int arm )
    // {
    //     //double sum = 0.0;
    //     AlphaReal max = -numeric_limiKLUCB<AlphaReal>::max();
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
    void KLUCB::receiveReward( int armNum, AlphaReal reward )
    {
      _time++;
      _T[ armNum ]++;
      _S[ armNum ] += reward;
      // calculate the feedback value


      // std::cout << "Number of Arms! " << _numOfArms << std::endl;

      // std::cout << "T: " << std::endl;
      // for (int i = 0; i < _numOfArms; i++) {
      //   std::cout << _T[i] << std::endl;
      // }
      // std::cout << "T End!" << std::endl;

      // std::cout << "S: " << std::endl;
      // for (int i = 0; i < _numOfArms; i++) {
      //   std::cout << _S[i] << std::endl;
      // }
      // std::cout << "S End!" << std::endl;
    }


} // end namespace MultiBoost

