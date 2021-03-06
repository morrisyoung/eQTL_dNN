// library.h
// function:

#ifndef LIBRARY_H
#define LIBRARY_H




#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>       /* pow */
#include <string>
#include <string.h>
#include <vector>






using namespace std;






class Map_list
{
	int dimension;									// number of genes
	int * list_start;								// list of start pos
	int * list_end;									// list of end pos
	int * list_indi_cis;							// indicating whether or not this gene has cis- SNPs

public:

	// used for loading data, since we might use other containers to load data first of all
	void init(vector<vector<int>> & container)
	{
		dimension = container.size();
		list_start = (int *)calloc( dimension, sizeof(int) );
		list_end = (int *)calloc( dimension, sizeof(int) );
		list_indi_cis = (int *)calloc( dimension, sizeof(int) );

		for(int i=0; i<dimension; i++)
		{
			list_start[i] = (container.at(i)).at(0);
			list_end[i] = (container.at(i)).at(1);

			if (list_end[i] == -1)
			{
				list_indi_cis[i] = 0;
			}
			else
			{
				list_indi_cis[i] = 1;
			}
		}

		return;
	}

	int get_start_at(int pos)
	{
		return list_start[pos];
	}

	int get_end_at(int pos)
	{
		return list_end[pos];
	}

	int * get_list_start()
	{
		return list_start;
	}

	int * get_list_end()
	{
		return list_end;
	}

	int * get_list_indi_cis()
	{
		return list_indi_cis;
	}


	// delete object
	void release()
	{
		free(list_start);
		free(list_end);
		free(list_indi_cis);
		return;
	}

};





//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============





class Matrix
{
	int dimension1;
	int dimension2;
	float * matrix;

public:

	void init(int length1, int length2)
	{
		dimension1 = length1;
		dimension2 = length2;
		matrix = (float *)calloc( length1 * length2, sizeof(float) );
		return;
	}


	// used for loading data, since we might use other containers to load data first of all
	void init(vector<vector<float>> & container)
	{
		dimension1 = container.size();
		dimension2 = (container.at(0)).size();
		matrix = (float *)calloc( dimension1 * dimension2, sizeof(float) );

		int count = 0;
		for(int i=0; i<dimension1; i++)
		{
			for(int j=0; j<dimension2; j++)
			{
				float value = (container.at(i)).at(j);
				matrix[count] = value;
				count += 1;
			}
		}

		return;
	}


	float * get_array_at(int pos)
	{
		return matrix + (pos-1)*dimension2;
	}

	void fill_with_ref_list(int * list_indiv_pos, float * matrix_ref)
	{
		for(int i=0; i<dimension1; i++)
		{
			int indiv_pos = list_indiv_pos[i];
			float * pointer_ref = matrix_ref + indiv_pos * dimension2;
			float * pointer = matrix + i * dimension2;
			memcpy( pointer, pointer_ref, dimension2*sizeof(float) );
		}
		return;
	}


	void append_column_one()
	{
		// the content update
		float * matrix_new = (float *)calloc( dimension1 * (dimension2+1), sizeof(float) );
		for(int i=0; i<dimension1; i++)
		{
			float * pointer = matrix_new + i*(dimension2+1);
			float * pointer_ref = matrix + i*dimension2;
			memcpy( pointer, pointer_ref, dimension2*sizeof(float) );
			*(pointer + dimension2) = 1;
		}
		free(matrix);
		matrix = matrix_new;
		// the boundary update
		dimension1 = dimension1;
		dimension2 = dimension2+1;
		return;
	}


	int get_dimension1()
	{
		return dimension1;
	}

	int get_dimension2()
	{
		return dimension2;
	}

	float * get_pointer()
	{
		return matrix;
	}


	void print_shape()
	{
		cout << "this matrix has shape: " << dimension1 << ", " << dimension2 << endl;
		return;
	}


	// given a filename, try to save this matrix into a file
	void save(char * filename)
	{
		FILE * file_out = fopen(filename, "w+");
		if(file_out == NULL)
		{
		    fputs("File error\n", stderr); exit(1);
		}

		for(int i=0; i<dimension1; i++)
		{
			int start = i * dimension2;
			for(int j=0; j<dimension2; j++)
			{
				int index = start + j;
				float value = matrix[index];
				char buf[1024];
				sprintf(buf, "%f\t", value);
				fwrite(buf, sizeof(char), strlen(buf), file_out);
			}
			fwrite("\n", sizeof(char), 1, file_out);
		}
		fclose(file_out);
		return;
	}


