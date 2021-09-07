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
#include "mma7660.hpp"

#include <cstring>

namespace modm
{

template <typename I2cMaster, typename INT>
Mma7660<I2cMaster, INT>::Mma7660() : modm::I2cDevice<I2cMaster, 3>(address) {}

template <typename I2cMaster, typename INT>
modm::ResumableResult<void>
Mma7660<I2cMaster, INT>::writeRead(Register reg, const void* writeBuffer, uint8_t writeLen, uint8_t readLen) {
	RF_BEGIN();
	static_assert(sizeof(buffer[0]) == sizeof(reg));
	buffer[0] = static_cast<uint8_t>(reg);
	memcpy(&buffer[1], writeBuffer, writeLen);

	this->transaction.configureWriteRead(buffer, writeLen + 1, buffer, readLen);
	bool ok;  // TODO: ?
	ok = RF_CALL(this->runTransaction());
	modm_assert(ok, "mma7660.rt", "I2C transaction failed", reg);

	RF_END();
}

template <typename I2cMaster, typename INT>
template <typename T>
modm::ResumableResult<void>
Mma7660<I2cMaster, INT>::write(Register reg, T data) {
	static_assert(sizeof(T) + sizeof(reg) <= sizeof(buffer));
	RF_BEGIN();
	RF_END_RETURN_CALL(writeRead(reg, &data, sizeof(data), 0));
}

template <typename I2cMaster, typename INT>
template <typename T>
modm::ResumableResult<T>
Mma7660<I2cMaster, INT>::read(Register reg) {
	static_assert(sizeof(T) <= sizeof(buffer));
	RF_BEGIN();
	RF_CALL(writeRead(reg, nullptr, 0, sizeof(T)));
	RF_END_RETURN(*reinterpret_cast<T*>(buffer));
}

template <typename I2cMaster, typename INT>
void Mma7660<I2cMaster, INT>::initialize() {
	using namespace modm::detail::mma7660;

	INT::setInput(modm::platform::Gpio::InputType::PullUp);

	RF_CALL_BLOCKING(write(Register::Mode, registers::Mode_t(registers::Mode::ActiveMode)));
}

template <typename I2cMaster, typename INT>
modm::ResumableResult<Mma7660Val>
Mma7660<I2cMaster, INT>::read() {
	RF_BEGIN();
	static Mma7660Val val;
	val = {};
	// TODO: read all at once
	// TODO: check alert field
	val.x = RF_CALL(read<uint8_t>(Register::XOut));
	val.y = RF_CALL(read<uint8_t>(Register::YOut));
	val.z = RF_CALL(read<uint8_t>(Register::ZOut));
	RF_END_RETURN(val);
}

} // namespace modm
