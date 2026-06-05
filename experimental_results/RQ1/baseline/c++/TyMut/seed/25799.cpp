# 1 "/home/dani/branches_svn/OROCOS_2.X/robot/sources/src/behaviour/robotTask/ShakeHand.cpp" 1
/*
 *  ShakeHand.cpp
 *  Copyright (c) 2011 PAL Robotics sl. All Rights Reserved
 *  Created on: 14/10/2011
 *      Author: dani
 */

//#include "stdafx.h"
#if 0 /* expanded by -frewrite-includes */
#include "ShakeHand.h"
#endif /* expanded by -frewrite-includes */
# 1 "/home/dani/branches_svn/OROCOS_2.X/robot/sources/src/behaviour/robotTask/ShakeHand.h" 1
/*
 *  ShakeHand.h
 *  Copyright (c) 2011 PAL Robotics sl. All Rights Reserved
 *  Created on: 14/10/2011
 *      Author: dani
 */

#ifndef PAL_BEHAVIOUR_SHAKEHAND_H
#define PAL_BEHAVIOUR_SHAKEHAND_H

#if 0 /* expanded by -frewrite-includes */
#include <behaviour/robotStateMachine/Task.h>
#endif /* expanded by -frewrite-includes */
# 12 "/home/dani/branches_svn/OROCOS_2.X/robot/sources/src/behaviour/robotTask/ShakeHand.h"


namespace pal
{
  namespace behaviour
  {
    namespace robotStateMachine
    {

      /**
       * ShakeHand Task.
       */
      class ShakeHand : public Task
      {
      public:
        ShakeHand();
        virtual ~ShakeHand();
        static bool staticInit(interaction::stateMachine::StateMachineType& sm);
        virtual void addActions(interaction::stateMachine::EventArguments const &args);

      }; //ShakeHand

    }
  }
}

#endif // SHAKEHAND_H
# 10 "/home/dani/branches_svn/OROCOS_2.X/robot/sources/src/behaviour/robotTask/ShakeHand.cpp" 2
namespace pal
{
  namespace behaviour
  {
    namespace robotStateMachine
    {
    
    ShakeHand::ShakeHand() : behaviour::robotStateMachine::Task("gui::TaskNames::getName(gui::ShakeHand)", gui::ShakeHand)
      {
      }
    }
  }
}

