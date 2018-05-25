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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef PINK_LEAD_ACTOR_H
#define PINK_LEAD_ACTOR_H

#include "common/rect.h"
#include "common/keyboard.h"

#include "pink/objects/actors/actor.h"

namespace Pink {

class CursorMgr;
class WalkMgr;
class WalkLocation;
class Sequencer;

class SupportingActor;
class InventoryItem;

class LeadActor : public Actor {
public:
	LeadActor();
	enum State {
		kReady = 0,
		kMoving = 1,
		kInDialog1 = 2, //???
		kInventory = 3,
		kPDA = 4,
		kInDialog2 = 5,//???
		kPlayingVideo = 6, // ???
		kUnk_Loading = 7// ????
	};


	virtual void deserialize(Archive &archive);

	virtual void toConsole();

	void setNextExecutors (Common::String &nextModule, Common::String &nextPage);
	virtual void init(bool unk);

	State getState() const;

	void start(bool isHandler);
	void update();

	void onKeyboardButtonClick(Common::KeyCode code);
	void onLeftButtonClick(Common::Point point);
	void onMouseMove(Common::Point point);
	void onWalkEnd();
	virtual void onClick();
	void onInventoryClosed(bool isItemClicked);
	virtual void onVariableSet() {};

	virtual void onMouseOver(Common::Point point, CursorMgr *mgr);

	bool isInteractingWith(SupportingActor *actor);

	virtual void loadState(Archive &archive);

	virtual void saveState(Archive &archive);

protected:
	virtual void updateCursor(Common::Point point);
	void forceUpdateCursor();

	virtual bool sendUseClickMessage(SupportingActor *actor);
	bool sendLeftClickMessage(SupportingActor *actor);

	virtual WalkLocation *getWalkDestination();

	State _state;
	State _nextState;
	State _stateCopy;

	bool _isHaveItem;

	SupportingActor *_recipient;

	CursorMgr *_cursorMgr;
	WalkMgr *_walkMgr;
	Sequencer *_sequencer;
};


class ParlSqPink : public LeadActor {
public:
	virtual WalkLocation *getWalkDestination();
	void toConsole();
};

class PubPink : public LeadActor {
public:
	PubPink();

	void toConsole();

	virtual void onClick();
	virtual void onVariableSet();

private:
	int _round;

	virtual bool sendUseClickMessage(SupportingActor *actor);
	virtual void updateCursor(Common::Point point);
	virtual WalkLocation *getWalkDestination();

	bool playingMiniGame();
};


} // End of namespace Pink

#endif
