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

#include "mma7660_register.hpp"

#include <modm/architecture/interface/i2c_device.hpp>
//TODO #include <modm/processing/protothread.hpp>
#include <modm/math/utils.hpp>

namespace modm
{

// TODO: inside class
struct Mma7660Val {
	uint8_t x, y, z;
};

template <typename I2cMaster, typename INT>
class Mma7660 : public modm::I2cDevice<I2cMaster, 3> {
	using Register = detail::mma7660::Register;

	static constexpr uint8_t address = 0x4C;

	uint8_t buffer[2];

	modm::ResumableResult<void>
	writeRead(Register reg, const void* writeBuffer, uint8_t writeLen, uint8_t readLen);

	template <typename T>
	modm::ResumableResult<void>
	write(Register reg, T data);

	template <typename T>
	modm::ResumableResult<T>
	read(Register reg);

public:
	Mma7660();

	void initialize();

	modm::ResumableResult<Mma7660Val>
	read();
};

} // namespace modm

#include "mma7660_impl.hpp"
