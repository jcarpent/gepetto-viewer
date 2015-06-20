//
//  leaf-node-ground.cpp
//  gepetto-viewer
//
//  Created by Justin Carpentier, Mathieu Geisert in November 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/leaf-node-ground.h>
#include <osg/Material>

namespace graphics {
    
    /* Declaration of private function members */
    
    void LeafNodeGround::init()
    {
      float x_dim = x_cell_dim_ * (float)nx_;
      float y_dim = y_cell_dim_ * (float)ny_;
      //node_osg_ptr_ = ::graphics::NodeOSG::create(getCenter(), getRotation(), getConfiguration());
      osgVector3 offset = osgVector3( x_dim / 2.f , y_dim / 2.f , 0.f );
      
      /* According to floor operation, we adapt sizes */
      //setSquareLength(2. * length_ / nX);
      //setSquareWidth(2. * width_ / nY);
      
      /* Setting base vectors */
      osgVector3 x_cell_vec = osgVector3( x_cell_dim_ , 0.0f , 0.0f );
      osgVector3 y_cell_vec = osgVector3( 0.0f , y_cell_dim_ , 0.0f );
      
      /* Allocation of vertices */
      ::osg::Vec3ArrayRefPtr vertices_array_ptr = new ::osg::Vec3Array;
      colors_array_ptr_ = new ::osg::Vec4Array;
      
      for ( std::size_t j(0) ; j < ny_ ; j++ )
      {
        for ( std::size_t i(0) ; i < nx_ ; i++ )
        {
          vertices_array_ptr->push_back( - offset + x_cell_vec * ((float) i) + y_cell_vec * ((float) (j+1)));
          vertices_array_ptr->push_back( - offset + x_cell_vec * ((float) i) + y_cell_vec * ((float) j));
          vertices_array_ptr->push_back( - offset + x_cell_vec * ((float) (i+1)) + y_cell_vec * ((float) j));
          vertices_array_ptr->push_back( - offset + x_cell_vec * ((float) (i+1)) + y_cell_vec * ((float) (j+1)));
          
          if ((i+j)%2) {
            colors_array_ptr_->push_back(color1_);
            colors_array_ptr_->push_back(color1_);
            colors_array_ptr_->push_back(color1_);
            colors_array_ptr_->push_back(color1_);
          }
          else {
            colors_array_ptr_->push_back(color2_);
            colors_array_ptr_->push_back(color2_);
            colors_array_ptr_->push_back(color2_);
            colors_array_ptr_->push_back(color2_);
          }
        }
      }
      
      /* Allocating grid_geometry_ptr_ */
      if (~grid_geometry_ptr_.valid())
        grid_geometry_ptr_ = new ::osg::Geometry;
      
      grid_geometry_ptr_->setVertexArray(vertices_array_ptr);
      grid_geometry_ptr_->setColorArray(colors_array_ptr_);
      
      grid_geometry_ptr_->setColorBinding(::osg::Geometry::BIND_PER_VERTEX);
      
      /* Define the normal to all quads */
      ::osg::Vec3ArrayRefPtr normals_array_ptr = new ::osg::Vec3Array;
      normals_array_ptr->push_back( osgVector3(0.0f,0.0f, 1.0f) );
      grid_geometry_ptr_->setNormalArray( normals_array_ptr );
      grid_geometry_ptr_->setNormalBinding( ::osg::Geometry::BIND_OVERALL );
      
      /* Defining type of geometries */
      grid_geometry_ptr_->addPrimitiveSet( new ::osg::DrawArrays( ::osg::PrimitiveSet::QUADS , 0 , vertices_array_ptr->size() ) );
      
      
      /* Allocating grid_geode_ptr_ */
      if (~grid_geode_ptr_.valid())
          grid_geode_ptr_ = new ::osg::Geode;
        
      grid_geode_ptr_->addDrawable(grid_geometry_ptr_);
      
      //node_osg_ptr_->asGroup()->addChild(grid_geode_ptr_);
      asQueue()->addChild(grid_geode_ptr_);
        
      /* Apply colors */
      setColors(color1_,color2_);

      // Set the diffusing mode of the ground
      osg::ref_ptr<osg::Material> material = new osg::Material;
      material->setAmbient(osg::Material::FRONT, osg::Vec4(0.f, 0.f, 0.f, 0.f));
      material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.3f, 0.3f, 0.3f, 1.f));
      material->setDiffuse(osg::Material::BACK, osg::Vec4(0.3f, 0.3f, 0.3f, 1.f));
      material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.f, 0.f, 0.f, 1.f));
      material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0.f, 0.0f, 0.f, 1.f));
      //material->setShininess(osg::Material::FRONT_AND_BACK, 0);
      material->setColorMode(osg::Material::DIFFUSE);
        
      grid_geode_ptr_->getOrCreateStateSet()->setAttributeAndModes(material.get (), ::osg::StateAttribute::ON | ::osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE);

