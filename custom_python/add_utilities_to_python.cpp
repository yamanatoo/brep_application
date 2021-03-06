// see brep_application/LICENSE.txt
//
//   Project Name:        Kratos
//   Last Modified by:    $Author: hbui $
//   Date:                $Date: 23 Aug 2019 $
//   Revision:            $Revision: 1.0 $
//
//

// External includes


// Project includes
#include "custom_python/add_utilities_to_python.h"
#ifdef BREP_APPLICATION_USE_OPENCASCADE
#include "custom_utilities/occ_utility.h"
#endif
#include "custom_utilities/brep_utility.h"
#include "custom_utilities/brep_mesh_utility.h"
#include "custom_utilities/delaunay.h"
#include "custom_utilities/tube_mesher.h"

namespace Kratos
{

namespace Python
{

using namespace boost::python;

std::size_t BRepUtility_GetLastNodeId(BRepUtility& rDummy, ModelPart& r_model_part)
{
    return rDummy.GetLastNodeId(r_model_part);
}

std::size_t BRepUtility_GetLastElementId(BRepUtility& rDummy, ModelPart& r_model_part)
{
    return rDummy.GetLastElementId(r_model_part);
}

std::size_t BRepUtility_GetLastConditionId(BRepUtility& rDummy, ModelPart& r_model_part)
{
    return rDummy.GetLastConditionId(r_model_part);
}

std::size_t BRepUtility_GetLastPropertiesId(BRepUtility& rDummy, ModelPart& r_model_part)
{
    return rDummy.GetLastPropertiesId(r_model_part);
}

boost::python::list BRepMeshUtility_CreateTriangleConditions(BRepMeshUtility& rDummy,
    ModelPart& r_model_part,
    const std::string& sample_condition_name,
    const int& type, // if 1: generate T3 elements; 2: T6 elements;
    const array_1d<double, 3>& rCenter,
    const array_1d<double, 3>& rNormal,
    const double& radius, const std::size_t& nsampling_axial, const std::size_t& nsampling_radial,
    const int& activation_level,
    Properties::Pointer pProperties)
{
    Element::GeometryType::PointType::PointType C, N;
    noalias(C) = rCenter;
    noalias(N) = rNormal;
    BRepMeshUtility::ConditionMeshInfoSimpleType Results = rDummy.CreateTriangleConditions(r_model_part,
        sample_condition_name, type, C, N, radius, nsampling_axial, nsampling_radial,
        activation_level, pProperties);

    boost::python::list Output;
    Output.append(std::get<0>(Results));
    Output.append(std::get<1>(Results));
    return Output;
}

boost::python::list TubeMesher_GetPoints(TubeMesher& dummy)
{
    boost::python::list point_list;
    for (std::size_t i = 0; i < dummy.GetPoints().size(); ++i)
        point_list.append(dummy.GetPoints()[i]);
    return point_list;
}

boost::python::list TubeMesher_GetElements(TubeMesher& dummy)
{
    boost::python::list element_list;
    for (std::size_t i = 0; i < dummy.GetElements().size(); ++i)
    {
        boost::python::list tmp1;
        for (std::size_t j = 0; j < dummy.GetElements()[i].size(); ++j)
        {
            boost::python::list tmp2;
            for (std::size_t k = 0; k < dummy.GetElements()[i][j].size(); ++k)
            {
                boost::python::list tmp3;
                for (std::size_t l = 0; l < dummy.GetElements()[i][j][k].size(); ++l)
                {
                    boost::python::list tmp4;
                    for (std::size_t m = 0; m < dummy.GetElements()[i][j][k][l].size(); ++m)
                        tmp4.append(dummy.GetElements()[i][j][k][l][m]);
                    tmp3.append(tmp4);
                }
                tmp2.append(tmp3);
            }
            tmp1.append(tmp2);
        }
        element_list.append(tmp1);
    }
    return element_list;
}

boost::python::list TubeMesher_GetConditions(TubeMesher& dummy)
{
    boost::python::list condition_list;
    for (std::size_t i = 0; i < dummy.GetConditions().size(); ++i)
    {
        boost::python::list tmp1;
        for (std::size_t j = 0; j < dummy.GetConditions()[i].size(); ++j)
        {
            boost::python::list tmp2;
            for (std::size_t k = 0; k < dummy.GetConditions()[i][j].size(); ++k)
            {
                boost::python::list tmp3;
                for (std::size_t l = 0; l < dummy.GetConditions()[i][j][k].size(); ++l)
                    tmp3.append(dummy.GetConditions()[i][j][k][l]);
                tmp2.append(tmp3);
            }
            tmp1.append(tmp2);
        }
        condition_list.append(tmp1);
    }
    return condition_list;
}

boost::python::list TubeMesher_GetSlices1(TubeMesher& dummy, const std::size_t& slice,
        const std::size_t& layer, const std::size_t& sub_layer)
{
    std::vector<std::vector<std::size_t> > conditions;
    dummy.GetSlices(conditions, slice, layer, sub_layer);

    boost::python::list condition_list;
    for (std::size_t i = 0; i < conditions.size(); ++i)
    {
        boost::python::list tmp1;
        for (std::size_t j = 0; j < conditions[i].size(); ++j)
            tmp1.append(conditions[i][j]);
        condition_list.append(tmp1);
    }
    return condition_list;
}

boost::python::list TubeMesher_GetSlices2(TubeMesher& dummy, const std::size_t& slice,
        const std::size_t& layer)
{
    std::vector<std::vector<std::vector<std::size_t> > > conditions;
    dummy.GetSlices(conditions, slice, layer);

    boost::python::list condition_list;
    for (std::size_t i = 0; i < conditions.size(); ++i)
    {
        boost::python::list tmp1;
        for (std::size_t j = 0; j < conditions[i].size(); ++j)
        {
            boost::python::list tmp2;
            for (std::size_t k = 0; k < conditions[i][j].size(); ++k)
                tmp2.append(conditions[i][j][k]);
            tmp1.append(tmp2);
        }
        condition_list.append(tmp1);
    }
    return condition_list;
}

class TubeMesherWrapper
{
public:

