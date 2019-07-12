// defines the signum function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


// updates the positions of all agents from time t to t+1
void update_positions(int agent_number, int dim, std::vector<std::vector<float> > &positions,
	std::vector<std::vector<float> > angles, float velocity, float time_step, float box_size)
{
	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
		for (int dim_ind = 0; dim_ind < dim; dim_ind++)
		{
			if (dim_ind == 0)
			{
				//differential equation
				positions[agent_ind][dim_ind] += velocity * cos(angles[agent_ind][dim_ind]) * time_step;

				//periodic boundary condition
				if (positions[agent_ind][dim_ind] >= box_size)
				{
					positions[agent_ind][dim_ind] -= box_size;
				}
				if (positions[agent_ind][dim_ind] < 0)
				{
					positions[agent_ind][dim_ind] += box_size;
				}
			}
			if (dim_ind == 1)
			{
				//differential equation
				positions[agent_ind][dim_ind] += velocity * sin(angles[agent_ind][dim_ind]) * time_step;
				
				//periodic boundary condition
				if (positions[agent_ind][dim_ind] >= box_size)
				{
					positions[agent_ind][dim_ind] -= box_size;
				}
				if (positions[agent_ind][dim_ind] < 0)
				{
					positions[agent_ind][dim_ind] += box_size;
				}

			}
		}
	}
}


// updates the angles of all agents from time t to t+1
std::vector<std::vector<float> > update_angles(int agent_number, int dim, std::vector<std::vector<float> > angles,
	float noise_strength, std::vector<std::vector<int> > interacting_neighbors, float angle_interval_low,
	float angle_interval_high, std::mt19937& gen, float polar_interact_prob)
{

	// RNG: draw from [-pi;pi) (angular noise) and [0,1) (polar or nematic interactions)
    std::uniform_real_distribution<> dis(angle_interval_low, angle_interval_high);
    std::uniform_real_distribution<> dis2(0, 1);


	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
	    // decide on type of interaction
	    bool polar_flag = false;
	    if (dis2(gen) < polar_interact_prob)
        {
            polar_flag = true;
        }

	    // treat all dimensions
		for (int dim_ind = 0; dim_ind < dim - 1; dim_ind++)
		{
			if (dim_ind == 0)
			{
			    // summations of sin and cosine components of angles
				float sin_sum = 0;
				float cos_sum = 0;

				// integration of the stochastic differential equations
				// angular update: following the original Vicsek paper
				for (int interact_neighbor_ind = 0; interact_neighbor_ind < interacting_neighbors[agent_ind].size();
				    interact_neighbor_ind++)
				{
					int selected_agent_ind = interacting_neighbors[agent_ind][interact_neighbor_ind];

					if (polar_flag) // polar interaction
					{
					    sin_sum += sin(angles[selected_agent_ind][dim_ind]);
                        cos_sum += cos(angles[selected_agent_ind][dim_ind]);
                    }
					else // nematic interaction according to arXiv:1206.3811 eq. 1
                    {
                       // angle_sum += sgn( cos (angles[agent_ind][dim_ind] - angles[selected_agent_ind][dim_ind]) )
                         //       * angles[selected_agent_ind][dim_ind];
                    }

					//std::cout << "added angle:\t"<< angles[selected_agent_ind][dim_ind]<<
					//"\nangle_sum:\t" << angle_sum << std::endl;
				}

				// treat periodicity of arctan
				// angle within -pi/2,pi/2
				if (cos_sum >= 0)
				{
                    angles[agent_ind][dim_ind] = atan2(sin_sum,cos_sum) + noise_strength * dis(gen);
                }
				else // angle within pi/2, 3pi/2 => add pi
                {
                    angles[agent_ind][dim_ind] = atan2(sin_sum,cos_sum) + atan(1) * 4 + noise_strength * dis(gen);
                }


				// pbc, put angles into [-pi,pi) by subtracting or adding 2pi
				if (angles[agent_ind][dim_ind] > angle_interval_high)
				{ 
					angles[agent_ind][dim_ind] -= 2 * atan(1) * 4;
				}
				else if (angles[agent_ind][dim_ind] <= angle_interval_low)
				{
					angles[agent_ind][dim_ind] += 2 * atan(1) * 4;
				}

			}
		}
	}
	return angles;
}