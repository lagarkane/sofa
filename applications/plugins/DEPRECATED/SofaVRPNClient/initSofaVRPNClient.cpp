/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/helper/system/config.h>

#include <vrpnclient_config.h>

namespace sofa
{

namespace component
{

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
    SOFA_SOFAVRPNCLIENT_API void initExternalModule();
    SOFA_SOFAVRPNCLIENT_API const char* getModuleName();
    SOFA_SOFAVRPNCLIENT_API const char* getModuleVersion();
    SOFA_SOFAVRPNCLIENT_API const char* getModuleLicense();
    SOFA_SOFAVRPNCLIENT_API const char* getModuleDescription();
    SOFA_SOFAVRPNCLIENT_API const char* getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }
}

const char* getModuleName()
{
    return "Plugin SofaVRPN";
}

const char* getModuleVersion()
{
    return "beta 1.0";
}

const char* getModuleLicense()
{
    return "LGPL";
}

const char* getModuleDescription()
{
    return "VRPN support into SOFA ";
}

const char* getModuleComponentList()
{
    return "MyFakeComponent, OtherFakeComponent";
}



}

}


SOFA_LINK_CLASS(VRPNTracker)
SOFA_LINK_CLASS(VRPNButton)
SOFA_LINK_CLASS(VRPNAnalog)
