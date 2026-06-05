

bool ptb::sequencer::load_track_file( const std::string& file_name )
{
  std::stringstream f;
  bear::engine::resource_pool::get_instance().get_file(file_name, f);
  std::string line;
  track t;
  bool result(false);
  bear::universe::time_type prev_date(0);
  std::size_t n(0);

  while ( !result && std::getline(f, line) )
    {
      claw::text::trim(line);

      if ( !line.empty() )
        if ( line[0] != '#' )
          {
            std::istringstream iss(line);
            result = ( iss >> t.duration >> t.width );
          }

      ++n;
    }
