{
  "targets": [
    {
      "target_name": "asioChannelName",
      "sources": [
        "src/asioChannelName.cc",
        "src/asio/common/asio.cpp",
        "src/asio/host/asiodrivers.cpp",
        "src/asio/host/pc/asiolist.cpp",
      ],
      "include_dirs": [
        "src/asio",
        "src/asio/common",
        "src/asio/host",
        "src/asio/host/pc",
      ]
    }
  ]
}
