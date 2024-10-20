# ASIO-Channel-Name

Display the input/output channel names from the ASIO device.

## Example

```typescript
const { AsioChannelName } = require('asio-channel-name');

const asio = AsioChannelName();

// Init asio driver
asio.start();

// Get ASIO device's list
const devices = asio.getDevices();
console.log("devices: ", devices);

// Load the device
asio.loadDevice(devices[0]);

// Get channel names
const channels = asio.getChannelNames();
console.log("channels", channels);

// Cleanup
asio.clearDevice();
asio.stop();
```

## Licence
This package is licensed under the MIT License.

The Steinberg ASIO SDK and licensing agreement can be found at:

- https://www.steinberg.net/developers/