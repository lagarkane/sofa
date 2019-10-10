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
#ifndef SOFA_CORE_DATAENGINE_H
#define SOFA_CORE_DATAENGINE_H

#include <sofa/core/core.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/DataTracker.h>

namespace sofa
{

namespace core
{

namespace objectmodel
{
template <class T>
class InputData;

template <class T>
class OutputData;
}

/**
 *  \brief from a set of Data inputs computes a set of Data outputs
 *
 * Implementation good rules:
 *
 * void init()
 * {
 *    addInput // indicate all inputs
 *    addOutput // indicate all outputs
 * }
 *
 * // optional (called each time a data is modified in the gui)
 * // it is not always desired
 * void reinit()
 * {
 *    update();
 * }
 *
 * void onUpdate() override
 * {
 *    access your inputs, set your outputs...
 * }
 *
 */
class SOFA_CORE_API DataEngine : public core::objectmodel::DDGNode, public virtual core::objectmodel::BaseObject
{
public:
    SOFA_ABSTRACT_CLASS2(DataEngine, core::objectmodel::BaseObject, core::objectmodel::DDGNode);
    SOFA_BASE_CAST_IMPLEMENTATION(DataEngine)

    DataEngine(const DataEngine& n) ;
    DataEngine& operator=(const DataEngine& n) ;

    /// Called in update(), back-propagates the data update
    /// in the data dependency graph
    void updateAllInputs();

protected:
    /// Constructor
    DataEngine();

    /// Destructor. Do nothing
    ~DataEngine() override;


private:
    /// @name Tracking Data mechanism
    /// each Data added to the DataTracker
    /// is tracked to be able to check if its value changed
    /// since their last clean, called by default
    /// in DataEngine::cleanDirty().
    /// @{

    DataTracker m_inputDataTracker;
    ///@}

    VecData m_inputDataFields;
    VecData m_outputDataFields;

protected:
    /// Where you put your engine's impl
    virtual void onUpdate() = 0;

public:
    /// Updates your inputs and calls cleanDirty() for you.
    /// User implementation moved to onUpdate()
    void update() final;

    bool hasInputDataChanged(const objectmodel::BaseData& input);
    bool hasInputDataChanged();

    /// Set dirty flag to false
    /// for the DDGNode and for all the tracked Data
    virtual void cleanDirty(const core::ExecParams* params = nullptr);

    typedef helper::vector<objectmodel::BaseData*> VecData;

    /// Get the list of input data fields for this Engine
    const VecData& getInputDataFields();

    /// Get the list of output data fields for this Engine
    const VecData& getOutputDataFields();


    /// used by InputData to register itself in DataEngine
    void addInput(objectmodel::BaseData* data);
    /// used by OutputData to register itself in DataEngine
    void addOutput(objectmodel::BaseData* data);
    /// used to unregister an InputData from this DataEngine
    void removeInput(objectmodel::BaseData* data);
    /// used to unregister a OutputData from this DataEngine
    void removeOutput(objectmodel::BaseData* data);

    // The methods below must be redefined because of the
    // double inheritance from Base and DDGNode

    /// @name Class reflection system
    /// @{

    template<class T>
    static std::string typeName(const T* ptr= NULL)
    {
        return core::objectmodel::BaseObject::typeName(ptr);
    }

    /// Helper method to get the class name of a type derived from this class
    ///
    /// This method should be used as follow :
    /// \code  T* ptr = NULL; std::string type = T::className(ptr); \endcode
    /// This way derived classes can redefine the className method
    template<class T>
    static std::string className(const T* ptr= NULL)
    {
        return core::objectmodel::BaseObject::className(ptr);
    }

    /// Helper method to get the namespace name of a type derived from this class
    ///
    /// This method should be used as follow :
    /// \code  T* ptr = NULL; std::string type = T::namespaceName(ptr); \endcode
    /// This way derived classes can redefine the namespaceName method
    template<class T>
    static std::string namespaceName(const T* ptr= NULL)
    {
        return core::objectmodel::BaseObject::namespaceName(ptr);
    }

