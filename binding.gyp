{
  "targets": [
    {
      "target_name": "fullbowody",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "./src/main.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<(module_root_dir)/include",
        "<(module_root_dir)/dependencies/Engine/include/public",
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "conditions": [
        ['OS == "win"',
          {
            "link_settings": {
                "libraries": [
                    
                ],
                "library_dirs": [
                  "<(module_root_dir)/lib"
                ]
            },
            "copies": [
              {
                "destination": "<(module_root_dir)/build/Release",
                "files": [
                  "<(module_root_dir)/bin/*.dll"
                ]
              }
            ]
          }
        ]
      ]
    }
  ]
}