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
//  Date:            10 Feb 2017
//


#if !defined(KRATOS_CIRCULAR_LEVEL_SET_H_INCLUDED )
#define  KRATOS_CIRCULAR_LEVEL_SET_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "custom_algebra/level_set/level_set.h"
#include "custom_utilities/brep_mesh_utility.h"


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
/** Detail class definition.
*/
class CircularLevelSet : public LevelSet
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of CircularLevelSet
    KRATOS_CLASS_POINTER_DEFINITION(CircularLevelSet);

    typedef LevelSet BaseType;

    static constexpr double PI = std::atan(1.0)*4;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    CircularLevelSet(const double& cX, const double& cY, const double& R)
    : BaseType(), mcX(cX), mcY(cY), mR(R)
    {}

    /// Copy constructor.
    CircularLevelSet(CircularLevelSet const& rOther)
    : BaseType(rOther), mcX(rOther.mcX), mcY(rOther.mcY), mR(rOther.mR)
    {}

    /// Destructor.
    virtual ~CircularLevelSet() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{


    virtual LevelSet::Pointer CloneLevelSet() const
    {
        return LevelSet::Pointer(new CircularLevelSet(*this));
    }


    virtual std::size_t WorkingSpaceDimension() const
    {
        return 2;
    }


    virtual double GetValue(const PointType& P) const
    {
        return pow(P(0) - mcX, 2) + pow(P(1) - mcY, 2) - pow(mR, 2);
    }


    virtual Vector GetGradient(const PointType& P) const
    {
        Vector grad(3);
        grad(0) = 2.0 * (P(0) - mcX);
        grad(1) = 2.0 * (P(1) - mcY);
        grad(2) = 0.0;
        return grad;
    }


    virtual Matrix GetGradientDerivatives(const PointType& P) const
    {
        Matrix Jac(3, 3);
        noalias(Jac) = ZeroMatrix(3, 3);

        Jac(0, 0) = 2.0;
        Jac(0, 1) = 0.0;

        Jac(1, 0) = 0.0;
        Jac(1, 1) = 2.0;

        return Jac;
    }


    /// Generate the sampling points on the level set surface
    std::vector<PointType> GeneratePoints(const double& start_angle, const double& end_angle,
        const std::size_t& nsampling_radial) const
    {
        std::vector<PointType> radial_points(nsampling_radial);
        double small_angle = (end_angle - start_angle) / nsampling_radial;

        PointType V;
        V[2] = 0.0;
        double d;
        for (std::size_t j = 0; j < nsampling_radial; ++j)
        {
            d = start_angle + j*small_angle;
            V[0] = mcX + mR*std::cos(d);
            V[1] = mcY + mR*std::sin(d);
            noalias(radial_points[j]) = V;
        }

        return radial_points;
    }


    /// Generate the sampling points on the level set surface
    std::vector<PointType> GeneratePoints(const std::size_t& nsampling_radial) const
    {
        return GeneratePoints(0.0, 2*PI, nsampling_radial);
    }


    /// Create the elements based on sampling points on the line
    std::pair<ModelPart::NodesContainerType, ModelPart::ElementsContainerType> CreateLineElements(ModelPart& r_model_part,
        const std::string& sample_element_name,
        Properties::Pointer pProperties,
        const double& start_angle,
        const double& end_angle,
        const std::size_t& nsampling_radial,
        const bool close = false) const
    {
        // firstly create the sampling points on surface
        std::vector<PointType> sampling_points = this->GeneratePoints(start_angle, end_angle, nsampling_radial);
        int order = 1;
        BRepMeshUtility::ElementMeshInfoType Info = BRepMeshUtility::CreateLineElements(r_model_part, sampling_points, sample_element_name, order, close, pProperties);
        return std::make_pair(std::get<0>(Info), std::get<1>(Info));
    }

    /// projects a point on the surface of level_set
    virtual void ProjectOnSurface(const PointType& P, PointType& Proj) const
    {
        double vector_length = sqrt(pow(P(0)-mcX, 2) + pow(P(1)-mcY, 2));
        if (vector_length == 0)
            KRATOS_THROW_ERROR(std::invalid_argument, "trying to project node that's in the center of Brep circle", "");

        Proj(0) = (P(0) - mcX) * mR / vector_length + mcX;
        Proj(1) = (P(1) - mcY) * mR / vector_length + mcY;
        Proj(2) = 0.0;
    }

    /// compute the derivatives of the projection point w.r.t to the original point.
    virtual void ProjectionDerivatives(const PointType& P, Matrix& Derivatives) const
    {
        if (Derivatives.size1() != 3 || Derivatives.size2() != 3)
            Derivatives.resize(3, 3, false);

        double vector_length = sqrt(pow(P(0)-mcX, 2) + pow(P(1)-mcY, 2));
        if (vector_length == 0)
            KRATOS_THROW_ERROR(std::invalid_argument, "trying to project node that's in the center of Brep circle", "");

        noalias(Derivatives) = ZeroMatrix(3, 3);

        Vector dvector_length(2);
        dvector_length(0) = 2.0*(P(0) - mcX);
        dvector_length(1) = 2.0*(P(1) - mcY);

        Derivatives(0, 0) = mR/vector_length - (P(0)-mcX)*mR*dvector_length(0)/pow(vector_length, 2);
        Derivatives(0, 1) = -(P(0)-mcX)*mR*dvector_length(1)/pow(vector_length, 2);

        Derivatives(1, 0) = -(P(1)-mcY)*mR*dvector_length(0)/pow(vector_length, 2);
        Derivatives(1, 1) = mR/vector_length - (P(1)-mcY)*mR*dvector_length(1)/pow(vector_length, 2);
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
        return "Circular Level Set";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << this->Info();
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        rOStream << "cX: " << mcX << ", cY: " << mcY << ", R: " << mR;
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


    double mcX, mcY, mR;


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
    CircularLevelSet& operator=(CircularLevelSet const& rOther);

    ///@}

}; // Class CircularLevelSet

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream, CircularLevelSet& rThis)
{
    return rIStream;
}

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream, const CircularLevelSet& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << " ";
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_CIRCULAR_LEVEL_SET_H_INCLUDED  defined
