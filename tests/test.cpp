//
//  test.cpp
//  Test for SceneViwer
//
//  Created by Mathieu Geisert in November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <iostream>
#include <gepetto/viewer/window-manager.h>
#include <gepetto/viewer/node.h>
#include <gepetto/viewer/group-node.h>
#include <gepetto/viewer/leaf-node-box.h>
#include <gepetto/viewer/leaf-node-capsule.h>
#include <gepetto/viewer/leaf-node-cone.h>
#include <gepetto/viewer/leaf-node-cylinder.h>
#include <gepetto/viewer/leaf-node-line.h>
#include <gepetto/viewer/leaf-node-face.h>
#include <gepetto/viewer/leaf-node-sphere.h>
#include <gepetto/viewer/leaf-node-ground.h>
#include <gepetto/viewer/leaf-node-collada.h>
#include <gepetto/viewer/urdf-parser.h>

#include <boost/date_time.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include <boost/thread.hpp>

typedef boost::chrono::microseconds us;
typedef boost::chrono::nanoseconds ns;

unsigned int frame_rate = 20; // ms

int main(int, const char**)

{
  using namespace graphics;

  // Create a graphical window
  WindowManagerPtr_t gm = WindowManager::create();

  // Create the world node
  GroupNodePtr_t world = GroupNode::create(std::string("world"));

  // Attach the world node to the graphical window
  gm->addNode(world);

  // Create a box
  LeafNodeBoxPtr_t box = LeafNodeBox::create("box1", osgVector3(1.,1.,1.));

  // Add a landmark attached to the box
  box->addLandmark(1.);

  // Add the box to the world node
  world->addChild(box);

  // Delete landmark
  box->deleteLandmark();

  // Add landmark of dimension 2
  box->addLandmark(2.);

  // Move the box with a SE3 transformation
  box->applyConfiguration(osgVector3(2.,1.,0.), osgQuat(0.884,0.306,-0.177,0.306));

  // Add a landmark to the world
  world->addLandmark(1.);

  // Create a sphere and attach it to the world
  LeafNodeSpherePtr_t sphere = LeafNodeSphere::create("sphere1", 2.);
  world->addChild (sphere);

  // Transform the sphere into ellipsoid
  sphere->setScale (osgVector3(1., 2., 3.));

  // Translate the sphere
  sphere->applyConfiguration(osgVector3(3., 1., 2.), osgQuat());

  // Change the name of the current window
  gm->windowName ("Bla bla window");

  // Change the size of the current window
  gm->setWindowDimension (1024, 780);

  // Run the global process
  boost::timer::cpu_timer timer;
  us loop_duration_max (frame_rate * 1000LL);

  while (! gm->done ())
  {
    ns start (timer.elapsed ().wall); // tic
    // Render the scene
    gm->frame ();
    ns end (timer.elapsed ().wall); // toc
    us loop_duration = boost::chrono::duration_cast<us> (end - start);

    if (loop_duration < loop_duration_max)
    {
      typedef us::rep counter_t;
      counter_t duration_count = boost::chrono::duration_cast<us> (loop_duration_max - loop_duration).count ();
      boost::posix_time::microseconds sleep_duration (duration_count);

      // Sleep during sleep_duration in microsecond
      boost::this_thread::sleep (sleep_duration);
    }
  }
}
