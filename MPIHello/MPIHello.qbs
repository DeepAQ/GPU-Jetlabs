import qbs

CppApplication {
    consoleApplication: true
    files: "main.c"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.includePaths: ['C:\\Program Files\\MPICH2\\include']
    cpp.libraryPaths: ['C:\\Program Files\\MPICH2\\lib']
    cpp.staticLibraries: "mpi"
}
