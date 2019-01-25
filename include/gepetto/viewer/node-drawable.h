//
//  node-drawable.h
//  gepetto-viewer
//
//  Created by Joseph Mirabel in 2018
//  Copyright (c) 2018 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_NODEDRAWABLE_HH
#define SCENEVIEWER_NODEDRAWABLE_HH

#include <gepetto/viewer/node.h>

namespace graphics {
    DEF_CLASS_SMART_PTR(NodeDrawable)
    
    /** Implementation of the drawable object in OSG render engine */
    class NodeDrawable : public Node
    {
    private:
        void init ();

    protected:
        ::osg::ShapeDrawableRefPtr shape_drawable_ptr_;

        /** Constructor */
        NodeDrawable (const std::string& name) : Node (name) { init (); }
        
        /** Copy constructor */
        NodeDrawable (const Node& other) : Node (other) { init (); }
        
    public:
        virtual void setAlpha (const float& alpha)
        {
          osgVector4 color = this->getColor();
          color[3] = alpha;
          this->setColor (color);
        }

	float getAlpha() const
        {
          return getColor () [3];
        }

        virtual void setColor(const osgVector4 &color);

        virtual osgVector4 getColor() const;

        virtual void setTexture(const std::string& image_path);

        /** Destructor */
        virtual ~NodeDrawable() {}
    };
} /* namespace graphics */

#endif /* SCENEVIEWER_NODEDRAWABLE_HH */