#ifdef DEBUG
      std::cout << getID() << " created" << std::endl;
#endif
    }
    
    void LeafNodeGround::initWeakPtr( const LeafNodeGroundWeakPtr &other_weak_ptr )
    {
        weak_ptr_ = other_weak_ptr;
    }
    
    /* End of declaration of private function members */
    
    /* Declaration of protected function members */
    
    LeafNodeGround::LeafNodeGround(const std::string & name, std::size_t nx, std::size_t ny,
                                   float x_cell_dim, float y_cell_dim, 
                                   const osgVector4 & color1, const osgVector4 & color2):
                                   Node(name), nx_(nx), ny_(ny), 
                                   x_cell_dim_(x_cell_dim), y_cell_dim_(y_cell_dim),
                                   color1_(color1), color2_(color2)
    {
      init();
    }

    LeafNodeGround::LeafNodeGround(const LeafNodeGround &other):
                                   Node(other.getID()), nx_(other.nx_), ny_(other.ny_), 
                                   x_cell_dim_(other.x_cell_dim_), y_cell_dim_(other.y_cell_dim_),
                                   color1_(other.color1_), color2_(other.color2_)
    {
      init();
    }
    /* End of declaration of protected function members */
  
    /* Declaration of public function members */
  
    LeafNodeGroundPtr_t LeafNodeGround::create(const std::string & name, std::size_t nx, std::size_t ny,
                                               float x_cell_dim, float y_cell_dim, 
                                               const osgVector4 & color1, const osgVector4 & color2)
    {

        LeafNodeGroundPtr_t shared_ptr(new LeafNodeGround(name, nx, ny, x_cell_dim, y_cell_dim, color1, color2));
        
        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);
        
        return shared_ptr;
    }

    LeafNodeGroundPtr_t LeafNodeGround::create(const std::string & name)
    {
      LeafNodeGroundPtr_t shared_ptr( new LeafNodeGround(name, 10, 10, 1.f, 1.f, osgVector4(0.,0.,0.,1.), osgVector4(1.,1.,1.,1.)) );

        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);

        return shared_ptr;
    }
    
    LeafNodeGroundPtr_t LeafNodeGround::createCopy(const LeafNodeGroundPtr_t &other)
    {
      LeafNodeGroundPtr_t shared_ptr( new LeafNodeGround( *other ) );
        
        // Add reference to itself
        shared_ptr->initWeakPtr(shared_ptr);
        
        return shared_ptr;
    }
    
    LeafNodeGroundPtr_t LeafNodeGround::clone(void) const
    {
        return LeafNodeGround::createCopy( weak_ptr_.lock() );
    }
    
    LeafNodeGroundPtr_t LeafNodeGround::self(void) const
    {
        return weak_ptr_.lock();
    }
  
    void LeafNodeGround::setColor1(const osgVector4 &color1)
    {
        LeafNodeGround::setColors(color1, color2_);
    }
    
    void LeafNodeGround::setColor2(const osgVector4 &color2)
    {
        LeafNodeGround::setColors(color1_, color2);
    }
    
    void LeafNodeGround::setColors(const osgVector4 &color1 , const osgVector4 &color2)
    {
        color1_ = color1;
        color2_ = color2;

        /* Reset colors array */
        colors_array_ptr_.release();
        colors_array_ptr_ = new ::osg::Vec4Array;
        
        
        /* Set colors */
        for ( std::size_t j(0) ; j < ny_ ; j++ )
        {
          for ( std::size_t i(0) ; i < nx_ ; i++ )
          {
            if ((i+j)%2) {
              colors_array_ptr_->push_back(color1_);
              colors_array_ptr_->push_back(color1_);
              colors_array_ptr_->push_back(color1_);
              colors_array_ptr_->push_back(color1_);
            }
            else {
              colors_array_ptr_->push_back(color2_);
              colors_array_ptr_->push_back(color2_);
              colors_array_ptr_->push_back(color2_);
              colors_array_ptr_->push_back(color2_);
            }
          }
        }
      
        /* Apply colors */
        grid_geometry_ptr_->setColorArray(colors_array_ptr_);
        grid_geometry_ptr_->setColorBinding(::osg::Geometry::BIND_PER_VERTEX);
    }
  
    LeafNodeGround::~LeafNodeGround()
    {
      
      /* Proper deletion of all tree scene */
      grid_geode_ptr_->removeDrawable(grid_geometry_ptr_);
      grid_geometry_ptr_ = NULL;
        
      this->asQueue()->removeChild(grid_geode_ptr_);
      grid_geode_ptr_ = NULL;

      colors_array_ptr_.release();

      weak_ptr_.reset();
      
    }
  
    /* End of declaration of public function members */
}
