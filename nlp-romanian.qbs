import qbs

CppApplication {
    consoleApplication: true
    cpp.debugInformation: true
    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: [
        "inc",
        "inc_dep",
    ]
    files: [
        "inc/nlp/romanian/base.hpp",
        "inc/nlp/romanian/phonetics.hpp",
        "main.cpp",
        "src/nlp/romanian/phonetics.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
