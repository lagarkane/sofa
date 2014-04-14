#ifndef GLPICKEDPOINT_H
#define GLPICKEDPOINT_H
#include <iostream>
#include <sofa/core/behavior/BaseMechanicalState.h>

using sofa::core::behavior::BaseMechanicalState;
typedef std::size_t nat;
typedef sofa::defaulttype::Vector3 Vec3;

namespace sofa{
namespace newgui{

/**
 * @brief The PickedPoint struct represents a vertex of a State, typically picked using the mouse
 * @author Francois Faure, 2014
 */
struct PickedPoint
{
    BaseMechanicalState::SPtr state; ///< the DOF of the picked object
    nat index;                       ///< index of the particle picked
    Vec3 point;                      ///< location of the picked particle in world space

    /// Conversion to boolean for easier test writing
    operator bool() const { return state != NULL; }

    PickedPoint()
    {
        state=0;    // default value is converted to false
    }

    inline friend std::ostream& operator << ( std::ostream& out, const PickedPoint p){
        out << "state: " << p.state->getName() << ", index: " << p.index << ", point: " << p.point;
        return out;
    }


};

}
}

#endif // GLPICKEDPOINT_H