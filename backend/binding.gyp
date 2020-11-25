{
  "targets": [
    {
      "target_name": "caffparser",
      "sources": [
        "../CaffParser/utils.cpp",
        "../CaffParser/ciff.cpp",
        "../CaffParser/caff.cpp",
        "./node_binding.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    }
  ]
}