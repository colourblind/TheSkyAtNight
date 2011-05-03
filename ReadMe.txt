Cinder-based skybox generation . . . IN SPACE

Usage:

TheSkyAtNight [square_texture_size] [random_seed]

[square_texture_size]
The square size of the resulting texture for each side. Defaults to 512.

[random_seed]
Seed for the random number generator. Will use a random seed if one is not 
provided

The resulting texture is in the following format:

Forward  Back  Left
Right    Up    Down 
