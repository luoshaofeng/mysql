file(REMOVE_RECURSE
  "../../../archive_output_directory/libzlib.a"
  "../../../archive_output_directory/libzlib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/zlib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
