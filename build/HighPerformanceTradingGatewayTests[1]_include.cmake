if(EXISTS "/root/cpp-workspace/HighPerformanceTradingGateway/build/HighPerformanceTradingGatewayTests[1]_tests.cmake")
  include("/root/cpp-workspace/HighPerformanceTradingGateway/build/HighPerformanceTradingGatewayTests[1]_tests.cmake")
else()
  add_test(HighPerformanceTradingGatewayTests_NOT_BUILT HighPerformanceTradingGatewayTests_NOT_BUILT)
endif()