file(REMOVE_RECURSE
  "../../../../../archive_output_directory/libndbgeneral.a"
  "../../../../../archive_output_directory/libndbgeneral.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ndbgeneral.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
