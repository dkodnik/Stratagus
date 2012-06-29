//       _________ __                 __
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/
//  ______________________                           ______________________
//                        T H E   W A R   B E G I N S
//         Stratagus - A free fantasy real time strategy game engine
//
/**@name unit.h - The unit headerfile. */
//
//      (c) Copyright 1998-2012 by Lutz Sammer, Jimmy Salmon and Joris Dauphin
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//

#ifndef __UNIT_FIND_H__
#define __UNIT_FIND_H__

//@{

#include "unit.h"
#include "unittype.h"

class CUnitCache;
class CMapField;

/*----------------------------------------------------------------------------
--  Declarations
----------------------------------------------------------------------------*/

//unit_find
struct CUnitTypeFinder {
	const UnitTypeType type;
	CUnitTypeFinder(const UnitTypeType t) : type(t)  {}
	bool operator()(const CUnit *const unit) const {
		const CUnitType *const t = unit->Type;
		if (t->Vanishes || (type != static_cast<UnitTypeType>(-1) && t->UnitType != type)) {
			return false;
		}
		return true;
	}

	CUnit *Find(const CUnitCache &cache) const { return cache.find(*this); }
	CUnit *Find(const CMapField *const mf) const { return mf->UnitCache.find(*this); }
};

struct CResourceFinder {
	const int resource;
	const int mine_on_top;
	CResourceFinder(const int r, int on_top) : resource(r), mine_on_top(on_top) {}
	inline bool operator()(const CUnit *const unit) const {
		const CUnitType *const type = unit->Type;
		return (type->GivesResource == resource
				&& unit->ResourcesHeld != 0
				&& (mine_on_top ? type->CanHarvest : !type->CanHarvest)
				&& !unit->IsUnusable(true) //allow mines under construction
			   );
	}
	CUnit *Find(const CMapField *const mf) const { return mf->UnitCache.find(*this); }
};

/// Find resource
extern CUnit *UnitFindResource(const CUnit &unit, const CUnit &startUnit, int range,
							   int resource, bool check_usage = false, const CUnit *deposit = NULL);

/// Find nearest deposit
extern CUnit *FindDeposit(const CUnit &unit, int range, int resource);
/// Find the next idle worker
extern CUnit *FindIdleWorker(const CPlayer &player, const CUnit *last);

/// Find the neareast piece of terrain with specific flags.
extern bool FindTerrainType(int movemask, int resmask, int range,
							const CPlayer &player, const Vec2i &startPos, Vec2i *pos);


// in unit_find.cpp

extern void FindUnitsByType(const CUnitType &type, std::vector<CUnit *> &units);

/// Find all units of this type of the player
extern void FindPlayerUnitsByType(const CPlayer &player, const CUnitType &type, std::vector<CUnit *> &units);
/// Return any unit on that map tile
extern CUnit *UnitOnMapTile(const Vec2i &pos, unsigned int type);// = -1);
/// Return possible attack target on that map area
extern CUnit *TargetOnMap(const CUnit &unit, const Vec2i &pos1, const Vec2i &pos2);

/// Return resource, if on map tile
extern CUnit *ResourceOnMap(const Vec2i &pos, int resource, bool mine_on_top = true);
/// Return resource deposit, if on map tile
extern CUnit *ResourceDepositOnMap(const Vec2i &pos, int resource);

/// Find best enemy in numeric range to attack
extern CUnit *AttackUnitsInDistance(const CUnit &unit, int range, bool onlyBuildings = false);
/// Find best enemy in attack range to attack
extern CUnit *AttackUnitsInRange(const CUnit &unit);
/// Find best enemy in reaction range to attack
extern CUnit *AttackUnitsInReactRange(const CUnit &unit);

//@}

#endif // !__UNIT_FIND_H__
