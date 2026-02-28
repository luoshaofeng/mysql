file(REMOVE_RECURSE
  "../../../../../archive_output_directory/libndblogger.a"
  "../../../../../archive_output_directory/libndblogger.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ndblogger.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
