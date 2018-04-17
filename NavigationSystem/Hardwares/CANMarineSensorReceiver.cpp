#include <iostream>
#include "CANMarineSensorReceiver.h"
#include "CAN_Services/CANService.h"
#include "Math/Utility.h"
#include "Messages/MarineSensorDataMsg.h"
#include "SystemServices/Logger.h"
#include "CanBusCommon/canbus_id_defs.h"
#include "CanBusCommon/CanMessageHandler.h"
#include "CanBusCommon/canbus_datamappings_defs.h"



CANMarineSensorReceiver::CANMarineSensorReceiver(MessageBus& messageBus, CANService& canService) :
CANFrameReceiver(canService, MSG_ID_MARINE_SENSOR_DATA), m_msgBus(messageBus)
{
}

void CANMarineSensorReceiver::processFrame (CanMsg& msg) {
    Logger::info("Recieved marine sensor readings from CanBus");

    CanMessageHandler handler(msg);

    double ph = handler.getMappedData(SENSOR_PH_DATASIZE,
                                     SENSOR_PH_INTERVAL_MIN, SENSOR_PH_INTERVAL_MAX);

    double conductivety = handler.getMappedData(SENSOR_CONDUCTIVETY_DATASIZE,
                                               SENSOR_CONDUCTIVETY_INTERVAL_MIN, SENSOR_CONDUCTIVETY_INTERVAL_MAX);

    double temp = handler.getMappedData(SENSOR_TEMPERATURE_DATASIZE,
                                       SENSOR_TEMPERATURE_INTERVAL_MIN, SENSOR_TEMPERATURE_INTERVAL_MAX);
    float salinity = Utility::calculateSalinity (temp, conductivety);

    MessagePtr marineSensorDataMsg = std::make_unique<MarineSensorDataMsg>(static_cast<float>(temp), static_cast<float>(conductivety), static_cast<float>(ph), salinity);
    m_msgBus.sendMessage(std::move(marineSensorDataMsg));



    if(handler.getError() > 0) {
        Logger::error("Error from marine sensors, error code: %d", handler.getError());
    }


}
