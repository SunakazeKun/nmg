def __main():
    from makesub.static_files import copy_static_files_to_game
    from makesub.use_resource import pack_use_resource_archives
    from makesub.other_archives import pack_other_archives
    from makesub.message_data import build_message_data
    from makesub.pack_distzip import pack_zip_archive
    from syati_module import build_syati_release_modules

    copy_static_files_to_game()
    pack_use_resource_archives()
    pack_other_archives()
    build_message_data()

    build_syati_release_modules()

    pack_zip_archive()


if __name__ == '__main__':
    __main()
