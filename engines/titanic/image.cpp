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

#include "common/file.h"
#include "titanic/image.h"

namespace Titanic {

BITMAPINFOHEADER::BITMAPINFOHEADER() {
	_biSize = 0;
	_biWidth = 0;
	_biHeight = 0;
	_biPlanes = 0;
	_biBitCount = 0;
	_biCompression = 0;
	_biSizeImage = 0;
	_biXPelsPerMeter = 0;
	_biYPelsPerMeter = 0;
	_biClrUsed = 0;
	_biClrImportant = 0;
}

/*------------------------------------------------------------------------*/

RGBQuad::RGBQuad() : _rgbRed(0), _rgbGreen(0), _rgbBlue(0), _rgbReserved(0) {}

/*------------------------------------------------------------------------*/

Image::Image() {
	_bitmapInfo = nullptr;
	_bits = nullptr;
	_flag = true;

	set(16, 16);
}

void Image::proc6() {

}

void Image::set(int width, int height) {
	delete _bitmapInfo;
	if (_flag && _bitmapInfo)
		delete[] _bits;

	_bitmapInfo = new tagBITMAPINFO;
	_bits = new byte[(width + 3) & 0xFFFC * height];

	tagBITMAPINFO &bi = *_bitmapInfo;
	bi._bmiHeader._biWidth = width;
	bi._bmiHeader._biHeight = height;
	bi._bmiHeader._biPlanes = 1;
	bi._bmiHeader._biBitCount = 8;
}

void Image::proc8() {

}

bool Image::loadResource(const Common::String &name) {
	// This method is hardcoded for the Titanic splash screen resource
	assert(name == "TITANIC");

	Common::File f;
	if (!f.open("ST.exe"))
		return false;

	// The ST.exe executable has a bitmap called "TITANIC". Since we can't use
	// the Windows FindResource function in ScummVM, this is hardcoded for now
	f.seek(0x29B660);
	uint size = f.readUint32LE();
	if (size != 40)
		return false;

	loadBitmap(f);

	return true;
}

void Image::proc10() {

}

void Image::draw() {

}

void Image::loadBitmap(Common::SeekableReadStream &s) {
	_bitmapInfo->_bmiHeader._biWidth = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biHeight = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biPlanes = s.readUint16LE();
	_bitmapInfo->_bmiHeader._biBitCount = s.readUint16LE();
	_bitmapInfo->_bmiHeader._biCompression = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biSizeImage = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biXPelsPerMeter = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biYPelsPerMeter = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biClrUsed = s.readUint32LE();
	_bitmapInfo->_bmiHeader._biClrImportant = s.readUint32LE();

}

} // End of namespace Titanic
