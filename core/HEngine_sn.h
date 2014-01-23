/**
 * @author VaL Doroshchuk
 * @copyright Alex X. Liu, Ke Shen, Eric Torng, VaL Doroshchuk
 * @license GNU GPL v2
 */


/**
 * Non recursive static implementation of HEngine
 */

#ifndef HENGINE_SN_H
#define HENGINE_SN_H

#include "HEngine_s.h"

namespace hengine
{

class HEngine_sn: public HEngine_s
{

protected:
    void build();

public:
    HEngine_sn( Table db, unsigned k, unsigned r = 0 ): HEngine_s( db, k, r ) { build();}
    Table query( BinStr ) const { return Table(); }
    Table query( Number ) const { return Table(); }
    Table permute( const BinStr& ) const;
};

} // namespace

#endif