/*
 * Copyright (c) 2021, Henrik Hose
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_H750VB_DEVEBOX_HPP
#define MODM_STM32_H750VB_DEVEBOX_HPP

#include <modm/platform.hpp>
#include <modm/architecture/interface/clock.hpp>

using namespace modm::platform;

/// @ingroup modm_board_devebox_stm32h750vb
namespace Board
{
using namespace modm::literals;

/// STM32H750 running at 400MHz from the external 25MHz HSE
struct SystemClock
{
	// NOTE: revision Y at 400MHz, only revision V runs at 480Mhz!!!
	// some of the devebox boards including the original one have the Y version!!!
	static constexpr uint32_t SysClk = 400_MHz;
	// Max 400MHz or 480MHz
	static constexpr uint32_t Hclk = SysClk / 1; // D1CPRE
	static constexpr uint32_t Frequency = Hclk;
	// Max 200MHz or 240MHz
	static constexpr uint32_t Ahb = Hclk / 2; // HPRE
	static constexpr uint32_t Ahb1 = Ahb;
	static constexpr uint32_t Ahb2 = Ahb;
	static constexpr uint32_t Ahb3 = Ahb;
	static constexpr uint32_t Ahb4 = Ahb;
	// Max 100MHz or 120MHz
	static constexpr uint32_t Apb1 = Ahb / 2; // D2PPRE1
	static constexpr uint32_t Apb2 = Ahb / 2; // D2PPRE2
	static constexpr uint32_t Apb3 = Ahb / 2; // D1PPRE
	static constexpr uint32_t Apb4 = Ahb / 2; // D3PPRE

	static constexpr uint32_t Adc1 = Ahb1;
	static constexpr uint32_t Adc2 = Ahb1;
	static constexpr uint32_t Adc3 = Ahb4;

	static constexpr uint32_t Dac1 = Apb1;

	static constexpr uint32_t Spi1 = Apb2;
	static constexpr uint32_t Spi2 = Apb1;
	static constexpr uint32_t Spi3 = Apb1;
	static constexpr uint32_t Spi4 = Apb2;
	static constexpr uint32_t Spi5 = Apb2;
	static constexpr uint32_t Spi6 = Apb4;

	static constexpr uint32_t Usart1 = Apb2;
	static constexpr uint32_t Usart2 = Apb1;
	static constexpr uint32_t Usart3 = Apb1;
	static constexpr uint32_t Uart4  = Apb1;
	static constexpr uint32_t Uart5  = Apb1;
	static constexpr uint32_t Usart6 = Apb2;
	static constexpr uint32_t Uart7  = Apb1;
	static constexpr uint32_t Uart8  = Apb1;

	static constexpr uint32_t LpUart1 = Apb4;

	static constexpr uint32_t Can1 = Apb1;
	static constexpr uint32_t Can2 = Apb1;

	static constexpr uint32_t I2c1 = Apb1;
	static constexpr uint32_t I2c2 = Apb1;
	static constexpr uint32_t I2c3 = Apb1;
	static constexpr uint32_t I2c4 = Apb4;

	static constexpr uint32_t Apb1Timer = Apb1 * 2;
	static constexpr uint32_t Apb2Timer = Apb2 * 2;
	static constexpr uint32_t Timer1  = Apb2Timer;
	static constexpr uint32_t Timer2  = Apb1Timer;
	static constexpr uint32_t Timer3  = Apb1Timer;
	static constexpr uint32_t Timer4  = Apb1Timer;
	static constexpr uint32_t Timer5  = Apb1Timer;
	static constexpr uint32_t Timer6  = Apb1Timer;
	static constexpr uint32_t Timer7  = Apb1Timer;
	static constexpr uint32_t Timer8  = Apb2Timer;
	static constexpr uint32_t Timer12 = Apb1Timer;
	static constexpr uint32_t Timer13 = Apb1Timer;
	static constexpr uint32_t Timer14 = Apb1Timer;
	static constexpr uint32_t Timer15 = Apb2Timer;
	static constexpr uint32_t Timer16 = Apb2Timer;
	static constexpr uint32_t Timer17 = Apb2Timer;

	static constexpr uint32_t Usb = 48_MHz; // From PLL3Q

	static bool inline
	enable()
	{
		Rcc::enableExternalCrystal(); // 25MHz
		Rcc::setVoltageScaling(Rcc::VoltageScaling::Scale0); // required for 400MHz/480MHz
		const Rcc::PllFactors pllFactors1{
			.range = Rcc::PllInputRange::MHz8_16,
			.pllM = 2,		//   25MHz / M= 12.5MHz
			.pllN = 64,		// 12.5MHz * N=  800MHz
			.pllP = 2,		//  800MHz / P=  400MHz = F_cpu
			.pllQ = 20,		//  800MHz / Q=   40MHz
			.pllR = 20,		//  800MHz / R=   40MHz
		};
		Rcc::enablePll1(Rcc::PllSource::ExternalClock, pllFactors1);
		// Use PLL3 for USB 48MHz
		const Rcc::PllFactors pllFactors3{
			.range = Rcc::PllInputRange::MHz4_8,
			.pllM = 5,		//   25MHz / M=     5MHz
			.pllN = 48,		//    5MHz * N=   240MHz
			.pllP = 2,		//  240MHz / P=   120MHz
			.pllQ = 5,		//  240MHz / Q=    48MHz = F_usb
			.pllR = 5,		//  240MHz / R=    48MHz
		};
		Rcc::enablePll3(Rcc::PllSource::ExternalClock, pllFactors3);
		Rcc::setFlashLatency<Ahb>();
		// max 240MHz on AHB
		Rcc::setAhbPrescaler(Rcc::AhbPrescaler::Div2);
		// max 120MHz on APB
		Rcc::setApb1Prescaler(Rcc::Apb1Prescaler::Div2);
		Rcc::setApb2Prescaler(Rcc::Apb2Prescaler::Div2);
		Rcc::setApb3Prescaler(Rcc::Apb3Prescaler::Div2);
		Rcc::setApb4Prescaler(Rcc::Apb4Prescaler::Div2);
		// update clock frequencies
		Rcc::updateCoreFrequency<Hclk>();
		Rcc::enableUsbClockSource(Rcc::UsbClockSource::Pll3Q);
		// Switch the main clock source to PLL
		Rcc::enableSystemClock(Rcc::SystemClockSource::Pll1P);

		return true;
	}
};


using ButtonK1 = GpioInverted<GpioInputE3>;
using ButtonK2 = GpioInverted<GpioInputC5>;

using LedGreen = GpioInverted<GpioOutputA1>;	// User LED

using Leds = SoftwareGpioPort< LedGreen >;

namespace usb
{
using Dm = GpioA11;		// DM: USB_DM
using Dp = GpioA12;		// DP: USB_DP
}


inline void
initialize()
{
	SystemClock::enable();
	SysTickTimer::initialize<SystemClock>();

	LedGreen::setOutput(modm::Gpio::Low);

	ButtonK1::setInput(Gpio::InputType::PullUp);
	ButtonK1::setInputTrigger(Gpio::InputTrigger::FallingEdge);
	ButtonK1::enableExternalInterrupt();

	ButtonK2::setInput(Gpio::InputType::PullUp);
	ButtonK2::setInputTrigger(Gpio::InputTrigger::FallingEdge);
	ButtonK2::enableExternalInterrupt();
}
}

#endif	// MODM_STM32_H750VB_DEVEBOX_HPP
