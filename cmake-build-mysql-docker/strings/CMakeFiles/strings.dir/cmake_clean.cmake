file(REMOVE_RECURSE
  "../archive_output_directory/libstrings.a"
  "../archive_output_directory/libstrings.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/strings.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
