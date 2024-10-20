const asioBindings = require("bindings")("asioChannelName.node");

function AsioChannelName() {
  let asioChannelName = {};

  asioChannelName.start = () => asioBindings.loadDriver();
  asioChannelName.stop = () => asioBindings.clearDriver();
  asioChannelName.getDevices = () =>
    asioBindings
      .getASIODevices()
      .map((device) => device.replaceAll("\x00", ""));
  asioChannelName.loadDevice = (name) => asioBindings.loadASIODevice(name);
  asioChannelName.clearDevice = () => asioBindings.clearASIODevice();
  asioChannelName.getChannelNames = () => asioBindings.getASIOChannels();

  return asioChannelName;
}
exports.AsioChannelName = AsioChannelName;