	// delete object
	void release()
	{
		free(matrix);
		return;
	}



	/*
	void init(int length1, int length2, float value)
	{
		dimension1 = length1;
		dimension2 = length2;
		matrix = (float *)calloc( length1 * length2, sizeof(float) );

		for(int i=0; i<length1*length2; i++)
		{
			matrix[i] = value;
		}
		return;
	}

	void init(int length1, int length2)
	{
		dimension1 = length1;
		dimension2 = length2;
		matrix = (float *)calloc( length1 * length2, sizeof(float) );
		return;
	}

	void init(int length1, int length2, float * data)
	{
		dimension1 = length1;
		dimension2 = length2;
		matrix = (float *)calloc( length1 * length2, sizeof(float) );

		int amount = dimension1*dimension2;
		memcpy( matrix, data, amount*sizeof(float) );

		return;
	}

	float get_element(int ind1, int ind2)
	{
		int index = ind1 * dimension2 + ind2;
		return matrix[index];
	}

	void set_element(int ind1, int ind2, float value)
	{
		int index = ind1 * dimension2 + ind2;
		matrix[index] = value;
		return;
	}

	// add on a value to the matrix entry, in place
	void addon(int ind1, int ind2, float value)
	{
		int index = ind1 * dimension2 + ind2;
		matrix[index] += value;
		return;
	}

	// sum all elements
	float sum()
	{
		float result = 0;
		for(int i=0; i<dimension1*dimension2; i++)
		{
			result += matrix[i];
		}
		return result;
	}

	// SOS for all elements
	float sum_of_square()
	{
		float result = 0;
		for(int i=0; i<dimension1*dimension2; i++)
		{
			result += pow(matrix[i], 2.0);
		}
		return result;
	}

	// SOS for on array
	float sum_of_square(int ind)
	{
		float result = 0;
		int start = ind * dimension2;
		for(int i=0; i<dimension2; i++)
		{
			result += pow(matrix[start+i], 2.0);
		}
		return result;
	}

	// raise all elements to the power of float exp
	void power(float exp)
	{
		for(int i=0; i<dimension1*dimension2; i++)
		{
			matrix[i] = pow(matrix[i], exp);
		}
		return;
	}

	// times all elements with coef
	void multiply(float coef)
	{
		for(int i=0; i<dimension1*dimension2; i++)
		{
			matrix[i] = matrix[i] * coef;
		}
		return;
	}
	*/

};





//================
class Tensor
{
	long int dimension1;
	long int dimension2;
	long int dimension3;
	float * tensor;

public:

	// used for loading data, since we might use other containers to load data first of all
	void init(vector<vector<vector<float>>> & container)
	{
		dimension1 = container.size();
		dimension2 = (container.at(0)).size();
		dimension3 = ((container.at(0)).at(0)).size();
		long int dimension = dimension1 * dimension2 * dimension3;
		tensor = (float *)calloc( dimension, sizeof(float) );

		long int count = 0;
		for(long int i=0; i<dimension1; i++)
		{
			for(long int j=0; j<dimension2; j++)
			{
				for(long int d=0; d<dimension3; d++)
				{
					float value = ((container.at(i)).at(j)).at(d);
					tensor[count] = value;
					count += 1;
				}
			}
		}

		return;
	}

	void init(long int length1, long int length2, long int length3)
	{
		dimension1 = length1;
		dimension2 = length2;
		dimension3 = length3;
		tensor = (float *)calloc( dimension1 * dimension2 * dimension3, sizeof(float) );
		return;
	}


	long int get_dimension1()
	{
		return dimension1;
	}

	long int get_dimension2()
	{
		return dimension2;
	}

	long int get_dimension3()
	{
		return dimension3;
	}

	// NOTE: use the following function carefully -- do not modify elements from outside
	float * get_tensor()
	{
		return tensor;
	}

	float * get_matrix_at(int pos)
	{
		float * pointer = tensor + pos*(dimension2*dimension3);
		return pointer;
	}

	void print_shape()
	{
		cout << "this tensor has shape: " << dimension1 << ", " << dimension2 << ", " << dimension3 << endl;
		return;
	}


