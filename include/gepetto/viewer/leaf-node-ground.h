//
//  leaf-node-ground.h
//  gepetto-viewer
//
//  Created by Justin Carpentier, Mathieu Geisert in November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#ifndef SCENEVIEWER_LEAFNODEGROUND_HH
#define SCENEVIEWER_LEAFNODEGROUND_HH

#include <gepetto/viewer/node.h>

namespace graphics
{
    DEF_CLASS_SMART_PTR(LeafNodeGround)
    
    class LeafNodeGround : public Node
    {
    private:
      
        std::size_t nx_;
        std::size_t ny_;
        float x_cell_dim_;
        float y_cell_dim_;

        osgVector4 color1_;
        osgVector4 color2_;

        /** Associated weak pointer */
        LeafNodeGroundWeakPtr weak_ptr_;
      
        /** Array of colors */
        ::osg::Vec4ArrayRefPtr colors_array_ptr_;

        /** Associated Geometry for LeafNodeGround */
        ::osg::GeometryRefPtr grid_geometry_ptr_;
        ::osg::GeodeRefPtr grid_geode_ptr_;
      
        /** Init method */
        void init();

        /** Constructor
         \brief Constructor with all useful parameters
         \param nx : number of cells along x axis
         \param ny : number of cells along y axis
         \param x_dim : dimension of a cell along x axis
         \param y_dim : dimension of a cell along y axis
         */
        LeafNodeGround(const std::string & name, std::size_t nx, std::size_t ny,
                       float x_cell_dim, float y_cell_dim, 
                       const osgVector4 & color1, const osgVector4 & color2);
        
        /** Copy constructor */
        LeafNodeGround(const LeafNodeGround& other);
      
        /** Initialize weak_ptr */
        void initWeakPtr( const LeafNodeGroundWeakPtr& other_weak_ptr );
        
    protected:
    public:
        
        /** Builder
         */
        static LeafNodeGroundPtr_t create(const std::string& name);
        
        /** Builder
         \brief Constructor with all useful parameters
         \param nx : number of cells along x axis
         \param ny : number of cells along y axis
         \param x_dim : dimension of a cell along x axis
         \param y_dim : dimension of a cell along y axis
         \param color1 : one of the two colors composing the ground
         \param color2 : one of the two colors composing the ground
         */
        static LeafNodeGroundPtr_t create(const std::string & name, std::size_t nx, std::size_t ny,
                                          float x_cell_dim = 1.f, float y_cell_dim = 1.f, 
                                          const osgVector4 & color1 = osgVector4(0., 0., 0., 1.), const osgVector4 & color2 = osgVector4(1., 1., 1., 1.));
        
        /** Create copy */
        static LeafNodeGroundPtr_t createCopy( const LeafNodeGroundPtr_t &other );
        
        /** Clone
         \brief Returns a shared pointer to a newly allocated copy of the object.
         */
        virtual LeafNodeGroundPtr_t clone(void) const;
      
      
        /** Copy
         \brief Proceed to a copy of the currend object as clone
         */
        virtual LeafNodeGroundPtr_t copy() const { return clone(); }
      
        /**
         \brief Returns a shared pointer of the object itself.
         */
        LeafNodeGroundPtr_t self(void) const;

        /** inline virtual void setColor1(const ColotSharedPtr &color1_ptr)
         \brief Allow to define color1
         \param color1_ptr : color to apply to first color
         */
        virtual void setColor1(const osgVector4 &color1);

        const osgVector4 & getColor1() const { return color1_; }
        
        /** inline virtual void setColor2(const ColotSharedPtr &color2_ptr)
         \brief Allow to define color2
         \param color2_ptr : color to apply to second color
         */
        virtual void setColor2(const osgVector4 & color2);
        
        const osgVector4 & getColor2() const { return color2_; }
        /** virtual void setColors(const ColorSharedPtr &color1_ptr , const ColorSharedPtr &color2_ptr);
         \brief Allow to define both colors
         \param color1_ptr : color to apply to first color
         \param color2_ptr : color to apply to second color
         */
        virtual void setColors(const osgVector4 & color1 , const osgVector4 & color2);
        
        /**
         \brief Virtual method for setting the shadow mode of the object : receive, cast, both or no shadow
         \param shadow_state state
         */
        //virtual void setShadowMode (const ShadowMode& shadow_state) {
        //    Node::setShadowMode ( shadow_state );
        //}
        
        /** Destructor */
        virtual ~LeafNodeGround();
    };
}

#endif /* SCENEVIEWER_LEAFNODEGROUND_HH */
