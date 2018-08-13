set(SRC_FILES
        main.c swkbd.c storage.c StorageAccess.c libraryAcessor.c libraryCreator.c)
add_executable(swkbd2 ${SRC_FILES})
target_link_libraries(swkbd2 -lnx)
add_nro_target(swkbd2 NO_NACP)