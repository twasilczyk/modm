/*
 * Copyright (c) 2021, Tomasz Wasilczyk
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/board.hpp>

#include <modm/driver/inertial/mma7660.hpp>

modm::Mma7660<Board::i2c::I2cMaster, GpioE5> accel;

int
main()
{
	Board::initialize();

	auto& display = Board::display;

	display.setFont(modm::font::Ubuntu_36);

	display << "i";
	display << 1;
	display.update();
	bool ok = RF_CALL_BLOCKING(accel.ping());
	display << (ok ? "OK" : "ERR");
	display.update();
	accel.initialize();
	display.update();
	display << 2;

	display.update();
	while (true) {
		display.setCursor(0, 40);
		// TODO: use a separate protothread to update this
		const auto r = RF_CALL_BLOCKING(accel.read());
		display << "x=" << r.x << " y=" << r.y << " z=" << r.z << "   ";
		display.update();
	}
}
