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

/*
 * This code is based on original Sfinx source code
 * Copyright (c) 1994-1997 Janus B. Wisniewski and L.K. Avalon
 */

#ifndef CGE2_VGA13H_H
#define CGE2_VGA13H_H

#include "common/serializer.h"
#include "common/events.h"
#include "graphics/surface.h"
#include "cge2/general.h"
#include "cge2/bitmap.h"
#include "cge2/snail.h"
#include "cge2/cge2.h"

namespace CGE2 {

#define kFadeStep        2
#define kVgaColDark      207
#define kVgaColDarkGray  225 /*219*/
#define kVgaColGray      231
#define kVgaColLightGray 237
#define kPixelTransp     0xFE
#define kNoSeq           (-1)
#define kNoPtr           ((uint8)-1)
#define kSprExt          ".SPR"
#define kPalCount        256
#define kPalSize         (kPalCount * 3)

struct Seq {
	uint8 _now;
	uint8 _next;
	int8 _dx;
	int8 _dy;
	int8 _dz;
	int _dly;
};

class SprExt {
public:
	V2D _p0;
	V2D _p1;
	BitmapPtr _b0;
	BitmapPtr _b1;
	BitmapPtr *_shpList;
	int _location;
	Seq *_seq;
	char *_name;
	CommandHandler::Command *_actions[kActions];
	SprExt(CGE2Engine *vm) : _p0(vm, 0, 0), _p1(vm, 0, 0),
		_b0(NULL), _b1(NULL),_shpList(NULL), _location(0),
		_seq(NULL), _name(NULL)
	{}
};

class Sprite {
protected:
	SprExt *_ext;
	CGE2Engine *_vm;
public:
	int _ref;
	signed char _scene;
	struct Flags {
		uint16 _hide : 1;       // general visibility switch
		uint16 _drag : 1;       // sprite is moveable
		uint16 _hold : 1;       // sprite is held with mouse
		uint16 _trim : 1;       // Trim flag
		uint16 _slav : 1;       // slave object
		uint16 _kill : 1;       // dispose memory after remove
		uint16 _xlat : 1;       // 2nd way display: xlat table
		uint16 _port : 1;       // portable
		uint16 _kept : 1;       // kept in pocket
		uint16 _frnt : 1;       // stay in front of sprite
		uint16 _east : 1;       // talk to east (in opposite to west)
		uint16 _near : 1;       // Near action lock
		uint16 _shad : 1;       // shadow
		uint16 _back : 1;       // 'send to background' request
		uint16 _zmov : 1;       // sprite needs Z-update in queue
		uint16 _tran : 1;       // transparent (untouchable)
	} _flags;
	V2D _pos2D;
	V3D _pos3D;
	V2D _siz;
	uint16 _time;
	struct { byte _ptr, _cnt; } _actionCtrl[kActions];
	int _seqPtr;
	int _seqCnt;
	int _shpCnt;
	char _file[kMaxFile];
	// Following trailer is not saved with the game:
	Sprite *_prev;
	Sprite *_next;
	static byte _constY;
	static byte _follow;
	static Seq _stdSeq8[];

	bool works(Sprite *spr);
	bool seqTest(int n);
	inline bool active() {
		return _ext != NULL;
	}
	Sprite(CGE2Engine *vm);
	Sprite(CGE2Engine *vm, BitmapPtr *shp, int cnt);
	virtual ~Sprite();
	BitmapPtr shp();
	void setShapeList(BitmapPtr *shp, int cnt);
	void moveShapesHi(void);
	void moveShapesLo(void);
	int LabVal(Action snq, int lab);
	Sprite *expand();
	Sprite *contract();
	Sprite *backShow(bool fast = false);
	void setName(char *newName);
	inline char *name() {
		return (_ext) ? _ext->_name : NULL;
	}
	void gotoxyz(int x, int y, int z = 0);
	void gotoxyz(void);
	void gotoxyz(V2D pos);
	void gotoxyz_(V2D pos);
	void gotoxyz(V3D pos);
	void center();
	void show(uint16 pg);
	void hide(uint16 pg);
	void show();
	void hide();
	BitmapPtr ghost();
	void makeXlat(uint8 *x);
	void killXlat();
	void step(int nr = -1);
	Seq *setSeq(Seq *seq);
	CommandHandler::Command *snList(Action type);
	//virtual void touch(uint16 mask, int x, int y, Common::KeyCode keyCode);
	//virtual void tick();
	void clrHide(void) { if (_ext) _ext->_b0 = NULL; }
	void sync(Common::Serializer &s);

	static void (*notify) (void);
};

class Queue {
	Sprite *_head;
	Sprite *_tail;
public:
	Queue(bool show);
	~Queue();

	bool _show;

	void append(Sprite *spr);
	void insert(Sprite *spr, Sprite *nxt);
	void insert(Sprite *spr);
	Sprite *remove(Sprite *spr);
	Sprite *first() {
		return _head;
	}
	Sprite *last() {
		return _tail;
	}
	Sprite *locate(int ref);
	void clear();
};

class Vga {
	CGE2Engine *_vm;
	bool _setPal;
	Dac *_oldColors;
	Dac *_newColors;
	const char *_msg;
	const char *_name;

	void updateColors();
	void setColors();
	void waitVR();
	uint8 closest(Dac *pal, const uint8 colR, const uint8 colG, const uint8 colB);

public:
	uint32 _frmCnt;
	Queue *_showQ;
	Queue *_spareQ;
	int _mono;
	Graphics::Surface *_page[4];
	Dac *_sysPal;

	Vga(CGE2Engine *vm);
	~Vga();

	uint8 *glass(Dac *pal, const uint8 colR, const uint8 colG, const uint8 colB);
	void getColors(Dac *tab);
	void setColors(Dac *tab, int lum);
	void clear(uint8 color);
	void copyPage(uint16 d, uint16 s);
	void sunrise(Dac *tab);
	void sunset();
	void show();
	void update();

	void palToDac(const byte *palData, Dac *tab);
	void dacToPal(const Dac *tab, byte *palData);
};

class HorizLine: public Sprite {
	CGE2Engine *_vm;
public:
	HorizLine(CGE2Engine *vm);
};

class SceneLight: public Sprite {
	CGE2Engine *_vm;
public:
	SceneLight(CGE2Engine *vm);
};

class Speaker: public Sprite {
	CGE2Engine *_vm;
public:
	Speaker(CGE2Engine *vm);
};

class PocLight: public Sprite {
	CGE2Engine *_vm;
public:
	PocLight(CGE2Engine *vm);
};

} // End of namespace CGE2

#endif // CGE2_VGA13H_H
