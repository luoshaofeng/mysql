file(REMOVE_RECURSE
  "../archive_output_directory/libmysys.a"
  "../archive_output_directory/libmysys.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/mysys.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
