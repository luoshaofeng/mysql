file(REMOVE_RECURSE
  "../archive_output_directory/libjson_client_library.a"
  "../archive_output_directory/libjson_client_library.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/json_client_library.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
