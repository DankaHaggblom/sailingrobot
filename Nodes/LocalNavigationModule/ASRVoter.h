/****************************************************************************************
 *
 * File:
 * 		ASRVoter.h
 *
 * Purpose:
 *		The abstract base voter class. A voter contains a course ballot and a vote 
 *      function which needs to be overriden. 
 *
 * License:
 *      This file is subject to the terms and conditions defined in the file 
 *      'LICENSE.txt', which is part of this source code package.
 *
 ***************************************************************************************/


#pragma once


#include "ASRCourseBallot.h"


class ASRVoter {
public:
    ///----------------------------------------------------------------------------------
 	/// Triggers a ASR voter to place votes on the course headings. This function returns
    /// a reference to the internal course ballot data.
 	///----------------------------------------------------------------------------------
    virtual const ASRCourseBallot_t& vote( BoatState_t& boatState ) = 0;

protected:
    ASRCourseBallot courseBallot;
};