def __main():
    from copy_to_game import copy_static_files_to_game
    from font_archives import pack_font_archives
    from message_data import build_message_data
    from heap_memory import update_heap_memory_bcsv_files
    from pack_distzip import pack_zip_archive
    from syati_module import build_syati_release_modules
    from use_resource import repack_use_resource_archives

    copy_static_files_to_game()

    build_message_data()
    pack_font_archives()

    build_syati_release_modules()

    update_heap_memory_bcsv_files()
    repack_use_resource_archives()

    pack_zip_archive()


if __name__ == '__main__':
    __main()
