#include <tesseract_scene_graph/macros.h>
TESSERACT_SCENE_GRAPH_IGNORE_WARNINGS_PUSH
#include <gtest/gtest.h>
//#include <boost/graph/filtered_graph.hpp>
#include <iostream>
#include <fstream>
TESSERACT_SCENE_GRAPH_IGNORE_WARNINGS_POP

#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/parser/mesh_parser.h>

TEST(TesseractSceneGraphUnit, TesseractSceneGraphUnit)
{
  using namespace tesseract_scene_graph;
  SceneGraph g;

  LinkPtr link_1(new Link("link_1"));
  LinkPtr link_2(new Link("link_2"));
  LinkPtr link_3(new Link("link_3"));
  LinkPtr link_4(new Link("link_4"));
  LinkPtr link_5(new Link("link_5"));

  g.addLink(link_1);
  g.addLink(link_2);
  g.addLink(link_3);
  g.addLink(link_4);
  g.addLink(link_5);

  JointPtr joint_1(new Joint("joint_1"));
  joint_1->parent_to_joint_origin_transform.translation()(0) = 1.25;
  joint_1->parent_link_name = "link_1";
  joint_1->child_link_name = "link_2";
  joint_1->type = JointType::FIXED;
  g.addJoint(joint_1);

  JointPtr joint_2(new Joint("joint_2"));
  joint_2->parent_to_joint_origin_transform.translation()(0) = 1.25;
  joint_2->parent_link_name = "link_2";
  joint_2->child_link_name = "link_3";
  joint_2->type = JointType::PLANAR;
  g.addJoint(joint_2);

  JointPtr joint_3(new Joint("joint_3"));
  joint_3->parent_to_joint_origin_transform.translation()(0) = 1.25;
  joint_3->parent_link_name = "link_3";
  joint_3->child_link_name = "link_4";
  joint_3->type = JointType::FLOATING;
  g.addJoint(joint_3);

  JointPtr joint_4(new Joint("joint_4"));
  joint_4->parent_to_joint_origin_transform.translation()(1) = 1.25;
  joint_4->parent_link_name = "link_2";
  joint_4->child_link_name = "link_5";
  joint_4->type = JointType::REVOLUTE;
  g.addJoint(joint_4);

  // Save Graph
  g.saveDOT("/tmp/graph_acyclic_tree_example.dot");

  // Test if the graph is Acyclic
  std::cout << "Is Acyclic: " << g.isAcyclic() << std::endl;
  EXPECT_TRUE(g.isAcyclic());

  // Test if the graph is Tree
  std::cout << "Is Tree: " << g.isTree() << std::endl;
  EXPECT_TRUE(g.isTree());

  // Test for unused links
  LinkPtr link_6(new Link("link_6"));
  g.addLink(link_6);
  std::cout << "Free Link, Is Tree: " << g.isTree() << std::endl;
  EXPECT_FALSE(g.isTree());
  g.removeLink(link_6->getName());
  std::cout << "Free Link Removed, Is Tree: " << g.isTree() << std::endl;
  EXPECT_TRUE(g.isTree());

  JointPtr joint_5(new Joint("joint_5"));
  joint_5->parent_to_joint_origin_transform.translation()(1) = 1.25;
  joint_5->parent_link_name = "link_5";
  joint_5->child_link_name = "link_4";
  joint_5->type = JointType::CONTINUOUS;
  g.addJoint(joint_5);

  // Save Graph
  g.saveDOT("/tmp/graph_acyclic_not_tree_example.dot");

  // Test if the graph is Acyclic
  std::cout << "Is Acyclic: " << g.isAcyclic() << std::endl;
  EXPECT_TRUE(g.isAcyclic());

  // Test if the graph is Tree
  std::cout << "Is Tree: " << g.isTree() << std::endl;
  EXPECT_FALSE(g.isTree());

  JointPtr joint_6(new Joint("joint_6"));
  joint_6->parent_to_joint_origin_transform.translation()(1) = 1.25;
  joint_6->parent_link_name = "link_5";
  joint_6->child_link_name = "link_1";
  joint_6->type = JointType::CONTINUOUS;
  g.addJoint(joint_6);

  // Save Graph
  g.saveDOT("/tmp/graph_cyclic_not_tree_example.dot");

  // Test if the graph is Acyclic
  std::cout << "Is Acyclic: " << g.isAcyclic() << std::endl;
  EXPECT_FALSE(g.isAcyclic());

  // Test if the graph is Tree
  std::cout << "Is Tree: " << g.isTree() << std::endl;
  EXPECT_FALSE(g.isTree());

  // Get Shortest Path
  SceneGraph::Path path = g.getShortestPath("link_1", "link_4");

// Todo:: Look at using filtered graph for chains and subgraphs
// boost::filtered_graph<

  std::cout << path << std::endl;
  EXPECT_TRUE(path.first.size() == 4);
  EXPECT_TRUE(std::find(path.first.begin(), path.first.end(), "link_1") != path.first.end());
  EXPECT_TRUE(std::find(path.first.begin(), path.first.end(), "link_2") != path.first.end());
  EXPECT_TRUE(std::find(path.first.begin(), path.first.end(), "link_3") != path.first.end());
  EXPECT_TRUE(std::find(path.first.begin(), path.first.end(), "link_4") != path.first.end());
  EXPECT_TRUE(path.second.size() == 3);
  EXPECT_TRUE(std::find(path.second.begin(), path.second.end(), "joint_1") != path.second.end());
  EXPECT_TRUE(std::find(path.second.begin(), path.second.end(), "joint_2") != path.second.end());
  EXPECT_TRUE(std::find(path.second.begin(), path.second.end(), "joint_3") != path.second.end());

  std::cout << (g.getName().c_str()) << std::endl;
}

TEST(TesseractSceneGraphUnit, LoadMeshUnit)
{
  using namespace tesseract_scene_graph;

  std::string mesh_file = std::string(DATA_DIR) + "/sphere_p25m.stl";
  std::vector<tesseract_geometry::MeshPtr> meshes = createMeshFromPath(mesh_file);
  EXPECT_TRUE(meshes.size() == 1);
  EXPECT_TRUE(meshes[0]->getTriangleCount() == 80);
  EXPECT_TRUE(meshes[0]->getVerticeCount() == 42);

  mesh_file = std::string(DATA_DIR) + "/sphere_p25m.ply";
  meshes = createMeshFromPath(mesh_file);
  EXPECT_TRUE(meshes.size() == 1);
  EXPECT_TRUE(meshes[0]->getTriangleCount() == 80);
  EXPECT_TRUE(meshes[0]->getVerticeCount() == 42);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}