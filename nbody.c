#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define G 1.0 /* Gravitational Constant */
#define ETA 0.1 /* Softening factor for close collisions */

typedef struct{
	float m; /* mass */
	float x,y,z; /* position */
	float vx,vy,vz; /* velocity */
} body_t;


void ppBody(FILE *fp,body_t* b) {
	fprintf(fp,"<M:%f P(%f,%f,%f) V(%f,%f,%f)>", 
		b->m,	b->x,b->y,b->z,	b->vx,b->vy,b->vz);
}

void printBodies(FILE *fp, int n, body_t* data) {
	int i;
	for (i=0;i<n;i++) {
		fprintf(fp,"B[%i]: ",i);
		ppBody(fp,&data[i]); /* data is pointer to array of body_t's, so get head of array, shift [i] to location, then use address of that point */
		fprintf(fp,"\n");
	}
}

void writeBodies(FILE *fp, int n, body_t* data) {
	int i;
	for (i=0;i<n;i++) {
		fprintf(fp,"%f %f %f %f %f %f %f\n", data[i].m,
			data[i].x,data[i].y,data[i].z,
			data[i].vx,data[i].vy,data[i].vz);
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
	assert(line != NULL);

	while(fgets(line,100,fp) != NULL) {
		if (line[0] != '#') { /* Skip comments */
			if (line_num == 0) {
				sscanf(line,"%i",&n);
				*numBodies = n;

				bodies = (body_t*)malloc( n * sizeof(body_t) ); /* Allocate memory for data */
				assert(bodies != NULL);
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

void updatePositions(int n, body_t* data, float dt) {
	int i;
	for (i=0;i<n;i++) {
		data[i].x += data[i].vx * dt;
		data[i].y += data[i].vy * dt;
		data[i].z += data[i].vz * dt;
	}

}

void updateVelocities(int n, body_t* data, float dt, float* acc) {
	int i;
	for (i=0;i<n;i++) {
		data[i].vx += acc[3*i]   * dt;
		data[i].vy += acc[3*i+1] * dt;
		data[i].vz += acc[3*i+2] * dt;
	}

}

void calcAccelerations(int n, body_t* data, float* accels) {
	int i,j;
	float dx,dy,dz,r, f;
	for (i=0;i<n;i++) {
		accels[3*i]=0;
		accels[3*i+1]=0;
		accels[3*i+2]=0;
	}
	for (i=0;i<n;i++) {
		for (j=i+1;j<n;j++) {
			dx = data[j].x - data[i].x;
			dy = data[j].y - data[i].y;
			dz = data[j].z - data[i].z;
			r = sqrt(dx*dx + dy*dy + dz*dz) + ETA;
			f = G * data[i].m * data[j].m / (r*r*r); /* force/r */

			accels[3*i]  = dx*f;
			accels[3*i+1]= dy*f;
			accels[3*i+2]= dz*f;

			accels[3*j]  = -dx*f;
			accels[3*j+1]= -dy*f;
			accels[3*j+2]= -dz*f;
		}
	}

}

int main(int argc, char *argv[])
{
	char *input_filename;
	FILE *fp = stdout;
	int n; /* number of bodies */
	body_t* bods = NULL; /* pointer to the head of an array of body_t's */
	float t=0, dt,TMAX; /* time info */
	float* acc;

	fprintf(stderr,"\n-------------------------\nN-BODY SIMULATION\n-------------------------\n");
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
	fprintf(stderr,"\n-------------------------\nINITIAL STATE\n-------------------------\n");
	
	fprintf(fp,"%i %f %f\n",n,dt,TMAX);
	writeBodies(fp,n,bods);
	fprintf(stderr,"----TIME %g---\n",t);
	printBodies(stderr,n,bods);	
	
	fprintf(stderr,"\n-------------------------\nSTARTING SIMULATION \tDT:%g\tTMAX:%g\n-------------------------\n",dt,TMAX);
	acc = (float*)malloc(3*n*sizeof(float)); /* Initialize acceleration array (3D x,y,z) */
	assert(acc!=NULL);

	while (t < TMAX) {
		t += dt;
		fprintf(stderr,"----TIME %g----\n",t);

		/* Calculate accelerations */
		calcAccelerations(n,bods,acc);

		/* Update velocities from accelerations */
		updateVelocities(n,bods,dt,acc);

		/* Update positions */
		updatePositions(n,bods,dt);

		/* print info to file or error or whatever */
		writeBodies(fp,n,bods);
		
	}

	fprintf(stderr,"\n-------------------------\nEND OF SIMULATION\n-------------------------\n");
	fprintf(stderr,"End Time : %g",t);
	fprintf(stderr,"\n-------------------------\nFINAL STATE\n-------------------------\n");
	printBodies(stderr,n,bods);
	fprintf(stderr,"\n-------------------------\nEND\n");

	free(bods);
	
	return 0;
}