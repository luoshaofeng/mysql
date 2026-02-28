file(REMOVE_RECURSE
  "archive_output_directory"
  "library_output_directory"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/unittest_all.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
