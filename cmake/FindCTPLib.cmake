list(APPEND CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/deps/ctp/include")
list(APPEND CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/deps/ctp/lib")
find_path(CTPLib_INCLUDE_DIR ThostFtdcTraderApi.h)
find_library(CTP_TRADER_LIB thosttraderapi_se)
find_library(CTP_MD_LIB thostmduserapi_se)
set(CTPLib_LIBRARIES ${CTP_TRADER_LIB} ${CTP_MD_LIB})
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CTPLib DEFAULT_MSG CTP_TRADER_LIB CTP_MD_LIB CTPLib_INCLUDE_DIR)