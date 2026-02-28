file(REMOVE_RECURSE
  "../archive_output_directory/libmytime.a"
  "../archive_output_directory/libmytime.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/mytime.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
