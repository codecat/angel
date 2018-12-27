# Sets the following variables:
#
#  GLM_FOUND
#  GLM_INCLUDE_DIR
#  GLM_LIBRARY

set(GLM_SEARCH_PATHS
	/usr/local/include
	/usr/include
	)

find_path(GLM_INCLUDE_DIR
	NAMES glm.hpp
	PATH_SUFFIXES glm
	PATHS ${GLM_SEARCH_PATHS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
