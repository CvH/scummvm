/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#define XEEN_MAZE_SOURCE

#include "xeen/game.h"

#include "xeen/maze/objects_.h"
#include "xeen/maze/segment_.h"
#include "xeen/maze/manager.h"

static const uint32 OFF_WALLS         = 0x000;
static const uint32 OFF_CELL_FLAGS    = 0x200;
static const uint32 OFF_SURR_MAZES    = 0x302;
static const uint32 OFF_MAP_FLAGS     = 0x30A;
static const uint32 OFF_WALL_TYPES    = 0x30E;
static const uint32 OFF_SURFACE_TYPES = 0x31E;
static const uint32 OFF_VALUES        = 0x32E;

XEEN::Maze::Segment::Segment(Valid<Manager> parent, FilePtr data) : _parent(parent), _data(data)
{
    memset(_surrMazes, 0, sizeof(_surrMazes));

    if(!_data)
    {
        _data.reset();
        markInvalid("MAZE file not found"); // TODO: Print name
    }
}

void XEEN::Maze::Segment::loadSurrounding()
{
    XEEN_VALID();

    for(int i = 0; i != 4; i ++)
    {
        const uint16 segID = _data->getU16At(OFF_SURR_MAZES + i * 2);
        if(segID)
        {
            _surrMazes[i] = _parent->getSegment(segID);
        }
    }
}

uint16 XEEN::Maze::Segment::getSurrounding(Direction dir) const
{
    return _data->getU16At(OFF_SURR_MAZES + dir * 2);
}

uint16 XEEN::Maze::Segment::getWall(const Common::Point& pos) const
{
    XEEN_VALID();
    return enforce(Common::Rect(0, 0, 16, 16).contains(pos)) ? _data->getU16At(OFF_WALLS + (pos.y * 32 + pos.x * 2)) : 0;
}

uint8 XEEN::Maze::Segment::getCellFlags(const Common::Point& pos) const
{
    XEEN_VALID();
    return enforce(Common::Rect(0, 0, 16, 16).contains(pos)) ? _data->getByteAt(OFF_CELL_FLAGS + (pos.y * 16 + pos.x)) : 0;
}

uint32 XEEN::Maze::Segment::getMapFlags() const
{
    XEEN_VALID();
    return _data->getU32At(OFF_MAP_FLAGS);
}

uint8 XEEN::Maze::Segment::lookupSurface(uint8 id) const
{
    XEEN_VALID();
    return enforce(id < 16) ? _data->getByteAt(OFF_SURFACE_TYPES + id) : 0;
}

uint8 XEEN::Maze::Segment::lookupWall(uint8 id) const
{
    XEEN_VALID();
    return enforce(id < 16) ? _data->getByteAt(OFF_WALL_TYPES + id) : 0;
}

uint32 XEEN::Maze::Segment::getValue(SegmentValue val) const
{
    XEEN_VALID();
    return enforce(val < MAX_VALUE) ? _data->getByteAt(OFF_VALUES + val) : 0;
}

XEEN::Maze::Segment* XEEN::Maze::Segment::resolveSegment(Common::Point& position)
{
    XEEN_VALID();

    Segment* activeSegment = this;

    for(; valid(activeSegment) && position.y >= 16; position.y -= 16)
    {
        activeSegment = activeSegment->_surrMazes[0];
    }

    for(; valid(activeSegment) && position.x >= 16; position.x -= 16)
    {
        activeSegment = activeSegment->_surrMazes[1];
    }

    for(; valid(activeSegment) && position.y < 0; position.y += 16)
    {
        activeSegment = activeSegment->_surrMazes[2];
    }
    
    for(; valid(activeSegment) && position.x < 0; position.x += 16)
    {
        activeSegment = activeSegment->_surrMazes[3];
    }

    return activeSegment;
}