	// given a filename, try to save this tensor into a file
	void save(char * filename)
	{
		FILE * file_out = fopen(filename, "w+");
		if(file_out == NULL)
		{
		    fputs("File error\n", stderr); exit(1);
		}

		char buf[1024];
		sprintf(buf, "%d\t", dimension1);
		fwrite(buf, sizeof(char), strlen(buf), file_out);
		sprintf(buf, "%d\t", dimension2);
		fwrite(buf, sizeof(char), strlen(buf), file_out);
		sprintf(buf, "%d\t", dimension3);
		fwrite(buf, sizeof(char), strlen(buf), file_out);
		fwrite("\n", sizeof(char), 1, file_out);

		for(long int k=0; k<dimension1; k++)
		{
			for(long int i=0; i<dimension2; i++)
			{
				long int start = k * dimension2 * dimension3 + i * dimension3;
				for(long int j=0; j<dimension3; j++)
				{
					long int index = start + j;
					float value = tensor[index];
					char buf[1024];
					sprintf(buf, "%f\t", value);
					fwrite(buf, sizeof(char), strlen(buf), file_out);
				}
				fwrite("\n", sizeof(char), 1, file_out);
			}
		}
		fclose(file_out);
		return;
	}


	// delete object
	void release()
	{
		free(tensor);
		return;
	}


	/*
	void init(long int length1, long int length2, long int length3)
	{
		dimension1 = length1;
		dimension2 = length2;
		dimension3 = length3;
		long int dimension = length1 * length2 * length3;
		tensor = (float *)calloc( dimension, sizeof(float) );
		return;
	}

	void init(long int length1, long int length2, long int length3, float value)
	{
		dimension1 = length1;
		dimension2 = length2;
		dimension3 = length3;
		long int dimension = length1 * length2 * length3;
		tensor = (float *)calloc( dimension, sizeof(float) );

		long int amount = dimension1*dimension2*dimension3;
		for(long int i=0; i<amount; i++)
		{
			tensor[i] = value;
		}

		return;
	}

	void init(long int length1, long int length2, long int length3, float * data)
	{
		dimension1 = length1;
		dimension2 = length2;
		dimension3 = length3;
		long int dimension = length1 * length2 * length3;
		tensor = (float *)calloc( dimension, sizeof(float) );

		long int amount = dimension1*dimension2*dimension3;
		memcpy( tensor, data, amount*sizeof(float) );

		return;
	}

	float * get_tensor_at(long int i)
	{
		long int shift = i * dimension2 * dimension3;
		return (tensor+shift);
	}

	float get_element(long int ind1, long int ind2, long int ind3)
	{
		long int index = ind1 * dimension2 * dimension3 + ind2 * dimension3 + ind3;
		return tensor[index];
	}

	void set_element(long int ind1, long int ind2, long int ind3, float value)
	{
		long int index = ind1 * dimension2 * dimension3 + ind2 * dimension3 + ind3;
		tensor[index] = value;
		return;
	}

	float sum()
	{
		float result = 0;
		for(long int i=0; i<dimension1*dimension2*dimension3; i++)
		{
			result += tensor[i];
		}
		return result;
	}

	// SOS for one layer
	float sum_of_square(long int ind)
	{
		float result = 0;
		long int start = ind * dimension2 * dimension3;
		for(long int i=0; i<dimension2*dimension3; i++)
		{
			result += pow(tensor[start + i], 2.0);
		}
		return result;
	}

	// raise all elements to the power of float exp
	void power(float exp)
	{
		for(long int i=0; i<dimension1*dimension2*dimension3; i++)
		{
			tensor[i] = pow(tensor[i], exp);
		}
		return;
	}
	*/


};




//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============
//=============/=============/=============/=============/=============/=============/=============/=============




class Tensor_expr
{
	int dimension1;										// number of tissues
	//vector<vector<int>> list_list_indiv_pos;			// contains the mappings to pos of individuals
	//int ** list_list_indiv_pos;						// same as above
	vector<int *> list_list_indiv_pos;					// same as above
	vector<int> list_dimension2;						// number of samples for different tissue
	int dimension3;										// number of genes
	vector<float *> list_matrix;						// expression matrix for different tissues

public:


