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

#include "common/scummsys.h"

#include "zvision/utility/utility.h"

#include "zvision/zvision.h"
#include "zvision/sound/zork_raw.h"

#include "common/tokenizer.h"
#include "common/file.h"

namespace ZVision {

void trimCommentsAndWhiteSpace(Common::String *string) {
	for (int i = string->size() - 1; i >= 0; i--) {
		if ((*string)[i] == '#') {
			string->erase(i);
		}
	}

	string->trim();
}

void tryToDumpLine(const Common::String &key,
                   Common::String &line,
                   Common::HashMap<Common::String, byte> *count,
                   Common::HashMap<Common::String, bool> *fileAlreadyUsed,
                   Common::DumpFile &output) {
	const byte numberOfExamplesPerType = 8;

	if ((*count)[key] < numberOfExamplesPerType && !(*fileAlreadyUsed)[key]) {
		output.writeString(line);
		output.writeByte('\n');
		(*count)[key]++;
		(*fileAlreadyUsed)[key] = true;
	}
}

} // End of namespace ZVision