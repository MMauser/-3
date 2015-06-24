#ifndef PERMUTATION_H
#define PERMUTATION_H


struct Combinator {
	int* values;
};

#ifdef __cplusplus
extern "C" {
#endif

	//to initialize k out of n combinator
	//no repitition
	//no order
	int Combinator_Create( struct Combinator** c, int k, int* values, int n );

	//writes next combination to dest
	//dest must be array with n places
	//return   0: success
	//returns != 0: failure
	int Combinator_GetNext( struct Combinator* c, int* dest );

	//free allocated resources
	void Combinator_Free( struct Combinator* c );

#ifdef __cplusplus
}
#endif

#endif