file(REMOVE_RECURSE
  "../archive_output_directory/libvio.a"
  "../archive_output_directory/libvio.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/vio.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
