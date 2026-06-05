

 QFileInfoList list, raw_list = dic_dir.entryInfoList();
 std::copy_if( raw_list.begin(),raw_list.end(), std::back_inserter(list), 
    []( QFileInfo const & inf ) -> bool
          { return inf.fileName().endsWith(".all.c"); }

