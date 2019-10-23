# HW #1

You are given some code. The main purpose of the code handout is to show you expected output. However, feel free to ask questions on Piazza in case of concerns.

Using GCC is not complicated but we slowly will start to use Makefiles. Under the link you can find a tutorial
http://mrbook.org/blog/tutorials/make/
I recommend starting to learn how to use Makefiles as starting from the next homework you will need to write your own.

Most likely 'make' is not yet installed in your VM so please install it by typing the following:
```
$ sudo apt-get install make
```

In your new repository you will have hidden file for Travis CI. Please don't modify it unless we specifically ask. You also will have a `main.c` file. You will use it for the development (feel free to use more *.h files and *.c if needed). You also may see Makefile.config. Please put your data into that file in the appropriate fields. In case you will be doing any of the extra credit options please change corresponding value in this file from zero to one.

You also can see Makefile file. It helps to build you program by issuing a simple command
```
$ make
```
And that's it. Please modify it according to your needs but we still should be able just to run `make` and then everything should work. Most likely you will not need to change it. There is also this README file in the repository.

Below you can find few examples of different output. First two, are when there is some error:

- BUY 4 Painting "Girl on the ball" Picasso 100
- BUY 4 Music Despacito "Luis Fonsi" 25

You program is supposed to print `ID NOT UNIQUE`.

- BUY 4 Painting "Girl on the ball" Picasso 100
- BUY 6 Music Despacito "Luis Fonsi" 25
- DELETE 5
 
Now you should print `RECORD  CANNOT  BE  DELETED  NOR  UPDATED`.

Examples of correct input

- BUY 3 Painting "Girl on the ball" Picasso 100
- BUY 4 Music Despacito "Luis Fonsi" 25

Then imagine we are trying to print all the records in the database assuming that initial budget was 150. We should see the following result:
```
3 Painting Girl on the ball Picasso 100
4 Music Despacito Luis Fonsi 25
25
125
```
- BUY 3 Painting "Girl on the ball" Picasso 100
- BUY 4 Music Despacito "Luis Fonsi" 25
- UPDATE 3 Painting "Girl on the ball" Picasso 125

Then imagine we are trying to print all the records in the database assuming that initial budget was 150. We should see the following result:
```
3 Painting Girl on the ball Picasso 125
4 Music Despacito Luis Fonsi 25
25
150
```
