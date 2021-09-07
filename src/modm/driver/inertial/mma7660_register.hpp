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

#pragma once

namespace modm::detail::mma7660
{

enum class Register : uint8_t {
	XOut = 0x00,  ///< X output (read only except in Test Mode).
	YOut = 0x01,  ///< Y output (read only except in Test Mode).
	ZOut = 0x02,  ///< Z output (read only except in Test Mode).
	Tilt = 0x03,  ///< Tilt status (read only).
	SampleRateStatus = 0x04,  ///< Sample rate status (read only).
	SleepCount = 0x05,  ///< Sleep count (read/write).
	IntSetup = 0x06,  ///< Interrupt setup.
	Mode = 0x07,  ///< Mode (read/write).
	SampleRate = 0x08,  ///< Sample rate (read/write).
	TapDetection = 0x09,  ///< Tap/pulse detection (read/write).
	TapDebounceCount = 0x0A,  ///< Tap/pulse debounce count (read/write).
};

namespace registers {

enum class Mode : uint8_t {
	ActiveMode = Bit0,
	TestMode = Bit2,
	AutoWake = Bit3,
	AutoSleep = Bit4,
	SleepCounterPrescaler = Bit5,  ///< Sleep counter clock is divided by 16.
	IntPushPull = Bit6,  ///< If set, INT is push-pull. If not, it's open-drain.
	IntActiveHigh = Bit7,  ///< If set, INT is active high.
};
typedef modm::Flags8<Mode> Mode_t;
MODM_TYPE_FLAGS(Mode_t);

}  // namespace registers

}  // namespace modm::detail::mma7660
