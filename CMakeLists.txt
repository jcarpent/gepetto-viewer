#
# Copyright (c) 2014 LAAS-CNRS
# Authors: Mathieu Geisert
#
#
# This file is part of Gepetto Viewer.
# SceneViewer is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-core is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-core  If not, see
# <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CXX_DISABLE_WERROR TRUE)
INCLUDE(cmake/base.cmake)
INCLUDE(cmake/eigen.cmake)
INCLUDE(cmake/boost.cmake)

SET(PROJECT_NAME gepetto-viewer)
SET(PROJECT_DESCRIPTION "An user-friendly Graphical Interface")
SET(PROJECT_URL "https://github.com/humanoid-path-planner/gepetto-viewer")

# Where to compile shared objects
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SETUP_PROJECT()

# Declare Headers
SET(${PROJECT_NAME}_HEADERS
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/config-osg.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/group-node.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-box.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-capsule.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-collada.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-cone.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-cylinder.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-face.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-ground.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-line.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-sphere.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/macros.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/node.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/urdf-parser.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/window-manager.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/leaf-node-xyzaxis.h
  ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/roadmap-viewer.h
)

ADD_REQUIRED_DEPENDENCY("openscenegraph >= 3.2")
ADD_REQUIRED_DEPENDENCY("urdfdom")
SEARCH_FOR_BOOST()

ADD_SUBDIRECTORY(src)
PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

SETUP_PROJECT_FINALIZE()
