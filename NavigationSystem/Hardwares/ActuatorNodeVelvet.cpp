
/****************************************************************************************
 *
 * File:
 * 		ActuatorNodeVelvet.cpp
 *
 * Purpose:
 *		Controls an actuator on the vessel.
 *
 * Developer Notes:
 *
 *
 ***************************************************************************************/

#include "../Hardwares/ActuatorNodeVelvet.h"
#include "../Messages/VelvetActuatorFeedbackMsg.h"
#include "../Hardwares/MaestroController/MaestroController.h"
#include "../SystemServices/Logger.h"


ActuatorNodeVelvet::ActuatorNodeVelvet(MessageBus& msgBus, NodeID id, int channel, int speed, int acceleration)
        :Node(id, msgBus), m_Channel(channel), m_Speed(speed), m_Acceleration(acceleration)
{
    msgBus.registerNode(*this,MessageType::VelvetActuatorFeedback);
    msgBus.registerNode(*this,MessageType::ServerConfigsReceived);
}

bool ActuatorNodeVelvet::init()
{
    if( MaestroController::writeCommand(MaestroCommands::SetSpeed, m_Channel, m_Speed) &&
        MaestroController::writeCommand(MaestroCommands::SetAcceleration, m_Channel, m_Acceleration) )
    {
        return true;
    }
    else
    {
        Logger::error("%s Failed to write actuator speed and acceleration!", __PRETTY_FUNCTION__);
        return false;
    }

}

void ActuatorNodeVelvet::processMessage(const Message* message)
{
    if(message->messageType() == MessageType::VelvetActuatorFeedback)
    {
        VelvetActuatorFeedbackMsg* msg = (VelvetActuatorFeedbackMsg*)message;

        //Get relevant command
        int setPosition = 0;

        if (nodeID() == NodeID::RudderActuator)
        {
            setPosition = msg->rudderFeedback();
        }
        else if (nodeID() == NodeID::SailActuator)
        {
            setPosition = msg->wingsailFeedback();
        }
        else
        {
            Logger::warning("%s Actuator: %d Unknown/Unregistered actuator message NodeID", __PRETTY_FUNCTION__, (int)nodeID());
            return;
        }

        if(not MaestroController::writeCommand(MaestroCommands::SetPosition, m_Channel, setPosition))
        {
            Logger::error("%s Actuator: %d Failed to write position command", __PRETTY_FUNCTION__, (int)nodeID());
        }
    }

}
