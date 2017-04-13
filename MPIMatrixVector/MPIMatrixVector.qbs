import qbs

CppApplication {
    consoleApplication: true
    files: "main.c"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.includePaths: ['C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Include']
    cpp.libraryPaths: ['C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x64']
    cpp.staticLibraries: "msmpi"
}
