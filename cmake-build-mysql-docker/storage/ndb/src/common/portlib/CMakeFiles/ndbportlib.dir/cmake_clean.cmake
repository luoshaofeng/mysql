file(REMOVE_RECURSE
  "../../../../../archive_output_directory/libndbportlib.a"
  "../../../../../archive_output_directory/libndbportlib.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ndbportlib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
