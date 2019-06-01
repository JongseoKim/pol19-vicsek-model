std::vector<std::vector<float> > positions_init(int agent_number, 
    float box_size, int dim, std::string init_mode = "random_uniform")
{
    std::vector<std::vector<float> > positions(agent_number, 
        std::vector<float>(dim));
    // saves current positions for each agent
    
    if (init_mode == "random_uniform")
    {
        // initialize random number generator, draw from [0;box_size)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, box_size);
        
        // draw for each agent, for each dimension one random number
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
        {
            for (int dim_ind = 0; dim_ind < dim; dim++)
            {
                positions[agent_ind][dim_ind] = dis(gen);
            }
        }
    }
    return positions;
}

std::vector<std::vector<float> > angles_init(int agent_number, 
    float box_size, int dim, std::string init_mode = "random_uniform")
{
    std::vector<std::vector<float> >  angles(agent_number); 
    // saves current angles
 
    if (init_mode == "random_uniform")
    {
        // initialize random number generator, draw from [0;box_size)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, M_PI);
        // CAUTION: for generalization to 3D adapt random interval
        
        // draw for each agent, for each dimension one random number
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
        {
            for (int dim_ind = 0; dim_ind < dim-1; dim++)
            {
                angles[agent_ind][dim_ind] = dis(gen);
            }
        }
    }
    return angles;
}