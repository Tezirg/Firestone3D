#include <list>
#include "getopt.h"
#include "demo.h"

int				main(int ac, char **av) 
{
	int32_t		flag = -1;
	option		opt[] = {
		{ "texture-mesh", 1, 0, 1 },
		{ "texture-material", 1, 0, 2 },
		{ "sequence-mesh", 1, 0, 3 },
		{ "sequence-frame", 1, 0, 4 },
		{ 0, 0, 0, 0 }
	};
	std::list<std::string>		texture_mesh;
	std::list<std::string>		texture_material;
	std::list<std::string>		sequence_mesh;
	std::list<std::string>		sequence_frame;

	while ((flag = getopt_long(ac, av, "", opt, 0)) != -1) {
		switch (flag) {
			case 1:
				texture_mesh.push_back(optarg);
				std::cout << "texture_mesh:" << optarg << std::endl;
				break;
			case 2:
				texture_material.push_back(optarg);
				std::cout << "texture_material:" << optarg << std::endl;
				break;
			case 3:
				sequence_mesh.push_back(optarg);
				std::cout << "sequence_mesh" << optarg << std::endl;
				break;
			case 4:
				sequence_frame.push_back(optarg);
				std::cout << "sequence_frame" << optarg << std::endl;
				break;
			default:
				std::cout << "Unknown command line option: " << (char)flag << std::endl;
				break;
		}
	}

	std::unique_ptr<Demo>	demo(new Demo());

	demo->init(texture_mesh, texture_material, sequence_mesh, sequence_frame);
	demo->run();
	demo->clean();

	return 0;
}