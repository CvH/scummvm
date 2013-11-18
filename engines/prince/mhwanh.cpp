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

#include "common/stream.h"

#include "graphics/surface.h"

#include "prince/mhwanh.h"

namespace Prince {

MhwanhDecoder::MhwanhDecoder() 
	: _surface(NULL), _palette(0), _paletteColorCount(0) {
}

MhwanhDecoder::~MhwanhDecoder() {
	destroy();
}

void MhwanhDecoder::destroy() {
	if (_surface) {
		_surface->free();
		delete _surface; 
		_surface = 0;
	}

	delete [] _palette; _palette = 0;
	_paletteColorCount = 0;
}

bool MhwanhDecoder::loadStream(Common::SeekableReadStream &stream) {
	destroy();
	_paletteColorCount = 256;
	stream.seek(0);
	stream.skip(0x20);
	// Read the palette
	_palette = new byte[_paletteColorCount * 3]; 
	for (uint16 i = 0; i < _paletteColorCount; i++) {
		_palette[i * 3 + 0] = stream.readByte();
		_palette[i * 3 + 1] = stream.readByte();
		_palette[i * 3 + 2] = stream.readByte();
	}  

	_surface = new Graphics::Surface();
	_surface->create(640, 480, Graphics::PixelFormat::createFormatCLUT8());
	for (int h = 0; h < 480; ++h) {
		stream.read(_surface->getBasePtr(0, h), 640);
	}

	return true;
}

}

/* vim: set tabstop=4 noexpandtab: */
