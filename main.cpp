#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

void interior(const char* path) 
{
		using namespace std;
		ifstream file;      
		string oneline;
		int i = 0, j = 0, k = 0;
		int h1 = 0, l = 0;
		char s[20];
		int total;

		file.open(path);
		if (file.fail())
		{
			cout << "Error opening file: " << path << endl;
			exit(1);
		}

		getline(file, oneline, '\n');
		sscanf(oneline.c_str(), "TOTAL %d\n", &total);

							std::cout << total << endl;

		while (!file.eof())
		{
			getline(file, oneline, '\n');

			if (sscanf(oneline.c_str(), "%s %d %d\n", s, &i, &j))
			{
				if (!strcmp("POLYGONS", s))
							 ;
				else if (!strcmp("QUADS", s))
					j = 4;
				else if (!strcmp("TRIANGLES", s))
					j = 3;
							
				std::cout << "_" << endl;

				for (h1 = 0; h1 < i; h1++)
				{
				std::cout << h1 << endl;
					std::cout << endl;
					for (l = 0; l < j; l++)
					{
						do
							getline(file, oneline, '\n');
						while (oneline[0] == '/');
							//std::cout << i<< " " << j << " " << h1 << " " << l << " " << oneline << endl;
					}
				}
			}
		}

		if (k < total) 
			exit(k*10 + 1);

    file.close();
}

int main(int argc, const char *argv[])
{
  interior("world.txt");
	return 0;
}
