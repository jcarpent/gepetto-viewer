//
//  test.cpp
//  Test for SceneViwer
//
//  Created by Mathieu Geisert in November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <iostream>
#include <Graphics/WindowManager.h>
#include <Graphics/Node.h>
#include <Graphics/GroupNode.h>
#include <Graphics/LeafNodeBox.h>
#include <Graphics/LeafNodeCapsule.h>
#include <Graphics/LeafNodeCone.h>
#include <Graphics/LeafNodeCylinder.h>
#include <Graphics/LeafNodeLine.h>
#include <Graphics/LeafNodeSphere.h>
#include <Graphics/LeafNodeGround.h>
#include <Graphics/LeafNodeCollada.h>
#include <Graphics/URDFparser.h>

  int main(int argc, const char * argv[])

  {
    using namespace Graphics;

    LeafNodeBoxPtr_t box = LeafNodeBox::create("box1", osgVector3(1.,1.,1.));
    LeafNodeCapsulePtr_t capsule = LeafNodeCapsule::create("capsule1", 1,1);
    LeafNodeConePtr_t cone = LeafNodeCone::create("cone", 1,1);
    LeafNodeCylinderPtr_t cylindre = LeafNodeCylinder::create("cylindre", 1,1);
    LeafNodeSpherePtr_t sphere = LeafNodeSphere::create("sphere", 1);
    LeafNodeGroundPtr_t ground = LeafNodeGround::create("ground");
    LeafNodeColladaPtr_t collada = LeafNodeCollada::create("collada","/local/mgeisert/devel/install/share/airbus_environment/urdf/meshes/screw_gun.dae");


    GroupNodePtr_t world = GroupNode::create(std::string("world"));
    //GroupNodePtr_t robot = GroupNode::create(std::string("robot"));
    GroupNodePtr_t robot = URDFparser::parse(std::string("hrp2"), std::string("/local/mgeisert/devel/src/hrp2/hrp2_14_description/urdf/hrp2_14_capsule.urdf"),std::string("/local/mgeisert/devel/src/hrp2/"));
    GroupNodePtr_t obstacle = GroupNode::create(std::string("obstacle"));

    /*world->addChild(robot);
    world->addChild(obstacle);

    DefVector3 position1(2.,0.,0.);
    DefQuat attitude1(1.,0.,0.,0.);
    Tools::ConfigurationPtr_t config1 = Tools::Configuration::create(position1, attitude1);
    DefVector3 position2(0.,2.,0.);
    DefQuat attitude2(1.,0.,0.,0.);
    Tools::ConfigurationPtr_t config2 = Tools::Configuration::create(position2, attitude2);

    robot->addChild(box);
    robot->applyConfiguration(config1);
    box->applyConfiguration(config1);
    robot->addChild(capsule);
    capsule->setVisibilityMode(VISIBILITY_OFF);
    robot->addChild(cone);
    cone->applyConfiguration(config2);
    cone->setColor(osgVector4(1.,1.,0.5,1.));
    cone->setVisibilityMode(VISIBILITY_ON);
    DefVector3 position3(0.,0.,8.);
    DefQuat attitude3(1.,0.,0.,0.);
    Tools::ConfigurationPtr_t config3 = Tools::Configuration::create(position3, attitude3);
    obstacle->applyConfiguration(config3);
    obstacle->addChild(cylindre);
    sphere->applyConfiguration(config2);
    obstacle->addChild(sphere);
    sphere->setAlpha(0.1f);
    world->addChild(ground);
    world->addChild(collada);
    collada->applyConfiguration(config2);
    std::string name("world/robot/genou");
    std::cout << (parseName(name)) << std::endl;*/

    world->addChild(ground);
    world->addChild(robot);
    WindowManagerPtr_t gm = WindowManager::create();
    gm->addNode(world);
    //osgViewer::Viewer viewer;
    //viewer.setSceneData( world->asGroup() );

    return gm->run();
  }