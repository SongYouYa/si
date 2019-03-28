./b2 --toolset=gcc-4.8 stage --layout=tagged -q threading=multi variant=release link=static runtime-link=shared  --with-system --with-date_time --with-filesystem --with-program_options --with-regex --with-thread


./b2 --toolset=gcc stage --layout=tagged -q threading=multi variant=release link=shared runtime-link=shared  --with-system --with-date_time --with-filesystem --with-program_options --with-regex --with-thread --with-iostreams


./b2 --toolset=gcc stage --layout=tagged -q threading=multi variant=release link=static runtime-link=shared  --with-system --with-date_time --with-filesystem --with-program_options --with-regex --with-thread --with-iostreams