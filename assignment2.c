#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    int vx;
    int vy;
    int colided;
} Particle;

void updateGrid(char ***arr, int x, int y, Particle *parr, int count)
{

  //allocate memory for 
  char **newArr = (char **)malloc(x * y * sizeof(char *));

  for (int i = 0; i < x; i++)
  {
      newArr[i] = (char *)malloc(y * sizeof(char));
  }

  //create the border
  for (int i = 0; i < x; i++)
  {
      for (int j = 0; j < y; j++)
      {
          if (i == 0 || i == x - 1 || j == 0 || j == y - 1)
          {
              newArr[j][i] = '*';
          }
          else
          {
              newArr[j][i] = ' ';
          }
      }
  }

  for (int p = 0; p < count && parr[p].colided == 0; p++) {
    printf("HERE %d HERE \n", parr[p].colided);
    newArr[parr[p].y + 1][parr[p].x + 1] = '+';
  }

  *arr = newArr;
}

// call this function in a for loop for each second the simulation runs.
void moveParticles(Particle *arr, int length, int gridLengthx, int gridLengthy) {
    // for each particle in the array given.
    for (int i = 0; i < length; i++) {
        if (arr[i].colided == 0) {
            // if the particle is going to move in the x direction
            if (arr[i].vx != 0) {
                // check if the particle will move past the maximum if so bounce, then invert the velocity
                if (arr[i].x + arr[i].vx > gridLengthx) {
                    arr[i].x = gridLengthx - (arr[i].x + arr[i].vx - gridLengthx);
                    arr[i].vx = -arr[i].vx;
                }
                // if past the minimum, bounce and invert
                else if (arr[i].x + arr[i].vx < 0) {
                    arr[i].x = -(arr[i].x + arr[i].vx);
                    arr[i].vx = -arr[i].vx;
                }
                // regularly move the particle
                else {
                    arr[i].x = arr[i].x + arr[i].vx;
                }
            }
            // does the same thing for y as it did for x.

            if (arr[i].vy != 0) {
                if (arr[i].y + arr[i].vy > gridLengthy) {
                    arr[i].y = gridLengthy - (arr[i].y + arr[i].vy - gridLengthy);
                    arr[i].vy = -arr[i].vy;
                } else if (arr[i].y + arr[i].vy < 0) {
                    arr[i].y = -(arr[i].y + arr[i].vy);
                    arr[i].vy = -arr[i].vy;
                } else {
                    arr[i].y = arr[i].y + arr[i].vy;
                }
            }
            //check if the particles have collided. if they collide, set their velocity to 0, making them stop
            printf(" x and y %d %d \n", arr[i].x, arr[i].y);
            for (int a = 0; a < length; a++) {
                if(a != i){
                printf("check");
                if ((arr[i].x == arr[a].x) && (arr[i].y == arr[a].y)) {
                    printf("colisions");
                    arr[i].colided = 1;
                    arr[a].colided = 1;
                  
                }
                }
            }
        }
    }
}
Particle makeParticle(int x, int y, int vx, int vy){
  Particle temp;
  temp.x = x;
  temp.y = y;
  temp.vx = vx;
  temp.vy = vy;
  temp.colided = 0;
  return temp;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input_file> <output_file\n", argv[0]);
        return 1;
    }

    char *inputFileName = argv[1];
    char *outputFileName = argv[2];

    int rows, cols, time;
    Particle particles[1000];

    FILE *file = fopen(inputFileName, "r");
    if (file == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    // Read the row value
    fscanf(file, "%d", &rows);
    // Read the column value
    fscanf(file, "%d", &cols);
    // Read the 
    fscanf(file, "%d", &time);

    int particle_count = 0;
    while (fgetc(file) != 'E' && fscanf(file, "%d, %d, %d, %d", &particles[particle_count].x, &particles[particle_count].y, &particles[particle_count].vx, &particles[particle_count].vy) == 4) { 
      particles[particle_count].colided = 0;
      particle_count++;
    }

    // Allocate memory for the 2D array
    char **arr = (char **)malloc(rows * sizeof(char *));
    if (arr == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < rows; i++)
    {
        arr[i] = (char *)malloc(cols * sizeof(char));
        if (arr[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }
    }

    //the size of the border
    int borderRows = rows + 2;
    int borderCols = cols + 2;


    for (int i = time; i > 0; i--) {
      moveParticles(particles, particle_count, cols, rows);
    }

    updateGrid(&arr, borderRows, borderCols, particles, particle_count);

    for (int i = 0; i < particle_count; i++) {
      printf("%d", particles[i].x);
      printf("%d", particles[i].y);
      printf("%d", particles[i].vx);
      printf("%d\n", particles[i].vy);
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL)
    {
        perror("Error opening output file");
        return 1;
    }

    for (int i = cols+1; i >-1 ; i--)
    {
        for (int j = 0; j < rows + 2; j++)
        {
            fprintf(outputFile, "%c", arr[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    // Free memory
    for (int i = 0; i < rows + 2; i++)
    {
        free(arr[i]);
    }
    free(arr);

    return 0;
}