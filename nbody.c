#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct{
	float m; /* mass */
	float x,y,z; /* position */
	float vx,vy,vz; /* velocity */
} body_t;


void ppBody(FILE *fp,body_t* b) {
	fprintf(fp,"<M:%f P(%f,%f,%f) V(%f,%f,%f)>", 
		b->m,	b->x,b->y,b->z,	b->vx,b->vy,b->vz);
}

void printBodies(FILE *fp, int n, body_t** data) {
	int i;
	for (i=0;i<n;i++) {
		fprintf(fp,"B[%i]: ",i);
		ppBody(fp,&(*data)[i]); /* data is pointer to array of body_t's, so get head of array, shift [i] to location, then use address of that point */
		fprintf(fp,"\n");
	}
}

/* Load to body_t array data all info from file filename */
void loadFile(const char* filename, int *numBodies, body_t** data) {
	FILE *fp = NULL;
	int n; /* number of bodies */	
	int line_num = 0; /* first line is n, rest are data */
	char *line; /* Line read from file */
	body_t* bodies = NULL; /* Holds all particles */

	/* Start loading file */
	fprintf(stderr,"Loading Input file %s...",filename);
	fp = fopen(filename,"r");
	assert(fp != NULL);

	/* Read in data from file */
	line = malloc(100 * sizeof(char)); /* Get temp memory to read lines */

	while(fgets(line,100,fp) != NULL) {
		if (line[0] != '#') { /* Skip comments */
			if (line_num == 0) {
				sscanf(line,"%i",&n);
				*numBodies = n;

				bodies = (body_t*)malloc( n * sizeof(body_t) ); /* Allocate memory for data */
				line_num++;
			} 
			else {
				sscanf(line,"%f %f %f %f %f %f %f", &bodies[line_num-1].m,
					&bodies[line_num-1].x,  &bodies[line_num-1].y,  &bodies[line_num-1].z,
					&bodies[line_num-1].vx, &bodies[line_num-1].vy, &bodies[line_num-1].vz);				
				line_num++;
			}
		}
	}
	assert(line_num-1==n); /* Read exactly n bodies */
	fprintf(stderr,"Loaded %i bodies.\n",n);

	free(line); /* Clear memory for line */
	*data = bodies; /* return body info */

	fclose(fp);
}

int main(int argc, char *argv[])
{
	char *input_filename;
	int n; /* number of bodies */
	body_t* bods = NULL;
	float t=0, dt,TMAX; /* time info */

	printf("\n-------------------------\nN-BODY SIMULATION\n-------------------------\n");
	if (argc != 4) {
		fprintf(stderr,"\nUSAGE:\n\tnbody INPUT_FILE DT TIMEMAX\n");
		return 0;
	}

	/* Get timing info from arguments */
	sscanf(argv[2], "%f", &dt); assert(dt > 0); /* time step */
	sscanf(argv[3], "%f", &TMAX); assert(TMAX > 0); /* Max Time (end time) */

	/* Load data from file */
	input_filename = argv[1];
	loadFile(input_filename, &n, &bods);
	printf("\n-------------------------\nINITIAL STATE\n-------------------------\n");
	printBodies(stderr,n,&bods);
	printf("\n-------------------------\nSTARTING SIMULATION DT:%g,TMAX:%g\n-------------------------\n",dt,TMAX);
	while (t < TMAX) {
		t += dt;
		printf("----TIME %g----\n",t);

		/* Calculate accelerations */

		/* Update bodies */

		/* print info to file or error or whatever */
		
	}

	printf("\n-------------------------\nEND OF SIMULATION\n-------------------------\n");
	printf("End Time : %g",t);
	printf("\n-------------------------\nFINAL STATE\n-------------------------\n");
	printBodies(stderr,n,&bods);
	printf("\n-------------------------\nEND\n");

	free(bods);
	
	return 0;
}