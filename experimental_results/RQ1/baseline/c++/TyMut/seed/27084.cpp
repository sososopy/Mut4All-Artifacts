

    template<typename PlotFunc>
    void DrawView(PlotFunc GetPlotFunc)
    {
        GetPlotFunc(1)(2);
    }
    
    void CalculateLightmap()
    {
        auto LightmapRenderer = [](unsigned round)
        {
            return [round](const auto& view)
            {
                return [=](auto prop)
                {
                    round + 0;
                };
            };
        };
    
        DrawView(LightmapRenderer(0));
    }
