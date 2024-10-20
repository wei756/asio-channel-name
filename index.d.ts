export enum AsioChannelNameError {
  ASE_OK = 0,
  UNKNOWN_ERROR = -1,
  ASE_SUCCESS = 0x3f4847a0,
  ASE_NotPresent = -1000,
  ASE_HWMalfunction = -999,
  ASE_InvalidParameter = -998,
  ASE_InvalidMode = -997,
  ASE_SPNotAdvancing = -996,
  ASE_NoClock = -995,
  ASE_NoMemory = -994,
}

export type AsioChannelNameBindings = {
  start: () => AsioChannelNameError;
  stop: () => AsioChannelNameError;
  getDevices: () => string[];
  loadDevice: (deviceName: string) => AsioChannelNameError;
  clearDevice: () => AsioChannelNameError;
  getChannelNames: () => [string[], string[]];
};

export function AsioChannelName(): AsioChannelNameBindings;
