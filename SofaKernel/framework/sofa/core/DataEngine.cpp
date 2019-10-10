/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <sofa/core/DataEngine.h>

namespace sofa
{

namespace core
{

DataEngine::DataEngine()
{
    addLink(&(this->core::objectmodel::DDGNode::inputs));
    addLink(&(this->core::objectmodel::DDGNode::outputs));
}

DataEngine::~DataEngine()
{
}


/// Add a new input to this engine
void DataEngine::addInput(objectmodel::BaseData* n)
{
    m_dataTracker.trackData(*n);
    if (n->getOwner() == this && n->getGroup().empty())
        n->setGroup("Inputs"); // set the group of input Datas if not yet set
    core::objectmodel::DDGNode::addInput(n);
    m_inputDataFields.push_back(n);
    setDirtyValue();
}

/// Add a new output to this engine
void DataEngine::addOutput(objectmodel::BaseData* n)
{
    if (n->getOwner() == this && (!n->getGroup() || !n->getGroup()[0]))
        n->setGroup("Outputs"); // set the group of output Datas if not yet set
    core::objectmodel::DDGNode::addOutput(n);
    m_outputDataFields.push_back(n);
    setDirtyValue();
}

/// Add a new input to this engine
void DataEngine::removeInput(objectmodel::BaseData* n)
{
    m_inputDataTracker.untrackData(*n);
    if (n->getOwner() == this)
        m_inputDataFields.erase(std::find(m_inputDataFields.begin(), m_inputDataFields.end(), n));
    core::objectmodel::DDGNode::delInput(n);
    setDirtyValue();
}

/// Add a new output to this engine
void DataEngine::removeOutput(objectmodel::BaseData* n)
{
    if (n->getOwner() == this)
        m_inputDataFields.erase(std::find(m_outputDataFields.begin(), m_outputDataFields.end(), n));
    core::objectmodel::DDGNode::delOutput(n);
    setDirtyValue();
}

void DataEngine::updateAllInputs()
{
    for(auto& input : getInputs())
    {
        static_cast<sofa::core::objectmodel::BaseData*>(input)
                ->updateIfDirty();
    }
}

void DataEngine::update()
{
    updateAllInputs();
    DDGNode::cleanDirty();
    onUpdate();
    m_inputDataTracker.clean();
}

bool DataEngine::hasInputDataChanged(const objectmodel::BaseData& input)
{
    return m_inputDataTracker.hasChanged(input);
}

bool DataEngine::hasInputDataChanged()
{
    return m_inputDataTracker.hasChanged();
}



void DataEngine::cleanDirty(const core::ExecParams* params)
{
    core::objectmodel::DDGNode::cleanDirty(params);

    /// it is also time to clean the tracked Data
    m_inputDataTracker.clean();
}

} // namespace core

} // namespace sofa
