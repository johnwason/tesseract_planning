/**
 * @file scene_graph_example_node.cpp
 * @brief Demonstrates manipulating the scene graph with a robot picking up another robot
 *
 * @author Matthew Powelson
 * @date Feb 17, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tesseract_examples/scene_graph_example.h>
#include <tesseract_common/filesystem.h>
#include <tesseract_environment/environment.h>
#include <tesseract_support/tesseract_support_resource_locator.h>

using namespace tesseract_examples;
using namespace tesseract_common;
using namespace tesseract_environment;

int main(int /*argc*/, char** /*argv*/)
{
  auto locator = std::make_shared<TesseractSupportResourceLocator>();
  tesseract_common::fs::path urdf_path =
      locator->locateResource("package://tesseract_support/urdf/scene_graph_example.urdf")->getFilePath();
  tesseract_common::fs::path srdf_path =
      locator->locateResource("package://tesseract_support/urdf/scene_graph_example.srdf")->getFilePath();
  auto env = std::make_shared<Environment>();
  if (!env->init(urdf_path, srdf_path, locator))
    exit(1);

  SceneGraphExample example(env, nullptr);
  if (!example.run())
    exit(1);
}