	void init_incomp(vector<vector<vector<float>>> vec_tensor_expr, vector<vector<int>> vec_indiv_pos_list)
	{
		//int dimension1;										// number of tissues
		dimension1 = vec_tensor_expr.size();
		//vector<int *> list_list_indiv_pos;					// same as above
		//vector<int> list_dimension2;						// number of samples for different tissue
		for(int i=0; i<dimension1; i++)
		{
			int dimension2 = (vec_tensor_expr.at(i)).size();
			list_dimension2.push_back(dimension2);
			int * pointer = (int *)calloc( dimension2, sizeof(int) );
			for(int j = 0; j<dimension2; j++)
			{
				int pos = (vec_indiv_pos_list.at(i)).at(j);
				pointer[j] = pos;
			}
			list_list_indiv_pos.push_back(pointer);
		}
		//int dimension3;										// number of genes
		dimension3 = ((vec_tensor_expr.at(0)).at(0)).size();
		//vector<float *> list_matrix;						// expression matrix for different tissues
		for(int i=0; i<dimension1; i++)
		{
			int dimension2 = list_dimension2.at(i);
			float * pointer = (float *)calloc( dimension2*dimension3, sizeof(float) );

			int count = 0;
			for(int count1=0; count1<dimension2; count1++)
			{
				for(int count2=0; count2<dimension3; count2++)
				{
					float value = ((vec_tensor_expr.at(i)).at(count1)).at(count2);
					pointer[count] = value;
					count += 1;
				}
			}

			list_matrix.push_back(pointer);
		}

		return;
	}


	void init_full(int length1, int length2, int length3, float * pointer_tensor)
	{
		//
		dimension1 = length1;
		//
		for(int i=0; i<dimension1; i++)
		{
			list_dimension2.push_back(length2);
			int * pointer = (int *)calloc( length2, sizeof(int) );
			list_list_indiv_pos.push_back(pointer);
			for(int j = 0; j<length2; j++)
			{
				(list_list_indiv_pos.at(i))[j] = j;
			}
		}
		//
		dimension3 = length3;
		//
		for(int i=0; i<dimension1; i++)
		{
			float * pointer = (float *)calloc( length2*length3, sizeof(float) );
			list_matrix.push_back(pointer);

			float * pointer_ref = pointer_tensor + i*(length2*length3);
			memcpy( list_matrix.at(i), pointer_ref, (length2*length3)*sizeof(float) );
		}

		return;
	}


	void append(int length1, int length2, int length3, int * pointer_indiv, float * pointer_data)
	{
		//
		dimension1 = length1;
		//
		list_dimension2.push_back(length2);
		//
		dimension3 = length3;
		//
		list_list_indiv_pos.push_back(pointer_indiv);
		//
		list_matrix.push_back(pointer_data);

		return;
	}



	int get_dimension2_at(int pos)
	{
		return list_dimension2.at(pos);
	}

	int * get_list_dimension2()
	{
		int * pointer = list_dimension2.data();
		return pointer;
	}


	int get_dimension3()
	{
		return dimension3;
	}

	float * get_matrix_at(int pos)
	{
		return list_matrix.at(pos);
	}

	int * get_list_indiv_pos_at(int pos)
	{
		return list_list_indiv_pos.at(pos);
	}

	int get_indiv_pos_at(int index_tissue, int pos)
	{
		return (list_list_indiv_pos.at(index_tissue))[pos];
	}


	float * get_sample_array_at(int index_tissue, int index_sample)
	{
		float * pointer = list_matrix.at(index_tissue) + index_sample*dimension3;
		return pointer;
	}


	// delete object
	void release()
	{
		for(int i=0; i<dimension1; i++)
		{
			free(list_list_indiv_pos.at(i));
			free(list_matrix.at(i));
		}

		return;
	}

};






class Tensor_beta_cis
{
	int dimension1;										// number of tissues
	int dimension2;										// number of genes
	//vector<int> list_dimension3;						// number of dimension3 for different dimension2 (gene)
	int * list_dimension3;								// same as above
	int * list_start;									// kind of same as above
	int amount;											// total amount of cis- parameters (for one tissue)
	vector<float *>	list_incomp_matrix;					// incomplete beta matrix for different tissues
	int * list_beta_cis_geneindex;						// mapping each parameter to their gene index


public:


