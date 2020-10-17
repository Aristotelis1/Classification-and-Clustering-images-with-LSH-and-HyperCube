#include <iostream> 
#include <string.h>
#include <fstream>
#include <vector>


#include "functions.h"
#include "structs.h"
#include "structs_cluster.h"


using namespace std;


int main(int argc, char* argv[])
{
    srand(time(NULL));

    int k=-1;
    char input_file[128], config_file[128], output_file[128], method[15], complete[10];
    for (int i=1; i<argc ; i+=2)
    {
        if (strcmp(argv[i],"-i")==0){
            strcpy(input_file,argv[i+1]);
        }else if (strcmp(argv[i],"-c")==0){
            strcpy(config_file,argv[i+1]);
        }else if (strcmp(argv[i],"-method")==0){
            strcpy(method,argv[i+1]);
        }else if (strcmp(argv[i],"-o")==0){
            strcpy(output_file,argv[i+1]);
        }else if(strcmp(argv[i],"-complete")==0){
            strcpy(complete,argv[i+1]);
        }else {
            cout << "Wrong arguments, please try again..." <<endl;
            return 0;
        }
    }

    while (strcmp(input_file, "")==0){
        cout <<"Need input file: ";
        cin >> input_file;
        cout <<endl;
    }while (strcmp(config_file, "")==0){
        cout <<"Need query file: ";
        cin >> config_file;
        cout <<endl;
    }while (strcmp(output_file, "")==0){
        cout <<"Need output file: ";
        cin >> output_file;
        cout <<endl;
    }

    while (k<1){
        cout <<"Give me k > 1 (if you want it on default type '0') : ";
        cin >> k;
        if (k==0){
            k=3;
            break;
        }
    }

        std::ifstream file (input_file);

    int magic_number, number_of_images, rows, columns, dimension;
    unsigned char temp = -1;

    if (file.is_open()){
        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= change_endianess(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= change_endianess(number_of_images);
        file.read((char*)&rows,sizeof(rows));
        rows= change_endianess(rows);
        file.read((char*)&columns,sizeof(columns));
        columns= change_endianess(columns);
        dimension=rows*columns;

       number_of_images = 20;

        //declare vector of images
        vector<Point> all_points;
        vector<vector<unsigned char> > images(number_of_images);
        int sum=0;
        unsigned char byte1, byte2, byte3;
        for(int i = 0; i < number_of_images; ++i){
            sum=0;
            images[i].resize(dimension);
            for(int y=0; y<dimension; ++y){
                file.read((char*)&temp,sizeof(temp));
                images[i][y]=temp;

            }
            get_bytes_from_int (i, byte1, byte2, byte3);        //split int (pos) to 3 bytes and push them at the end of the image vertex
            images[i].push_back(byte1);
            images[i].push_back(byte2);
            images[i].push_back(byte3);
            Point temp(images[i],images[i].size()-3);
            all_points.push_back(temp);

        }
        cout << "Read binary file, with number_of_images = " << number_of_images << " and dimension = " << dimension << endl;

        if(strcmp(method,"classic") == 0)
        {
            cout << "We gonna do classic clustering" << endl;
            KMeans kmeans(k);
            kmeans.run(all_points);
        }


    }else{
        cout<<"Cannot open input file: "<<input_file<<endl;
    }


}