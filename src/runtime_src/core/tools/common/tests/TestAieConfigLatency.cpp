// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

// ------ I N C L U D E   F I L E S -------------------------------------------
// Local - Include Files
#include "TestAieConfigLatency.h"
#include "tools/common/XBUtilities.h"
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_hw_context.h"
#include "xrt/xrt_kernel.h"
namespace XBU = XBUtilities;

#include <filesystem>

static constexpr int itr_count = 100000;

// ----- C L A S S   M E T H O D S -------------------------------------------
TestAieConfigLatency::TestAieConfigLatency()
  : TestRunner("aie-config-latency", "Measure xclbin download latency")
{}

boost::property_tree::ptree
TestAieConfigLatency::run(std::shared_ptr<xrt_core::device> dev)
{
  boost::property_tree::ptree ptree = get_test_header();

  //Find first xclbin/ validate doesn't have PDI right?
  const auto xclbin_name = xrt_core::device_query<xrt_core::query::xclbin_name>(dev, xrt_core::query::xclbin_name::type::validate);
  auto xclbin_path = findPlatformFile(xclbin_name, ptree);
  if (!std::filesystem::exists(xclbin_path))
    return ptree;

  logger(ptree, "Xclbin", xclbin_path);

  xrt::xclbin xclbin;
  try {
    xclbin = xrt::xclbin(xclbin_path);
  }
  catch (const std::runtime_error& ex) {
    logger(ptree, "Error", ex.what());
    ptree.put("status", test_token_failed);
    return ptree;
  }

  // Find second xclbin 
  const auto xclbin2_name = xrt_core::device_query<xrt_core::query::xclbin_name>(dev, xrt_core::query::xclbin_name::type::gemm);
  auto xclbin2_path = findPlatformFile(xclbin2_name, ptree);
  if (!std::filesystem::exists(xclbin2_path))
    return ptree;

  logger(ptree, "Xclbin", xclbin2_path);

  xrt::xclbin xclbin2;
  try {
    xclbin2 = xrt::xclbin(xclbin2_path);
  }
  catch (const std::runtime_error& ex) {
    logger(ptree, "Error", ex.what());
    ptree.put("status", test_token_failed);
    return ptree;
  }

  logger(ptree, "Details", boost::str(boost::format("No. of iterations: '%f'") % itr_count));

  auto working_dev = xrt::device(dev);
  float elapsedSecs = 0.0;

  try {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < itr_count; i++) {
      working_dev.register_xclbin(xclbin); //sync call?
      working_dev.register_xclbin(xclbin2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    elapsedSecs = std::chrono::duration_cast<std::chrono::duration<float>>(end-start).count();
  }
  catch (const std::exception& ex) {
    logger(ptree, "Error", ex.what());
    ptree.put("status", test_token_failed);
  }

  const float latency = (elapsedSecs / itr_count) * 1000000; //convert s to us

  logger(ptree, "Details", boost::str(boost::format("Average latency: '%.1f' us") % latency));
  ptree.put("status", test_token_passed);
  return ptree;
}
