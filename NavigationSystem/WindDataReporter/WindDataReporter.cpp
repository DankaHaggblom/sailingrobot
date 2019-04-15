#include "WindDataReporter.h"

WindDataReporterNode::WindDataReporterNode(MessageBus& msgBus) : ActiveNode(NodeID::WindDataReporter, msgBus)
{
    msgBus.registerNode(*this, MessageType::WindData);
}

bool WindDataReporterNode::init()
{
	m_Initialised = true;
	
	return m_Initialised;
}

void WindDataReporterNode::start()
{
	if(m_Initialised)
	{
		runThread(WindDataReporterThread);
	}
	else
	{
		Logger::error("%s Cannot start wind data reporter thread as the node was not correctly initialised!", __PRETTY_FUNCTION__);
	}
}

void WindDataReporterNode::processMessage(const Message* message)
{
	if (message->messageType() == MessageType::WindData)
	{
		WindDataMsg* windMessage = (WindDataMsg*)message;
        printf("{\"timestamp\":\"%s\",\"direction\":%f,\"speed\":%f}", "now", windMessage->windDirection, windMessage->windSpeed);
	}	
}

void WindDataReporterNode::WindDataReporterThread(ActiveNode* nodePtr)
{
    WindDataReporterNode* dataReporter = (WindDataReporterNode*) nodePtr;
}