    static TubeMesher::Pointer initWrapper(const Curve::Pointer pCurve, boost::python::list r_list,
        boost::python::list nsamping_layers,
        const std::size_t& nsampling_axial, const std::size_t& nsampling_radial,
        const double& rotate_angle, const double& start_angle, const double& end_angle,
        const double& tmin, const double& tmax,
        const int& type, const std::size_t& last_node_id)
    {
        std::vector<double> r_vec;
        for (int i = 0; i < boost::python::len(r_list); ++i)
            r_vec.push_back(boost::python::extract<double>(r_list[i]));

        std::vector<std::size_t> nsamping_layers_vec;
        for (int i = 0; i < boost::python::len(nsamping_layers); ++i)
            nsamping_layers_vec.push_back(static_cast<std::size_t>(boost::python::extract<int>(nsamping_layers[i])));

        return TubeMesher::Pointer(new TubeMesher(pCurve, r_vec, nsamping_layers_vec, nsampling_axial, nsampling_radial,
            rotate_angle, start_angle, end_angle, tmin, tmax, type, last_node_id));
    }
};

void BRepApplication_AddUtilitiesToPython()
{

    #ifdef BREP_APPLICATION_USE_OPENCASCADE
    class_<OCCUtility, OCCUtility::Pointer, boost::noncopyable>
    ( "OCCUtility", init<>() )
    .def("MakeBottle", &OCCUtility::MakeBottle)
    .def("MakeSphere", &OCCUtility::MakeSphere)
    .def("ReadSTEP", &OCCUtility::ReadSTEP)
    .def("WriteSTEP", &OCCUtility::WriteSTEP)
    .def(self_ns::str(self))
    ;
    #endif

    class_<BRepUtility, BRepUtility::Pointer, boost::noncopyable>
    ("BRepUtility", init<>())
    .def("GetLastNodeId", &BRepUtility_GetLastNodeId)
    .def("GetLastElementId", &BRepUtility_GetLastElementId)
    .def("GetLastConditionId", &BRepUtility_GetLastConditionId)
    .def("GetLastPropertiesId", &BRepUtility_GetLastPropertiesId)
    ;

    class_<BRepMeshUtility, BRepMeshUtility::Pointer, boost::noncopyable>
    ("BRepMeshUtility", init<>())
    .def("CreateTriangleConditions", &BRepMeshUtility_CreateTriangleConditions)
    ;

    class_<TubeMesher, TubeMesher::Pointer, boost::noncopyable>
    ("TubeMesher", no_init)
    .def("__init__", make_constructor(&TubeMesherWrapper::initWrapper))
    .def("GetPoints", &TubeMesher_GetPoints)
    .def("GetElements", &TubeMesher_GetElements)
    .def("GetConditions", &TubeMesher_GetConditions)
    .def("GetSlices", &TubeMesher_GetSlices1)
    .def("GetSlices", &TubeMesher_GetSlices2)
    ;

    void(Delaunay::*pointer_to_addPoint)(const double&, const double&) = &Delaunay::addPoint;
    class_<Delaunay, boost::shared_ptr<Delaunay>, boost::noncopyable>
    ("Delaunay", init<const double&, const double&, const double&, const double&>())
    .def("AddPoint", pointer_to_addPoint)
    .def("Print", &Delaunay::Print)
    ;

}
}  // namespace Python.
}  // namespace Kratos.

