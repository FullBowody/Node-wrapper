{
  "targets": [
    {
      "target_name": "fullbowody",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "<!@(node -p \"require('fs').readdirSync('./src').map(f=>'src/'+f).join(' ')\")"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<(module_root_dir)/include/public",
        "<(module_root_dir)/include/private",
        "<(module_root_dir)/dependencies/Engine/include/public",
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "conditions": [
        ['OS == "win"', {
          "msvs_settings": {
            'VCCLCompilerTool': {
              'AdditionalOptions': [ '-std:c++17', ],
            },
          }
        }]
      ]
    }
  ]
}