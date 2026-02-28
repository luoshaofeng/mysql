file(REMOVE_RECURSE
  "../archive_output_directory/libclientlib.a"
  "../archive_output_directory/libclientlib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/clientlib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
