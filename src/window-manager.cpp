//
//  window-manager.cpp
//  gepetto-viewer
//
//  Created by Justin Carpentier, Mathieu Geisert in November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/window-manager.h>

#include <osg/Camera>
#include <osgGA/TrackballManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TerrainManipulator>
#include <osgDB/WriteFile>
#include <gepetto/viewer/leaf-node-sphere.h>

#include <boost/lexical_cast.hpp>
#include <sstream>

namespace graphics {

  std::size_t WindowManager::global_num_instances_ = 0;

    /* Declaration of private function members */
    void WindowManager::init(int x,
                             int y,
                             int width,
                             int height)
    {
      osg::TraitsRefPtr traits_ptr = new ::osg::GraphicsContext::Traits;
      osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

      traits_ptr->windowName = id_;
      traits_ptr->x = x;
      traits_ptr->y = y;
      traits_ptr->width = width;
      traits_ptr->height = height;
      traits_ptr->windowDecoration = true;
      traits_ptr->doubleBuffer = true;
      traits_ptr->useMultiThreadedOpenGLEngine = false;

      traits_ptr->alpha = ds->getMinimumNumAlphaBits();
      traits_ptr->stencil = ds->getMinimumNumStencilBits();
      traits_ptr->sampleBuffers = ds->getMultiSamples();
      traits_ptr->samples = 4;
      osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext( traits_ptr );

      init (gc);
    }

    void WindowManager::init(osg::GraphicsContextRefPtr gc)
    {
        std::string name = "root";
        scene_ptr_ = ::graphics::GroupNode::create(name);

        viewer_ptr_ = new ::osgViewer::Viewer();

        /* init main camera */
        main_camera_ = viewer_ptr_->getCamera ();

        const osg::GraphicsContext::Traits* traits_ptr = gc->getTraits ();
        gc_ = gc;
        main_camera_->setGraphicsContext(gc);
        main_camera_->setViewport(new osg::Viewport(0,0, traits_ptr->width, traits_ptr->height));
        main_camera_->setProjectionMatrixAsPerspective(
            30.0f, static_cast<double>(traits_ptr->width)/static_cast<double>(traits_ptr->height), 1.0f, 10000.0f );
        GLenum buffer = traits_ptr->doubleBuffer ? GL_BACK : GL_FRONT;
        main_camera_->setDrawBuffer(buffer);
        main_camera_->setReadBuffer(buffer);

        /* add camera to the viewer */
        viewer_ptr_->setSceneData ( scene_ptr_->asGroup() );
        viewer_ptr_->setKeyEventSetsDone (0);

        viewer_ptr_->setCameraManipulator( new ::osgGA::SphericalManipulator );

      viewer_ptr_->setThreadingModel(::osgViewer::ViewerBase::ThreadPerContext);

      // Create the window
      viewer_ptr_->realize ();

      // Set the frame rate and its display mode
      frame_rate_ = 0.;
      frame_rate_display_mode_ = RATE_IN_WINDOW_TITLE;

      // Init timer
      current_toc_ = ns (frame_timer_.elapsed ().wall);

      // Achieve one rendering
      viewer_ptr_->frame ();
    }

    WindowManager::WindowManager () : id_ ("Window " + ::boost::lexical_cast<std::string>(++global_num_instances_))
    {
        init (0, 0, DEF_WIDTH_WINDOW, DEF_HEIGHT_WINDOW);
    }

    WindowManager::WindowManager (osg::GraphicsContextRefPtr gc) : id_ ("Window " + ::boost::lexical_cast<std::string>(++global_num_instances_))
    {
        init (gc);
    }

    WindowManager::WindowManager (int x,
                                  int y,
                                  int width,
                                  int height) :
    id_ ("WindowManager" + ::boost::lexical_cast<std::string>(++global_num_instances_))
    {
        init (x, y, width, height);
    }

    WindowManager::WindowManager (const WindowManager& other) : id_ ("Window " + ::boost::lexical_cast<std::string>(++global_num_instances_))
    {
      init ((int) other.getWindowPosition().x(),
            (int) other.getWindowPosition().y(),
            (int) other.getWindowDimension().x(),
            (int) other.getWindowDimension().y());
    }

    void WindowManager::initWeakPtr (WindowManagerWeakPtr other_weak_ptr)
    {
        weak_ptr_ = other_weak_ptr;
    }

    /* End declaration of private function members */

    /* Declaration of protected function members */

    WindowManagerPtr_t WindowManager::create ()
    {
        WindowManagerPtr_t shared_ptr(new WindowManager());

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }

    WindowManagerPtr_t WindowManager::create (int x,
                                              int y,
                                              int width,
                                              int height)
    {
        WindowManagerPtr_t shared_ptr(new WindowManager(x, y, width, height));

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }

    WindowManagerPtr_t WindowManager::create(osg::GraphicsContext* gc)
    {
        WindowManagerPtr_t shared_ptr(new WindowManager(gc));

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }

    WindowManagerPtr_t WindowManager::createCopy (WindowManagerPtr_t other)
    {
        WindowManagerPtr_t shared_ptr(new WindowManager(*other));

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }

    /* End declaration of protected function members */

    /* Declaration of public function members */

    WindowManagerPtr_t WindowManager::clone (void) const
    {
        return WindowManager::createCopy(weak_ptr_.lock());
    }

    WindowManagerPtr_t WindowManager::self (void) const
    {
        return weak_ptr_.lock();
    }

    bool WindowManager::addNode(NodePtr_t graphical_object_ptr)
    {
        bool result = scene_ptr_->addChild (graphical_object_ptr);
        viewer_ptr_->home(); // Partie du code à revoir pour inclure juste un nouveau focus

        return result;
    }