    /// Helper method to get the template name of a type derived from this class
    ///
    /// This method should be used as follow :
    /// \code  T* ptr = NULL; std::string type = T::templateName(ptr); \endcode
    /// This way derived classes can redefine the templateName method
    template<class T>
    static std::string templateName(const T* ptr= NULL)
    {
        return core::objectmodel::BaseObject::templateName(ptr);
    }

    /// Helper method to get the shortname of a type derived from this class.
    /// The default implementation return the class name.
    ///
    /// This method should be used as follow :
    /// \code  T* ptr = NULL; std::string type = T::shortName(ptr); \endcode
    /// This way derived classes can redefine the shortName method
    template< class T>
    static std::string shortName( const T* ptr = NULL, core::objectmodel::BaseObjectDescription* desc = NULL )
    {
        return core::objectmodel::BaseObject::shortName(ptr,desc);
    }

    template<class T>
    static void dynamicCast(T*& ptr, Base* b)
    {
        core::objectmodel::BaseObject::dynamicCast(ptr, b);
    }

    /// @}

    /// This method is needed by DDGNode
    const std::string& getName() const override
    {
        return objectmodel::BaseObject::getName();
    }

    /// This method is needed by DDGNode
    objectmodel::Base* getOwner() const override
    {
        return const_cast<DataEngine*>(this);
    }

    /// This method is needed by DDGNode
    objectmodel::BaseData* getData() const override
    {
        return nullptr;
    }

    /// Add a link.
    void addLink(objectmodel::BaseLink* l)
    {
        objectmodel::BaseObject::addLink(l);
    }



    template<class SOFA_T>
    objectmodel::BaseData::BaseInitData initInput(Data<SOFA_T>* field, const char* name, const char* help,
                                                                objectmodel::BaseData::DataFlags dataflags)
    {
        objectmodel::BaseData::BaseInitData res;
        this->initData0(field, res, name, help, dataflags);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Inputs";
        addInput(field);
        return res;
    }

    template<class SOFA_T>
    objectmodel::BaseData::BaseInitData initOutput(Data<SOFA_T>* field, const char* name, const char* help,
                                                                objectmodel::BaseData::DataFlags dataflags)
    {
        objectmodel::BaseData::BaseInitData res;
        this->initData0(field, res, name, help, dataflags);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Outputs";
        addOutput(field);
        return res;
    }


    template<class SOFA_T> objectmodel::BaseData::BaseInitData
    initInput(objectmodel::Data<SOFA_T>* field, const char* name, const char* help,
             bool isDisplayed=true, bool isReadOnly=false)
    {
        ::sofa::core::objectmodel::BaseData::BaseInitData res;
        this->initData0(field, res, name, help,
                        isDisplayed, isReadOnly);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Inputs";
        addInput(field);
        return res;
    }

    template<class SOFA_T> objectmodel::BaseData::BaseInitData
    initOutput(objectmodel::Data<SOFA_T>* field, const char* name, const char* help,
             bool isDisplayed=true, bool isReadOnly=false)
    {
        ::sofa::core::objectmodel::BaseData::BaseInitData res;
        this->initData0(field, res, name, help,
                        isDisplayed, isReadOnly);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Outputs";
        addOutput(field);
        return res;
    }

    template<class SOFA_T> typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData initInput(
            ::sofa::core::objectmodel::Data<SOFA_T>* field, const SOFA_T& value, const char* name,
            const char* help, bool isDisplayed=true, bool isReadOnly=false)
    {
        typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData res;
        this->initData0(field, res, value, name, help,
                        isDisplayed, isReadOnly);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Inputs";
        addInput(field);
        return res;
    }
    template<class SOFA_T> typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData initOutput(
            ::sofa::core::objectmodel::Data<SOFA_T>* field, const SOFA_T& value, const char* name,
            const char* help, bool isDisplayed=true, bool isReadOnly=false)
    {
        typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData res;
        this->initData0(field, res, value, name, help,
                        isDisplayed, isReadOnly);
        res.ownerClass = GetClass()->className.c_str();
        res.group = "Outputs";
        addOutput(field);
        return res;
    }
};

} // namespace core

} // namespace sofa

#include <sofa/core/objectmodel/EngineData.h>

#endif
