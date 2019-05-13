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



#ifndef _TS_H
#define _TS_H

// #include "beta.h"
#include <list> 
#include <functional>
#include <math.h> //for pow
// #include <random>

#include "GenericBanditAlgorithm.h"
#include "Utils/Utils.h"
#include "Exp3G.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

namespace MultiBoost {


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


    class TS : public Exp3G
    {
    protected:
        double _horizon; //horizon
        vector <int> _alpha;
        vector <int> _beta;


    public:
        TS(void);
        virtual ~TS(void) 
        {
        }

        virtual void receiveReward( int armNum, AlphaReal reward );
        virtual void initialize( vector< AlphaReal >& vals );

        // virtual void betaRV(AlphaReal alpha, AlphaReal beta);
        virtual int getNextAction();
    };


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


} // end of namespace MultiBoost

#endif

