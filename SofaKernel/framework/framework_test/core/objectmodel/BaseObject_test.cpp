#include <iostream>

#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject;

#include <sofa/helper/testing/BaseTest.h>
using sofa::helper::testing::BaseTest ;

struct D : public BaseObject {
    SOFA_CLASS(D, BaseObject)
    virtual void init() override
    {
        std::cout << D::getClass()->className << std::endl;
    }
};

struct A : public virtual D
{
    SOFA_CLASS(A, D)

    virtual void init() override
    {
        std::cout << A::getClass()->className << std::endl;
    }
};

struct B : public virtual D
{
    SOFA_CLASS(B, D)

    virtual void init() override
    {
        std::cout << B::getClass()->className << std::endl;
    }
};

struct C : public A, B
{
    SOFA_CLASS2(C, A, B)

    virtual void init() override
    {
        std::cout << C::getClass()->className << std::endl;
    }
};


struct BaseObject_test : public BaseTest
{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    void test_initCascade()
    {
        C c;
        c.Init();
    }
};

TEST_F(BaseObject_test, test_initCascade)
{
    this->test_initCascade();
}