  void WindowManager::computeFrameRate ()
  {
    typedef us::rep counter_t;

    us loop_duration = boost::chrono::duration_cast<us> (current_toc_ - previous_toc_);
    counter_t duration_counts = loop_duration.count ();

    frame_rate_ = 1000000. / (double) duration_counts;
  }

    bool WindowManager::done ()
    {
        return  viewer_ptr_->done();
    }

    bool WindowManager::frame ()
    {
//      viewer_ptr_->getCamera()->setViewMatrixAsLookAt(osg::Vec3(0,-10,0),
//                                                      osg::Vec3(0,0,0), osg::Vec3(0,0,1));
      // Compute frame rate
      previous_toc_ = current_toc_;
      current_toc_ = ns (frame_timer_.elapsed ().wall);
      computeFrameRate ();

      // Update window name if necessary
      applyWindowName (buildWindowName ());

      viewer_ptr_->frame ();
      return true;
    }

    bool WindowManager::run ()
    {
        return viewer_ptr_->run();
    }

    void WindowManager::setWindowDimension (int width,
                                            int height)
    {
      const osg::GraphicsContext::Traits* traits_ptr = viewer_ptr_->getCamera ()->getGraphicsContext ()->getTraits ();

      typedef osgViewer::Viewer::Windows Windows;
      Windows windows_list;

      viewer_ptr_->getWindows(windows_list);

      for (Windows::iterator it = windows_list.begin();
           it != windows_list.end(); ++it)
      {
        (*it)->setWindowRectangle(traits_ptr->x, traits_ptr->y, width, height);
      }
    }

    osgVector2 WindowManager::getWindowDimension() const
    {
        osgVector2 dimention;
        const osg::GraphicsContext::Traits* traits_ptr = viewer_ptr_->getCamera ()->getGraphicsContext ()->getTraits ();
        dimention.x() = (osg::Vec2f::value_type) traits_ptr->width;
        dimention.y() = (osg::Vec2f::value_type) traits_ptr->height;
        return dimention;
    }

    void WindowManager::setWindowPosition (int x_position,
                                           int y_position)
    {
      const osg::GraphicsContext::Traits* traits_ptr = viewer_ptr_->getCamera ()->getGraphicsContext ()->getTraits ();

      typedef osgViewer::Viewer::Windows Windows;
      Windows windows_list;

      viewer_ptr_->getWindows(windows_list);

      for (Windows::iterator it = windows_list.begin();
           it != windows_list.end(); ++it)
      {
        (*it)->setWindowRectangle(x_position, y_position, traits_ptr->width, traits_ptr->height);
      }
    }

    osgVector2 WindowManager::getWindowPosition() const
    {
        osgVector2 position;
        const osg::GraphicsContext::Traits* traits_ptr = viewer_ptr_->getCamera ()->getGraphicsContext ()->getTraits ();
        position.x() = (osg::Vec2f::value_type) traits_ptr->x;
        position.y() = (osg::Vec2f::value_type) traits_ptr->y;
        return position;
    }

    WindowManager::~WindowManager()
    {
      stopCapture ();
      scene_ptr_.reset();
    }
  
  osgViewer::ViewerRefPtr WindowManager::getViewerClone()
  {
//    return ::osgViewer::ViewerRefPtr(viewer_ptr_.get());
    return viewer_ptr_;
  }
  
  void WindowManager::startCapture (const std::string& filename,
      const std::string& extension)
  {
    if (screen_capture_ptr_.valid ()) {
      screen_capture_ptr_->startCapture ();
      return;
    }
    /* Create an handler to save video */
    write_to_file_ptr_ = new osgViewer::ScreenCaptureHandler::WriteToFile
      (filename, extension);
    screen_capture_ptr_ = new osgViewer::ScreenCaptureHandler
      (write_to_file_ptr_.get (), -1);
    /* Screen capture can be stopped with stopCapture */
    screen_capture_ptr_->setKeyEventTakeScreenShot (0);
    screen_capture_ptr_->setKeyEventToggleContinuousCapture (0);
    viewer_ptr_->addEventHandler (screen_capture_ptr_);
    screen_capture_ptr_->startCapture ();
  }

  void WindowManager::stopCapture ()
  {
    if (!screen_capture_ptr_) return;
    screen_capture_ptr_->stopCapture ();
    frame ();
  }

  bool WindowManager::writeNodeFile (const std::string& fn)
  {
    osgDB::Registry::instance ()->loadLibrary ("osgPlugins-3.3.4/osgdb_osgjs.so");
    return osgDB::writeNodeFile (*(viewer_ptr_->getSceneData()), fn);
  }

  void WindowManager::applyWindowName (const std::string & window_name)
  {
    typedef osgViewer::Viewer::Windows Windows;
    Windows windows_list;

    viewer_ptr_->getWindows (windows_list);

    for (Windows::iterator it = windows_list.begin ();
         it != windows_list.end(); ++it)
    {
      (*it)->setWindowName (window_name);
    }
  }

  std::string WindowManager::buildWindowName () const
  {
    std::string complete_window_name;
    complete_window_name.append (windowName ());

    if (frame_rate_display_mode_ == RATE_IN_WINDOW_TITLE || frame_rate_display_mode_ == RATE_IN_WINDOW_AND_TITLE)
    {
      complete_window_name.append (" - ");
      std::stringstream ss;
      ss << (unsigned int) frame_rate_;
      complete_window_name.append (ss.str ());
      complete_window_name.append (" fps");
    }

    return complete_window_name;
  }

    /* End declaration of public function members */

} /* namespace graphics */
