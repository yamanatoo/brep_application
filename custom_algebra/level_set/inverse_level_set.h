//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         brep_application/LICENSE.txt
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Hoang-Giang Bui
//  Date:            15 Feb 2017
//


#if !defined(KRATOS_INVERSE_LEVEL_SET_H_INCLUDED )
#define  KRATOS_INVERSE_LEVEL_SET_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/element.h"
#include "includes/ublas_interface.h"
#include "custom_algebra/level_set/level_set.h"


namespace Kratos
{
///@addtogroup BRepApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.
/** Class for inverse of one level set
 * REF: Massing et al, CutFEM: Discretizing geometry and partial differential equations
*/
class InverseLevelSet : public LevelSet
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of InverseLevelSet
    KRATOS_CLASS_POINTER_DEFINITION(InverseLevelSet);

    typedef LevelSet BaseType;

    typedef typename Element::GeometryType GeometryType;

    typedef typename GeometryType::PointType NodeType;

    typedef typename NodeType::PointType PointType;

    typedef typename NodeType::CoordinatesArrayType CoordinatesArrayType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    InverseLevelSet(const BaseType::Pointer p_level_set)
    : BaseType(), mp_level_set(p_level_set)
    {}

    /// Copy constructor.
    InverseLevelSet(InverseLevelSet const& rOther)
    : BaseType(rOther), mp_level_set(rOther.mp_level_set->CloneLevelSet())
    {}

    /// Destructor.
    virtual ~InverseLevelSet() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{


    virtual LevelSet::Pointer CloneLevelSet() const
    {
        return LevelSet::Pointer(new InverseLevelSet(*this));
    }


    virtual std::size_t WorkingSpaceDimension() const
    {
        return mp_level_set->WorkingSpaceDimension();
    }


    virtual double GetValue(const PointType& P) const
    {
        return -mp_level_set->GetValue(P);
    }


    virtual Vector GetGradient(const PointType& P) const
    {
        return -mp_level_set->GetGradient(P);
    }


    /// Get the original level set
    BaseType::Pointer pLeveSet() const
    {
        return mp_level_set;
    }

    void pSetLevelSet(BaseType::Pointer p_level_set)
    {
        mp_level_set = p_level_set;
    }

    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        std::stringstream ss;
        ss << "Inverse Level Set of (" << mp_level_set->Info() << ")";
        return ss.str();
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
    }


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    BaseType::Pointer mp_level_set;


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    InverseLevelSet& operator=(InverseLevelSet const& rOther);

    ///@}

}; // Class InverseLevelSet

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream, InverseLevelSet& rThis)
{}

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream, const InverseLevelSet& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_INVERSE_LEVEL_SET_H_INCLUDED  defined
