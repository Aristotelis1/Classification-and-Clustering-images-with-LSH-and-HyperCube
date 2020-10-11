#include <iostream> 
#include <string.h>
#include <fstream>
#include <vector>

#include "hash_functions.h"

using namespace std;

int change_endianess (int big_end) 
{
    unsigned char byte1, byte2, byte3, byte4;
    int little_end;
    
    byte1 = big_end & 255;
    byte2 = (big_end >> 8) & 255;
    byte3 = (big_end >> 16) & 255;
    byte4 = (big_end >> 24) & 255;
    little_end = ((int)byte1 << 24) + ((int)byte2 << 16) + ((int)byte3 << 8) + byte4;


    return little_end;
}


  
int main(int argc, char* argv[]) 
{ 
    int i,y,z, k=3, L=5, N=1, M=10;
    float R=1.0;
    char input_file[128], query_file[128], output_file[128];

    for (i=1; i<argc ; i+=2)
    {

        if (strcmp(argv[i],"-d")==0){
            strcpy(input_file,argv[i+1]);
        }else if (strcmp(argv[i],"-q")==0){
            strcpy(query_file,argv[i+1]);
        }else if (strcmp(argv[i],"-k")==0){
            k=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-L")==0){
            L=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-o")==0){
            strcpy(output_file,argv[i+1]);
        }else if (strcmp(argv[i],"-N")==0){
            N=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-R")==0){
            R=atoi(argv[i+1]);
        }else if (strcmp(argv[i],"-t")==0){       //lsh or cube
            cout << "Working on lsh..." << endl;        //TO BE DONE SOMETHING
        }else if (strcmp(argv[i],"-M")==0){
            M=atoi(argv[i+1]);
        }else {
            cout << "Lathos orismata (argv=|" << argv[i] <<"|" << endl;
        }

    }


    //Reading data from binary 


//    int magic_number, number_of_images, rows, columns;
//    ifstream infile (input_file, ios::binary);

    cout << input_file << endl;

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

        //declare vector of images
        vector<vector<unsigned char>> images(number_of_images);
        int sum=0;
        for(i = 0; i < number_of_images; ++i){
            sum=0;
            images[i].resize(dimension);
            for(y=0; y<dimension; ++y){
                file.read((char*)&temp,sizeof(temp));
                images[i][y]=temp;
                // kathisterei poly me th class image
                //im = new Image(temp_str);
                //images.push_back(*im);
//                sum+=images[i][y];
//              std::cout << (int)temp <<"|";
            }
//            cout<<i<<" -->"<< s << "\tLength of vector is:"<< images[i].size()<<"-->"<<(int)images[i][100] <<(int)images[i][200] <<(int)images[i][300] << endl;

        }
        cout << "Read binary file, with number_of_images = " << number_of_images << " and dimension = " << dimension << endl;
        cout << "Mean distance (r) is: " << get_mean_range(50, images) << endl;

        //create a vector for s (normally distributed L*k*d doubles in range [0,w])
        vector<double>s(L*k*dimension);
        double s_range = (double) get_w(get_mean_range(number_of_images/30, images)) / (double) (L * k * dimension);
        for (int i=0; i<L*k*dimension ; i++){
            s[i]= i*s_range;
        }

        Hash_Function hf(dimension, s, 1);
        int w=get_w(get_mean_range(number_of_images/30, images));
        cout<<"Hash key is: "<< hf.get_hash_key(images[100], w)<<endl;;



    }

    return 0; 
} 