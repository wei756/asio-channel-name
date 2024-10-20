#include <node_api.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "asio.h"
#include "asiodrivers.h"

extern AsioDrivers *asioDrivers;

int loadDriver()
{
  asioDrivers = new AsioDrivers();
  if (!asioDrivers)
  {
    std::cerr << "Failed to initialize ASIO driver manager." << std::endl;
    return -1;
  }
  std::cout << "ASIO Driver loaded." << std::endl;
  return 0;
}

int clearDriver()
{
  if (asioDrivers)
  {
    std::cout << "ASIO Driver unloaded." << std::endl;
    delete asioDrivers;
    asioDrivers = nullptr;
    return 0;
  }
  return -1;
}

std::vector<std::string> getASIODrivers()
{
  long deviceCount = asioDrivers->asioGetNumDev();
  std::vector<std::string> devicesNames(deviceCount, std::string(32, '\0'));

  for (int i = 0; i < deviceCount; i++)
  {
    asioDrivers->asioGetDriverName(i, devicesNames[i].data(), 32);
    std::cout << "ASIO Driver " << i << ": " << devicesNames[i].data() << std::endl;
  }
  return devicesNames;
}

int loadASIODevice(std::string deviceName)
{
  if (!asioDrivers->loadDriver(deviceName.data()))
  {
    std::cerr << "Failed to load ASIO driver: " << deviceName << std::endl;
    return -1;
  }
  std::cout << "ASIO Driver loaded: " << deviceName << std::endl;

  ASIODriverInfo driverInfo;
  ASIOError result;
  result = ASIOInit(&driverInfo);
  if (result != ASE_OK)
  {
    std::cout << result << std::endl;
    std::cerr << "Failed to initialize ASIO driver." << std::endl;

    return result;
  }
  return 0;
}

int clearASIODevice()
{
  ASIOError result;
  result = ASIOExit();
  if (result != ASE_OK)
  {
    std::cerr << "Failed to clear ASIO driver." << std::endl;
    return result;
  }
  std::cout << "ASIO Driver cleared." << std::endl;
  return 0;
}

std::array<std::vector<std::string>, 2> getASIOChannelNames()
{
  long numInputChannels, numOutputChannels;

  if (ASIOGetChannels(&numInputChannels, &numOutputChannels) != ASE_OK)
  {
    std::cerr << "Error getting channel count." << std::endl;

    std::array<std::vector<std::string>, 2> channelNames{
        std::vector<std::string>(0, std::string(32, '\0')),
        std::vector<std::string>(0, std::string(32, '\0'))};

    return channelNames;
  }
  std::array<std::vector<std::string>, 2> channelNames{
      std::vector<std::string>(numInputChannels, std::string(32, '\0')),
      std::vector<std::string>(numOutputChannels, std::string(32, '\0'))};

  std::cout << "Input Channels:" << std::endl;
  for (long i = 0; i < numInputChannels; ++i)
  {
    ASIOChannelInfo channelInfo = {0};
    channelInfo.channel = i;
    channelInfo.isInput = ASIOTrue;

    if (ASIOGetChannelInfo(&channelInfo) == ASE_OK)
    {
      channelNames[0][i] = channelInfo.name;
      std::cout << "  " << i << ": " << channelInfo.name << std::endl;
    }
    else
    {
      std::cerr << "Error getting input channel info." << std::endl;
    }
  }

  std::cout << "Output Channels:" << std::endl;
  for (long i = 0; i < numOutputChannels; ++i)
  {
    ASIOChannelInfo channelInfo = {0};
    channelInfo.channel = i;
    channelInfo.isInput = ASIOFalse;

    if (ASIOGetChannelInfo(&channelInfo) == ASE_OK)
    {
      channelNames[1][i] = channelInfo.name;
      std::cout << "  " << i << ": " << channelInfo.name << std::endl;
    }
    else
    {
      std::cerr << "Error getting output channel info." << std::endl;
    }
  }
  return channelNames;
}

napi_value LoadDriver(napi_env env, napi_callback_info info)
{
  napi_value result;
  napi_create_int32(env, loadDriver(), &result);

  return result;
}

napi_value ClearDriver(napi_env env, napi_callback_info info)
{
  napi_value result;
  napi_create_int32(env, clearDriver(), &result);

  return result;
}

napi_value GetASIODevices(napi_env env, napi_callback_info info)
{
  napi_value devices;
  std::vector<std::string> names;
  names = getASIODrivers();
  napi_create_array_with_length(env, names.size(), &devices);
  for (int i = 0; i < names.size(); i++)
  {
    napi_value name;
    napi_create_string_utf8(env, names[i].data(), names[i].size(), &name);
    napi_set_element(env, devices, i, name);
  }

  return devices;
}

napi_value LoadASIODevice(napi_env env, napi_callback_info info)
{
  size_t argc = 1;
  napi_value args[1];
  napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

  // parse the device name
  size_t strSize;
  napi_get_value_string_utf8(env, args[0], nullptr, 0, &strSize);

  std::string deviceName(strSize, '\0');
  napi_get_value_string_utf8(env, args[0], deviceName.data(), strSize + 1, nullptr);

  std::cout << "deviceName: " << strlen(deviceName.data()) << std::endl;
  // load the device
  napi_value result;
  napi_create_int32(env, loadASIODevice(deviceName), &result);

  return result;
}

napi_value ClearASIODevice(napi_env env, napi_callback_info info)
{
  napi_value result;
  napi_create_int32(env, clearASIODevice(), &result);

  return result;
}

napi_value GetASIOChannels(napi_env env, napi_callback_info info)
{
  // get channel names
  std::array<std::vector<std::string>, 2> channelNames = getASIOChannelNames();

  // create the result array
  napi_value result;
  napi_create_array_with_length(env, 2, &result);

  for (int i = 0; i < 2; i++)
  {
    napi_value channels;
    napi_create_array_with_length(env, channelNames[i].size(), &channels);
    for (int j = 0; j < channelNames[i].size(); j++)
    {
      napi_value name;
      napi_create_string_utf8(env, channelNames[i][j].data(), channelNames[i][j].size(), &name);
      napi_set_element(env, channels, j, name);
    }
    napi_set_element(env, result, i, channels);
  }

  return result;
}

napi_value Init(napi_env env, napi_value exports)
{
  napi_value loadDriver_fn,
      clearDriver_fn,
      getASIODevices_fn,
      loadASIODevice_fn,
      clearASIODevice_fn,
      getASIOChannels_fn;

  napi_create_function(env, nullptr, 0, LoadDriver, nullptr, &loadDriver_fn);
  napi_create_function(env, nullptr, 0, ClearDriver, nullptr, &clearDriver_fn);
  napi_create_function(env, nullptr, 0, GetASIODevices, nullptr, &getASIODevices_fn);
  napi_create_function(env, nullptr, 0, LoadASIODevice, nullptr, &loadASIODevice_fn);
  napi_create_function(env, nullptr, 0, ClearASIODevice, nullptr, &clearASIODevice_fn);
  napi_create_function(env, nullptr, 0, GetASIOChannels, nullptr, &getASIOChannels_fn);
  napi_set_named_property(env, exports, "loadDriver", loadDriver_fn);
  napi_set_named_property(env, exports, "clearDriver", clearDriver_fn);
  napi_set_named_property(env, exports, "getASIODevices", getASIODevices_fn);
  napi_set_named_property(env, exports, "loadASIODevice", loadASIODevice_fn);
  napi_set_named_property(env, exports, "clearASIODevice", clearASIODevice_fn);
  napi_set_named_property(env, exports, "getASIOChannels", getASIOChannels_fn);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)