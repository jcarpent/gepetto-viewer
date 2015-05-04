//
//  window-manager.h
//  ScneViewer
//
//  Created by Justin Carpentier, Mathieu Geisert in  November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_WINDOWMANAGER_HH
#define SCENEVIEWER_WINDOWMANAGER_HH

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <gepetto/viewer/group-node.h>
#include <gepetto/viewer/node.h>

#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

namespace graphics {

    DEF_CLASS_SMART_PTR(WindowManager)

    class WindowManager
    {
    public:
      enum FrameRateDisplayMode
      {
        NO_RATE = 0,
        RATE_IN_WINDOW_TITLE,
        RATE_IN_WINDOW,
        RATE_IN_WINDOW_AND_TITLE
      };

    private:
      typedef boost::chrono::nanoseconds ns;
      typedef boost::chrono::microseconds us;
      typedef boost::timer::cpu_timer timer;

      /** global Window Manager class elements counter */
      static std::size_t global_num_instances_;

      /** Unique identifier defined by the program */
      const std::string id_;

      /** Name of the window */
      std::string window_name_;

      /** Refreshing rate */
      double frame_rate_;
      ns current_toc_;
      ns previous_toc_;
      timer frame_timer_;

      /** Refreshing rate display mode */
      FrameRateDisplayMode frame_rate_display_mode_;

        /** Scene Graphical Group */
        GroupNodePtr_t scene_ptr_;

        /** OSG viewer */
        ::osgViewer::ViewerRefPtr viewer_ptr_;

        /** OSG cameras */
        ::osg::CameraRefPtr main_camera_;
        ::osg::GraphicsContextRefPtr gc_;

        ::osg::CameraRefPtr hud_camera_;

        /* OSG Screen capture handler */
        osg::ref_ptr < ::osgViewer::ScreenCaptureHandler> screen_capture_ptr_;
        osg::ref_ptr < ::osgViewer::ScreenCaptureHandler::WriteToFile> write_to_file_ptr_;

        /** Associated weak pointer */
        WindowManagerWeakPtr weak_ptr_;

        void init(osg::GraphicsContextRefPtr gc);

        void init(int x, int y, int width, int height);

        WindowManager();

        WindowManager (osg::GraphicsContextRefPtr gc);

        WindowManager(int x, int y, int width, int height);

        WindowManager(const WindowManager& other);

        /** Initialize weak_ptr */
        void initWeakPtr (WindowManagerWeakPtr other_weak_ptr);

      /** \brief Change effectively the name of the windows associated to the viewer */
      void applyWindowName (const std::string & window_name);

      /** \brief Build the window name according to the different set options */
      std::string buildWindowName () const;

      /** \brief Compute the instatenous frame rate, i.e. the frequency between two calls of frame method */
      void computeFrameRate ();

    public:

        std::string id () const { return id_; }

        /** Create and initialize a graphical engine of type OSG
         */
        static WindowManagerPtr_t create();

        /** Create and initialize a graphical engine with a GraphicsContext
         */
        static WindowManagerPtr_t create(osg::GraphicsContext* gc);

        /** Create and initialize a graphical engine of type OSG with some parameters : position + dimension
         */
        static WindowManagerPtr_t create(int x, int y, int width, int height);

        /** Static method for creating a clone of box other with the copy constructor
         */
        static WindowManagerPtr_t createCopy(WindowManagerPtr_t other);

        /** Proceed to a clonage of the current object defined by the copy constructor
         */
        virtual WindowManagerPtr_t clone(void) const;

        /** Return a shared pointer of the current object
         */
        WindowManagerPtr_t self(void) const;

        /** Add a graphical object to the scene
         */
        virtual bool addNode(NodePtr_t graphical_object_ptr);

        /** Return the scene group
         */
        virtual GroupNodePtr_t getScene() const { return scene_ptr_->self(); }

        /** Return the current doing of procedure
         */
        virtual bool done ();

        /** Generate a new frame rendering
         */
        virtual bool frame ();

        /** Run the scene process
         */
        virtual bool run ();

        /** Define dimension of the window */
        virtual void setWindowDimension (int width, int height);

        /** Define the window position */
        virtual void setWindowPosition (int x_position, int y_position);

        /** Return the window x and y position as a 2D vector */
        osgVector2 getWindowPosition () const;

        /** Return the window width and height as a 2D vector */
        osgVector2 getWindowDimension () const;

        /** Return a ref to the viewer */
        ::osgViewer::ViewerRefPtr getViewerClone();

        void startCapture (const std::string& filename,
            const std::string& extension);

        void stopCapture ();

        bool writeNodeFile (const std::string& filename);

      /** \brief Set the window name */
      void windowName (const std::string & window_name) { window_name_ = window_name; }

      /** \brief Get the window name */
      const std::string & windowName () const { return window_name_; }

      /** \brief Return the current frame rate */
      double frameRate () const { return frame_rate_; }

      /** \brief Set display mode of refreshing rate */
      void displayRefreshingRate (FrameRateDisplayMode mode) { frame_rate_display_mode_ = mode; }
      FrameRateDisplayMode displayRefreshingRate () const { return frame_rate_display_mode_; }

      /** \brief Destructor */
      virtual ~WindowManager();

    };
} /* namespace graphics */

#endif /* SCENEVIEWER_WINDOWMANAGER_HH */
