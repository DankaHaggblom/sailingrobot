
/****************************************************************************************
 *
 * File:
 * 		WindDataReporter.h
 *
 * Purpose:
 *		The WindDataReporter node provides listens to wind data messages and transforms them
 *      into JSON bodies.
 *
 * Developer Notes:
 *
 *
 ***************************************************************************************/
#pragma once

#include "MessageBus/ActiveNode.h"
#include "Messages/WindDataMsg.h"
#include "SystemServices/Logger.h"
#include <atomic>
#include <chrono>
#include <string>
#include <thread>

class WindDataReporterNode : public ActiveNode 
{
   public:
    WindDataReporterNode(MessageBus& msgBus);

    virtual ~WindDataReporterNode() {}

    bool init();
    void start();
    void stop();

    void processMessage(const Message* message);

    static void WindDataReporterThread(ActiveNode* nodePtr);

   private:
    std::atomic<bool> m_Running;
    bool m_Initialised;  // Indicates that the node was correctly initialised
};
