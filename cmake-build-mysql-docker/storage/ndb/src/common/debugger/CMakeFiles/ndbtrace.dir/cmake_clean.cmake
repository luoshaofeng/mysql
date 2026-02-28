file(REMOVE_RECURSE
  "../../../../../archive_output_directory/libndbtrace.a"
  "../../../../../archive_output_directory/libndbtrace.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ndbtrace.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
