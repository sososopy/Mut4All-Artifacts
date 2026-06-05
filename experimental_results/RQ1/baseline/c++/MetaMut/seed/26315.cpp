
    bool rv;
    
    auto broken = [&] () 
    {
        try
        {
            
        }
        catch(...)
        {
            // Capturing a variable here produces an error related to exception handling
            rv=false;
        }
    };
