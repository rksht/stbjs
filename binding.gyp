{
    "targets": [
        {
            "target_name": "stbjs",
            "cflags": ["-fno-exceptions"],
            "cflags_cc": ["-std=c++17", "-fno-exceptions", "-fmax-errors=1"],
            "sources": ["cppsrc/addon.cpp"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "cppsrc/",
                "cppsrc/fmt/include",
                "/usr/include/c++/9.2.1/"
            ],
            "libraries": ["-lstdc++fs"],
            "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
        }
    ]
}