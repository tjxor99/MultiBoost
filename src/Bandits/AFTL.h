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



#ifndef _AFTL_H
#define _AFTL_H

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


    class AFTL : public Exp3G
    {
    protected:
        double _horizon; //horizon
        double _time; //time in WISH

        double _out_time; // i counter in ABE. Set as double to put into pow
        int _inner_time;

        double _alpha; // exploration exponent
        double _Cparam;

        vector< AlphaReal > _r_av; //average reward in WISH

        int _next_arm;

    public:
        AFTL(void);
        virtual ~AFTL(void)
        {
        }
        virtual int getNextAction();

        virtual void receiveReward( int armNum, AlphaReal reward );
        virtual void initialize( vector< AlphaReal >& vals );
    };


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


} // end of namespace MultiBoost

#endif

