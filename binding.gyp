{
    "targets": [{
        "target_name": "fuse_bindings",
        "sources": ["fuse-bindings.cc", "abstractions.cc"],
        "include_dirs": [
            "<!(node -e \"require('nan')\")"
        ],
        "conditions": [
            ['OS!="win"', {
                'variables':
                {
                    'fuse__include_dirs%': '<!(pkg-config fuse --cflags-only-I | sed s/-I//g)',
                    'fuse__library_dirs%': '',
                    'fuse__libraries%': '<!(pkg-config --libs-only-L --libs-only-l fuse)'
                },
                "include_dirs": [
                    "<@(fuse__include_dirs)"
                ],
                'library_dirs': [
                  '<@(fuse__library_dirs)',
                ],
                "link_settings": {
                    "libraries": [
                        "<@(fuse__libraries)"
                    ]
                }
            }],
            ['OS=="win"', {
                "variables": {
                    'winfsp__install_dir%': '<!(find_winfsp.cmd)'
                },
                "include_dirs": [
                    "<(winfsp__install_dir)\\inc\\fuse",
                    "$(INCLUDE)"
                ],
                "link_settings": {
                    "libraries": [
                        "<(winfsp__library)"
                    ]
                },
                "conditions": [
                    ['target_arch=="x64"', {
                        "variables": { 'winfsp__library%': '<(winfsp__install_dir)\\lib\\winfsp-x64' }
                    }, {
                        "variables": { 'winfsp__library%': '<(winfsp__install_dir)\\lib\\winfsp-x86' }
                    }]
                ]
            }]
        ],
        "configurations": {
            "Debug": {
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "RuntimeLibrary": 2
                    }
                }
            },
            "Release": {
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "RuntimeLibrary": 2
                    }
                }
            }
        }
    }]
}
