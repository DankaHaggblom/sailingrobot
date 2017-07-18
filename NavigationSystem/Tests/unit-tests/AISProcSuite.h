/****************************************************************************************
*
* File:
* 		AISProcSuite.h
*
* Purpose:
*		Testing the AISProcessing node!
*
* Developer Notes:
*
*
***************************************************************************************/

#include "WorldState/AISProcessing.h"
#include "../cxxtest/cxxtest/TestSuite.h"
#include "Messages/StateMessage.h"
#include "../../MessageBus/MessageBus.h"
#include "TestMocks/MessageLogger.h"
#include "WorldState/CollidableMgr/CollidableMgr.h"

#define AISPROC_TEST_COUNT 2

class AISProcSuite : public CxxTest::TestSuite {
public:
  AISProcessing* aisProc;
  CollidableMgr cMgr;
  MockNode* mockNode;
  bool mockNodeRegistered = false;

  int testCount;

  std::thread* thr;

  static MessageBus& msgBus(){
    static MessageBus* mbus = new MessageBus();
    return *mbus;
  }

  static void runMessageLoop() {
    msgBus().run();
  }

  void setUp() {
    mockNode = new MockNode(msgBus(), mockNodeRegistered);
    if (aisProc == 0) {
      aisProc = new AISProcessing(msgBus(), &cMgr, 300e6, 100.0);

      aisProc->start();

      std::this_thread::sleep_for(std::chrono::milliseconds(200));

      thr = new std::thread(runMessageLoop);
    }
    testCount++;
  }

  void tearDown() {
    if (testCount == AISPROC_TEST_COUNT) {
      // delete aisProc;
    }
    delete mockNode;
  }

  void test_ReceiveMessage(){
    std::vector<AISVessel> AISList;
    AISVessel v1, v2, v3;
    v1.MMSI = 1;
    v1.latitude = 60.2f;
    v1.longitude = 19.1f;
    v1.COG = 200;
    v1.SOG = 10;
    v2.MMSI = 2;
    v2.latitude = 62.f;
    v2.longitude = 18.1f;
    v2.COG = 100;
    v2.SOG = 5;
    v3.MMSI = 3;
    v3.latitude = 61.5f;
    v3.longitude = 18.7f;
    v3.COG = 80;
    v3.SOG = 7;
    AISList.push_back(v1);
    AISList.push_back(v2);
    AISList.push_back(v3);

    MessagePtr mockAISMsg = std::make_unique<AISDataMsg>(AISList, 60.1, 19.1);
    msgBus().sendMessage(std::move(mockAISMsg));

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    TS_ASSERT(mockNode->m_MessageReceived);
  }

  void test_ReceiveData() {
    std::vector<AISVessel> AISList;
    AISVessel v1, v2, v3;
    v1.MMSI = 1;
    v1.latitude = 60.2f;
    v1.longitude = 19.1f;
    v1.COG = 200;
    v1.SOG = 10;
    v2.MMSI = 2;
    v2.latitude = 62.f;
    v2.longitude = 18.1f;
    v2.COG = 100;
    v2.SOG = 5;
    v3.MMSI = 3;
    v3.latitude = 61.5f;
    v3.longitude = 18.7f;
    v3.COG = 80;
    v3.SOG = 7;
    AISList.push_back(v1);
    AISList.push_back(v2);
    AISList.push_back(v3);

    MessagePtr mockAISMsg = std::make_unique<AISDataMsg>(AISList, 60.1, 19.1);
    msgBus().sendMessage(std::move(mockAISMsg));

    cMgr.getAISContacts();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    TS_ASSERT_EQUALS(cMgr.getAISContacts().length(),3);
  }
};
