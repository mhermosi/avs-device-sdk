/*
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <sstream>

#include "SampleApp/EverloopControl.h"

#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>
#include "AVSCommon/Utils/SDKVersion.h"

#include "SampleApp/ConsolePrinter.h"

namespace alexaClientSDK {
namespace sampleApp {

using namespace avsCommon::sdkInterfaces;

static const std::string VERSION = avsCommon::utils::sdkVersion::getCurrentVersion();

void EverloopControl::iterateEverLoopLeds(int red, int green, int blue, int white) {
    for(int nled = 0; nled < image1d.leds.size(); i++){
        hal::LedValue &led = image1d.leds[nled];
        led.red = red;
        led.green = green;
        led.blue = blue;
        led.white = white;

        if(nled > 0) {
            led = image1d.leds[nled-1];
            led.red = 0;
            led.green = 0;
            led.blue = 0;
            led.white = 0;
        }
    }

    everloop.Write(&image1d);

}

void EverloopControl::blankLed(hal::LedValue& led ) {
    led.red   = 0;
    led.green = 0;
    led.blue  = 0;
    led.white = 0;
}
void EverloopControl::blankAllLeds() {
    for (hal::LedValue& led : image1d.leds) {
        EverloopControl::blankLed(led);
    }
}

void EverloopControl::SetEverloopColors(int red, int green, int blue, int white) {
     for (hal::LedValue& led : image1d.leds) {
        led.red = red;
        led.green = green;
        led.blue = blue;
        led.white = white;
    }
    everloop.Write(&image1d);
}

void EverloopControl::onDialogUXStateChanged(DialogUXState state) {
    
    m_executor.submit([this, state]() {
        switch (state) {
            case DialogUXState::IDLE:
                iterateEverLoopLeds(0,10,10,0);
                return;
            case DialogUXState::LISTENING:
                iterateEverLoopLeds(0,10,0,0);
                return;
            case DialogUXState::THINKING:
                iterateEverLoopLeds(10, 0, 0, 0);
                return;
            case DialogUXState::SPEAKING:
                iterateEverLoopLeds(10, 0, 0, 0);
                return;
            case DialogUXState::FINISHED:
                return;
            default:
                return;
        }   
    });
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
