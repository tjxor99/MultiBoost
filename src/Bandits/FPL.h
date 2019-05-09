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

// Follow the Perturbed Leader (FPL)



#ifndef _FPL_H
#define _FPL_H

#include <list>
#include <functional>
#include <math.h> //for pow
#include "GenericBanditAlgorithm.h"
#include "Utils/Utils.h"
#include "Exp3G.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

namespace MultiBoost {


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


    class FPL : public Exp3G
    {
    protected:
        double _horizon; //horizon
        double _time; //time in WISH

        AlphaReal _eta;
        AlphaReal _gamma;

        vector< AlphaReal > _r_av; //average reward in WISH
        vector< AlphaReal > _Z; // Laplacian noise in FPL
        vector< AlphaReal > _Xhat; // _r_av[i] + Z[i]

        int _next_arm;

    public:
        FPL(void);
        virtual ~FPL(void)
        {
        }
        virtual int getNextAction();
        virtual double sgn( double x );

        virtual void receiveReward( int armNum, AlphaReal reward );
        virtual void initialize( vector< AlphaReal >& vals );
    };


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


} // end of namespace MultiBoost

#endif

