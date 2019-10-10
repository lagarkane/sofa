#pragma once

#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/Data.h>

namespace sofa
{
namespace core
{
namespace objectmodel
{

/// Alias for Data<T>, to differentiate between inputs, outputs and Data fields in DataEngine.
/// Contrary to Datas, InputDatas are NOT registered in Base::m_vecData, and are instead stored in DDGNode::inputs
template < class T = void* >
class InputData : public Data<T>
{
    void addInputToEngine(T* type = nullptr)
    {
        if (this->m_owner)
        {
            DataEngine* engine = dynamic_cast<DataEngine*>(this->m_owner);
            if (engine)
            {
                engine->removeData(this); // removes this input data from Base::m_vecData
                engine->addInput(this); // adds this input data to DDGNode::inputs
            }
        }
    }
public:
    explicit InputData(const BaseData::BaseInitData& init)
        : Data<T>(init)
    {
        addInputToEngine();
    }

    explicit InputData(const typename Data<T>::InitData& init)
        : Data<T>(init)
    {
        addInputToEngine();
    }

    [[deprecated("Replaced with one with std::string instead of char* version")]]
    InputData( const char* helpMsg=nullptr, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(helpMsg, isDisplayed, isReadOnly)
    {
        addInputToEngine();
    }

    InputData( const std::string& helpMsg, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(helpMsg, isDisplayed, isReadOnly)
    {
        addInputToEngine();
    }


    InputData( const T& value, const char* helpMsg=nullptr, bool isDisplayed=true, bool isReadOnly=false) :
        Data<T>(value, helpMsg, isDisplayed, isReadOnly)
    {
        addInputToEngine();
    }

    InputData( const T& value, const std::string& helpMsg, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(value, helpMsg, isDisplayed, isReadOnly)
    {
        addInputToEngine();
    }

    /// Destructor.
    virtual ~InputData()
    {}
};

/// Alias for Data<T>, to differentiate between inputs, outputs and Data fields in DataEngine.
/// Contrary to Datas, OutputDatas are NOT registered in Base::m_vecData, and are instead stored in DDGNode::outputs
template < class T = void* >
class OutputData : public Data<T>
{
private:
    void addOutputToEngine()
    {
        if (this->m_owner)
        {
            DataEngine* engine = dynamic_cast<DataEngine*>(this->m_owner);
            if (engine)
            {
                engine->removeData(this); // removes this input data from Base::m_vecData
                engine->addOutput(this); // adds this input data to DDGNode::inputs
            }
        }
    }
public:
    explicit OutputData(const BaseData::BaseInitData& init)
        : Data<T>(init)
    {
        addOutputToEngine();
    }

    explicit OutputData(const typename Data<T>::InitData& init)
        : Data<T>(init)
    {
        addOutputToEngine();
    }

    [[deprecated("Replaced with one with std::string instead of char* version")]]
    OutputData( const char* helpMsg=nullptr, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(helpMsg, isDisplayed, isReadOnly)
    {
        addOutputToEngine();
    }

    OutputData( const std::string& helpMsg, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(helpMsg, isDisplayed, isReadOnly)
    {
        addOutputToEngine();
    }


    OutputData( const T& value, const char* helpMsg=nullptr, bool isDisplayed=true, bool isReadOnly=false) :
        Data<T>(value, helpMsg, isDisplayed, isReadOnly)
    {
        addOutputToEngine();
    }

    OutputData( const T& value, const std::string& helpMsg, bool isDisplayed=true, bool isReadOnly=false)
        : Data<T>(value, helpMsg, isDisplayed, isReadOnly)
    {
        addOutputToEngine();
    }

    /// Destructor.
    virtual ~OutputData()
    {}
};

}  // namespace objectmodel
}  // namespace core

// the Data class is used everywhere
using core::objectmodel::InputData;
using core::objectmodel::OutputData;

}  // namespace sofa