	// used for loading data, since we might use other containers to load data first of all
	void init(vector<vector<vector<float>>> & container)
	{
		//==== meta info
		dimension1 = container.size();
		dimension2 = (container.at(0)).size();
		//
		list_dimension3 = (int *)calloc( dimension2, sizeof(int) );
		amount = 0;
		for(int i=0; i<dimension2; i++)
		{
			int dimension3 = ((container.at(0)).at(i)).size();
			amount += dimension3;
			list_dimension3[i] = dimension3;
		}
		//
		list_start = (int *)calloc( dimension2, sizeof(int) );
		for(int i=0; i<dimension2; i++)
		{
			if(i==0)
			{
				list_start[i] = 0;
			}
			else
			{
				list_start[i] = list_start[i-1] + list_dimension3[i-1];
			}
		}
		//
		list_beta_cis_geneindex = (int *)calloc( amount, sizeof(int) );
		int pos = 0;
		for(int i=0; i<dimension2; i++)
		{
			int dimension3 = list_dimension3[i];
			for(int j=0; j<dimension3; j++)
			{
				list_beta_cis_geneindex[pos] = i;
				pos += 1;
			}
		}


		//==== fill in (so ugly...)
		for(long int i=0; i<dimension1; i++)
		{
			float * pointer = (float *)calloc( amount, sizeof(float) );
			list_incomp_matrix.push_back(pointer);

			int pos = 0;
			for(int j=0; j<dimension2; j++)
			{
				int dimension3 = list_dimension3[j];
				for(int d=0; d<dimension3; d++)
				{
					float value = ((container.at(i)).at(j)).at(d);
					(list_incomp_matrix.at(i))[pos] = value;
					pos += 1;
				}
			}
		}

		return;
	}

	// binary fill in
	void init(int ref_dimension1, int ref_dimension2, int * ref_list_dimension3, int * ref_list_start, int ref_amount, vector<float *> & ref_list_incomp_matrix, int * ref_list_beta_cis_geneindex)
	{
		//==== meta info
		dimension1 = ref_dimension1;
		dimension2 = ref_dimension2;
		//
		list_dimension3 = ref_list_dimension3;
		list_start = ref_list_start;
		amount = ref_amount;
		//
		list_beta_cis_geneindex = ref_list_beta_cis_geneindex;

		//
		for(int i=0; i<dimension1; i++)
		{
			float * pointer = ref_list_incomp_matrix.at(i);
			list_incomp_matrix.push_back(pointer);
		}

		return;
	}



	int get_dimension1()
	{
		return dimension1;
	}

	int get_dimension2()
	{
		return dimension2;
	}

	int * get_list_dimension3()
	{
		return list_dimension3;
	}

	int * get_list_start()
	{
		return list_start;
	}

	float * get_incomp_matrix_at(int pos)
	{
		return list_incomp_matrix.at(pos);
	}

	int get_amount()
	{
		return amount;
	}

	int * get_list_beta_cis_geneindex()
	{
		return list_beta_cis_geneindex;
	}


	int get_start_beta(int index_gene)
	{
		return list_start[index_gene];
	}

	int get_end_beta(int index_gene)
	{
		return (list_start[index_gene] + list_dimension3[index_gene] - 1);
	}



	// // given a filename, try to save this tensor into a file
	// void save(char * filename)
	// {
	// 	FILE * file_out = fopen(filename, "w+");
	// 	if(file_out == NULL)
	// 	{
	// 	    fputs("File error\n", stderr); exit(1);
	// 	}

	// 	char buf[1024];
	// 	sprintf(buf, "%d\t", dimension1);
	// 	fwrite(buf, sizeof(char), strlen(buf), file_out);
	// 	sprintf(buf, "%d\t", dimension2);
	// 	fwrite(buf, sizeof(char), strlen(buf), file_out);
	// 	fwrite("\n", sizeof(char), 1, file_out);

	// 	for(long int k=0; k<dimension1; k++)
	// 	{
	// 		float * matrix = list_incomp_matrix.at(k);
	// 		int pos = 0;

	// 		for(long int i=0; i<dimension2; i++)
	// 		{
	//			int dimension3 = list_dimension3[i];
	// 			for(int j=0; j<dimension3; j++)
	// 			{
	// 				//
	// 				float value = matrix[pos];
	// 				char buf[1024];
	// 				sprintf(buf, "%f\t", value);
	// 				fwrite(buf, sizeof(char), strlen(buf), file_out);
	// 				//
	// 				pos += 1;
	// 			}
	// 			fwrite("\n", sizeof(char), 1, file_out);
	// 		}

	// 	}
	// 	fclose(file_out);
	// 	return;
	// }


	// delete object
	void release()
	{
		free(list_start);
		for(int i=0; i<dimension1; i++)
		{
			free(list_incomp_matrix.at(i));
		}
		free(list_beta_cis_geneindex);

		return;
	}


};





#endif

// end of library.h


