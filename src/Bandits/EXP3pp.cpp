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

#include "EXP3pp.h" // EXP3++
#include <limits>
#include <set>
#include <math.h>


namespace MultiBoost {
//----------------------------------------------------------------
//----------------------------------------------------------------

    EXP3pp::EXP3pp( void ) : Exp3G()
    {
      _eta = 0.4;
      _c = 1. / 32;
    }


//----------------------------------------------------------------
//----------------------------------------------------------------

//----------------------------------------------------------------
//----------------------------------------------------------------

    void EXP3pp::initialize( vector< AlphaReal >& vals )
    {
      _T.resize( _numOfArms );
      _G.resize( _numOfArms );
      _p.resize( _numOfArms );
      _w.resize( _numOfArms );
      _tmpW.resize( _numOfArms );
      _time = 1;

      //one pull for all arm
      fill( _T.begin(), _T.end(), 0 );
      fill( _p.begin(), _p.end(), 1.0 / _numOfArms );
      fill( _w.begin(), _w.end(), 0.0 );
      fill( _G.begin(), _G.end(), 0.0 );


      setInitializedFlagToTrue();
    }

    void EXP3pp::updateithValue( )
    {
      double beta, estimate;
      vector <AlphaReal> eps(_numOfArms);
      vector <AlphaReal> zeta(_numOfArms);

      beta = 0.5 * sqrt(log(_numOfArms) / _time / _numOfArms);
      if (beta > 0.5 / _numOfArms) {beta = 0.5 / _numOfArms;}

      double max_val = *std::max_element(_G.begin(), _G.end());
      for (int i = 0; i < _numOfArms; i++) {
        eps[i] = beta; // This is already the min over beta and the other term.

        estimate = 1. / _time * (max_val - _G[i]);
        if (estimate > 1) {
          estimate = 1;
        }
        if (estimate > 0){ // If estimate == 0, eps[i] != zeta[i]
          zeta[i] = _c * pow(log(_time * estimate), 2) / _time / pow(estimate, 2);

          if (eps[i] > zeta[i]) {
            eps[i] = zeta[i];
          }          
        }
      }

      AlphaReal max = -numeric_limits<AlphaReal>::max();
      for( int i=0; i<_numOfArms; i++ ) 
      {
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
          _p[i] = ( 1 - eps[i] ) * ( exp( _tmpW[i] ) / expSum ) + ( eps[i] / (AlphaReal)getIterNum() );
      }
    }

//----------------------------------------------------------------
//----------------------------------------------------------------
    void EXP3pp::receiveReward( int armNum, AlphaReal reward )
    {
      _time++;
      _T[ armNum ]++;

      _eta = 0.5 * sqrt(log(_numOfArms) / _time / _numOfArms);
      _G[ armNum ] += reward / _p[armNum];
      _w[ armNum ] += _eta * _G[ armNum ];
      updateithValue();
    }


} // end namespace MultiBoost

