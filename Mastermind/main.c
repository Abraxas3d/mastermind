//
//  main.c
//  Mastermind
//
//  Michelle Thompson commenced writing 1/23/18
//  It is cold in the scriptorium and her thumb ached.
//  Coded for the Phase 4 Ground Trans-Ionospheric badge

/*  argc is "argument count". It signifies how many arguments are being passed into the executable.
    argv means "argument values". It is a pointer to an array of characters.
 
 So if you have a program "foo" and execute it like this:
 
 foo -bar baz -theAnswer 42
 Then in your main() function, argc will be 5, and argv will be:
 
 argv[0] = "/full/path/to/foo";
 argv[1] = "-bar";
 argv[2] = "baz";
 argv[3] = "-theAnswer";
 argv[4] = "42";
 */


//if the badge is upside down then display the chosen colors along the bottom
//if the badge is right side up, then display the code pegs.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//-=-=-=-=-=-=-=-=-=-=-
// Mastermind functions
//-=-=-=-=-=-=-=-=-=-=-

//Get an opponent or play the badge

//Show me nearby opponents
int view_opponents(int* neighbors, long NumOfElems)
{
    //get list and populate. This is the neighbor list.
    printf("Potential Opponents:\n");
    if (NumOfElems > 1){
        //there's more than just me out there
        //print the elements of the array
        for(int i = 0; i < NumOfElems; i++) {
            printf("%d ", neighbors[i]);
        }
        return(1);
    }else{
            printf("Looks like it's just you out there\n");
        return(0);
        
    }
}



//issue a challenge to opponent and get an answer
//right now this is just random, 50-50 chance
//we need to either make the badge automagically play
//or else timeout
//I like automagically playing
int challenge_opponent(int challenged)
{
    if (rand()%2 == 0){
        return(0); //challenge declined
    }else{
        return(1); //challenge accepted
    }
    //need a timeout
}


//select opponent from list and issue challenge (call to challenge_opponent)
int select_opponent(int* neighbors, long NumOfElems)
{
    printf("\nSelect opponent from list.\n");
    int challenged;
    scanf( "%d", &challenged);
    printf("we are now challenging %d.\n", challenged);
    //challenge can be declined
    //maybe they wander out of range
    //maybe it's a user setting
    if (challenge_opponent(challenged) == 1){
        //challenge accepted
        int opponent = challenged;
        printf("\nChallenge accepted!!\n");
        return opponent;
    }else{
        printf("Challenge declined. :+(\n");
        return(0);
    }

}



//display chosen colors
void chosen_colors(int cp1, int cp2, int cp3, int cp4){
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Here are the colors you told me to use.\n");
    printf("The colors are %d %d %d %d\n", cp1, cp2, cp3, cp4);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}


//shuffle the array of key pegs
void shuffle(int* array, long NumberOfElements)
{
    if (NumberOfElements > 1)
    {
        long i;
        for (i = 0; i < NumberOfElements - 1; i++)
        {
            long j = i + rand() / (RAND_MAX / (NumberOfElements - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}




int testCode(int* color, int* guess, int* pegs)
{
    //pegs are the key pegs, used to communicate the score of the guess
    //key peg result of 2 is a match in both position and color
    //key peg result of 1 is a match in color, but not position

    //needs to be zero'd out for the Knuth algorithm.
    //doesn't hurt us for when humans play
    pegs[0] = 0;
    pegs[1] = 0;
    pegs[2] = 0;
    pegs[3] = 0;
    

    //punch out all the perfect matches first (we're optimists)
    //Check for exact matches and mark corresponding key peg with a 2
    if(guess[0] == color[0]){
        printf("correct1\n");
        pegs[0] = 2;
    }
    if(guess[1] == color[1]){
        printf("correct2\n");
        pegs[1] = 2;
    }
    if(guess[2] == color[2]){
        printf("correct3\n");
        pegs[2] = 2;
    }
    if(guess[3] == color[3]){
        printf("correct4\n");
        pegs[3] = 2;
    }
    
    //then check remaining guesses to see if the color appears in remaining color code
    //found another bug.
    
    int color_flag[4] = {1, 1, 1, 1};  //trust me
    
    if(pegs[0]==0){
        printf("test guess[0] for color out of place\n");
        //we only work with guess[0] if it wasn't an exact match
        //it's either 0 or 2 at this point of the tests
        //this might work more generally if it was != 2
        //but I don't know that yet
        if((guess[0] == color[1])&&(pegs[1]!=2)){
            //guess[0] matches color[1], but we can't claim it if guess[1] matched color[1] already
            //since pegs[1] = 2 in that condition, we can work this into the test
            printf("guess[0] at color[1] so peg[0] = 1 and color_flag[1] = 0 to remove it from further testing\n");
            pegs[0] = 1;
            color_flag[1] = 0; //we matched with color[1], remove it
        }else{//only execute this if the above condition was false
            if((guess[0] == color[2])&&(pegs[2]!=2)){
                //guess[0] matches color[2], but we can't claim it if guess[2] matched color[2] already
                //since pegs[2] = 2 in that condition, we can work this into the test
                printf("guess[0] at color[2] so pegs[0]=1 and color_flag[2] = 0 to remove it from further testing\n");
                pegs[0] = 1;
                color_flag[2] = 0; //we matched with color[2], remove it
            }else{//only execute this if the above condition was false
                if((guess[0] == color[3])&&(pegs[3]!=2)){
                    //guess[0] matches color[3], but we can't claim it if guess[3] matched color[3] already
                    //since pegs[3] = 2 in that condition, we can work this into the test
                    printf("guess[0] at color[3] so pegs[0]=1 and color_flag[3] = 0 to remove it from further testing\n");
                    pegs[0] = 1;
                    color_flag[3] = 0; //we matched with color[3], remove it
                    
                    
                }
            }
        }
    }
    
    
    if(pegs[1]==0){
        printf("test guess[1] for color out of place\n");
        //we only work with guess[1] if it wasn't an exact match
        //it's either 0 or 2 at this point of the tests
        //this might work more generally if it was != 2
        //but I don't know that yet
        if((guess[1] == color_flag[0]*color[0])&&(pegs[0]!=2)){
            //guess[1] matches color[0], but we can't claim it if guess[0] matched color[0] already
            //since pegs[0] = 2 in that condition, we can work this into the test
            printf("guess[1] at color[0] so peg[1] = 1 and color_flag[0] = 0 to remove it from further testing\n");
            pegs[1] = 1;
            color_flag[0] = 0; //we matched with color[0], remove it
            
            
        }else{//only execute this if the above condition was false
            if((guess[1] == color_flag[2]*color[2])&&(pegs[2]!=2)){
                //guess[1] matches color[2], but we can't claim it if guess[2] matched color[2] already
                //since pegs[2] = 2 in that condition, we can work this into the test
                printf("guess[1] at color[2] so pegs[1]=1 and color_flag[2] = 0 to remove it from further testing\n");
                pegs[1] = 1;
                color_flag[2] = 0; //we matched with color[2], remove it
                
                
            }else{//only execute this if the above condition was false
                if((guess[1] == color_flag[3]*color[3])&&(pegs[3]!=2)){
                    //guess[1] matches color[3], but we can't claim it if guess[3] matched color[3] already
                    //since pegs[3] = 2 in that condition, we can work this into the test
                    printf("guess[1] at color[3] so pegs[1]=1 and color_flag[3] = 0 to remove it from further testing\n");
                    pegs[1] = 1;
                    color_flag[3] = 0; //we matched with color[3], remove it
                    
                    
                }
            }
        }
    }
    
    if(pegs[2]==0){
        printf("test guess[2] for color out of place\n");
        //we only work with guess[2] if it wasn't an exact match
        //it's either 0 or 2 at this point of the tests
        //this might work more generally if it was != 2
        //but I don't know that yet
        if((guess[2] == color_flag[0]*color[0])&&(pegs[0]!=2)){
            //guess[2] matches color[0], but we can't claim it if guess[0] matched color[0] already
            //since pegs[0] = 2 in that condition, we can work this into the test
            printf("guess[2] at color[0] so peg[2] = 1 and color_flag[0] = 0 to remove it from further testing\n");
            pegs[2] = 1;
            color_flag[0] = 0; //we matched with color[0], remove it
            
            
        }else{//only execute this if the above condition was false
            if((guess[2] == color_flag[1]*color[1])&&(pegs[1]!=2)){
                //guess[2] matches color[1], but we can't claim it if guess[1] matched color[1] already
                //since pegs[1] = 2 in that condition, we can work this into the test
                printf("guess[2] at color[1] so pegs[2]=1 and color_flag[1] = 0 to remove it from further testing\n");
                pegs[2] = 1;
                color_flag[1] = 0; //we matched with color[1], remove it
                
                
            }else{//only execute this if the above condition was false
                if((guess[2] == color_flag[3]*color[3])&&(pegs[3]!=2)){
                    //guess[2] matches color[3], but we can't claim it if guess[3] matched color[3] already
                    //since pegs[3] = 2 in that condition, we can work this into the test
                    printf("guess[2] at color[3] so pegs[2]=1 and color_flag[3] = 0 to remove it from further testing\n");
                    pegs[2] = 1;
                    color_flag[3] = 0; //we matched with color[3], remove it
                    
                    
                }
            }
        }
    }
    
    if(pegs[3]==0){
        printf("test guess[3] for color out of place\n");
        //we only work with guess[3] if it wasn't an exact match
        //it's either 0 or 2 at this point of the tests
        //this might work more generally if it was != 2
        //but I don't know that yet
        if((guess[3] == color_flag[0]*color[0])&&(pegs[0]!=2)){
            //guess[3] matches color[0], but we can't claim it if guess[0] matched color[0] already
            //since pegs[0] = 2 in that condition, we can work this into the test
            printf("guess[3] at color[0] so peg[3] = 1 and color_flag[0] = 0 to remove it from further testing\n");
            pegs[3] = 1;
            color_flag[0] = 0; //we matched with color[0], remove it
            
            
        }else{//only execute this if the above condition was false
            if((guess[3] == color_flag[2]*color[2])&&(pegs[2]!=2)){
                //guess[3] matches color[2], but we can't claim it if guess[2] matched color[2] already
                //since pegs[2] = 2 in that condition, we can work this into the test
                printf("guess[3] at color[2] so pegs[3]=1 and color_flag[2] = 0 to remove it from further testing\n");
                pegs[3] = 1;
                color_flag[2] = 0; //we matched with color[2], remove it
                
                
            }else{//only execute this if the above condition was false
                if((guess[3] == color_flag[1]*color[1])&&(pegs[1]!=2)){
                    //guess[3] matches color[1], but we can't claim it if guess[1] matched color[1] already
                    //since pegs[1] = 2 in that condition, we can work this into the test
                    printf("guess[3] at color[1] so pegs[3]=1 and color_flag[1] = 0 to remove it from further testing\n");
                    pegs[3] = 1;
                    color_flag[1] = 0; //we matched with color[1], remove it
                    
                    
                }
            }
        }
    }
    printf("inside testCode, peg count returned is %d\n", pegs[0]+pegs[1]+pegs[2]+pegs[3]);
    return *pegs;
}
    

    
    




















void knuth_activate(int *key_pegs, int *algo_guess, int *active, int b[1296][4], int turn_limit){
    
    
/*
    int from_b[4] = {0}; //the code from all possible codes that we're checking
    int temp_pegs[4] = {0}; //because we don't want to scribble on the ones for the Human interaction
    printf("~~~~~~~~~~~~~~~~~~~~~\n");
    printf("inside knuth_activate for guess #%d\n", turn_limit);
    printf("I've got key pegs: %d %d %d %d\n", key_pegs[0], key_pegs[1], key_pegs[2], key_pegs[3]);
    printf("I've got the most recent guess: %d %d %d %d\n", algo_guess[0], algo_guess[1], algo_guess[2], algo_guess[3]);
    //printf("I have the active list here's some of it %d %d %d\n", active[1], active[2], active[3]);
    //printf("I have the list of all possible codes here's some of it by element %d %d %d %d\n", b[2][0], b[2][1], b[2][2], b[2][3]);
    //printf("Test that I can modify\n");
    //b[2][0] = b[2][0] + 1;

    
    int count = 0;
    for (int i = 0; i < 1296; ++i){
        if(active[i])
            count++;
    }
    
    printf("Searching numbers to choose from. The count of codes to choose from is %d. Our current guess is %d %d %d %d with r = %d\n", count, algo_guess[0], algo_guess[1], algo_guess[2], algo_guess[3], (key_pegs[0] + key_pegs[1] + key_pegs[2] + key_pegs[3]) );
    
    if (turn_limit == 0){//setNumbersWithSameCodeActive
        //for each number with the same # of blacks and whites set "active"
        for (int i = 0; i < 1296; ++i){
            from_b[0] = b[i][0];
            from_b[1] = b[i][1];
            from_b[2] = b[i][2];
            from_b[3] = b[i][3];
            //printf("b[%d] %d %d %d %d is \n",i, b[i][0], b[i][1], b[i][2], b[i][3]);
            //printf("from_b is now %d %d %d %d\n", from_b[0], from_b[1], from_b[2] ,from_b[3]);
            printf("take %d %d %d %d and compare %d %d %d %d\n", from_b[0], from_b[1], from_b[2] ,from_b[3], algo_guess[0], algo_guess[1], algo_guess[2], algo_guess[3]);
            printf("We are looking for anything that has a key peg result of %d\n", key_pegs[0]+key_pegs[1]+key_pegs[2]+key_pegs[3]);

            *temp_pegs = testCode(from_b, algo_guess, temp_pegs);
            printf("after running a test, we get a key peg result of %d\n", temp_pegs[0] + temp_pegs[1] + temp_pegs[2] + temp_pegs[3]);

            if ((temp_pegs[0] + temp_pegs[1] + temp_pegs[2] + temp_pegs[3]) == (key_pegs[0] + key_pegs[1] + key_pegs[2] + key_pegs[3])) {
                printf("\n\n\n\n\n\nfound another possibility that matches the key peg sum\n");
                printf("found another possibility that matches the key peg sum\n");
                printf("found another possibility that matches the key peg sum\n");
                printf("found another possibility that matches the key peg sum\n");
                printf("found another possibility that matches the key peg sum\n\n\n\n\n");
                active[i] = 1;
            }else {
                active[i] = 0;
            }
            

        }
        for (int i = 0; i < 1296; i++){
            printf("%d\n", active[i]);
        }
        

    }
    else {//excludeNumbersWithSameCode
        //for each number with not the same # of blacks and whites set "not active"
        for (int i = 0; i < 1296; ++i){
            from_b[0] = b[i][0];
            from_b[1] = b[i][1];
            from_b[2] = b[i][2];
            from_b[3] = b[i][3];
            //printf("b[%d] %d %d %d %d is \n",i, b[i][0], b[i][1], b[i][2], b[i][3]);
            //printf("from_b is now %d %d %d %d\n", from_b[0], from_b[1], from_b[2] ,from_b[3]);
            printf("take %d %d %d %d and compare %d %d %d %d\n", from_b[0], from_b[1], from_b[2] ,from_b[3], algo_guess[0], algo_guess[1], algo_guess[2], algo_guess[3]);
            printf("We are looking for anything that does not match a key peg result of %d\n", key_pegs[0]+key_pegs[1]+key_pegs[2]+key_pegs[3]);
            
            
            *temp_pegs = testCode(from_b, algo_guess, temp_pegs);
            
            
            printf("after running a test, we get a key peg result of %d\n", temp_pegs[0] + temp_pegs[1] + temp_pegs[2] + temp_pegs[3]);
            
            if (   active[i]   &&    ((temp_pegs[0] + temp_pegs[1] + temp_pegs[2] + temp_pegs[3]) != (key_pegs[0] + key_pegs[1] + key_pegs[2] + key_pegs[3])))
            {
                active[i] = 0;
                printf("\n\n\n\n\n\nfound another possibility that does not match the key peg sum\n");
                printf("found another possibility that does not match the key peg sum\n");
                printf("found another possibility that does not match the key peg sum\n");
                printf("found another possibility that does not match the key peg sum\n");
                printf("found another possibility that does not match the key peg sum\n\n\n\n\n");
            }
        }
    }
        
    printf("~~~~~~~~~~~~~~~~~~~~~\n");
    
 
    */
    

    
    
    
    
    //if we're doing random guessing, then use this
    printf("I'll randomly pick a combination to try.\n");
    algo_guess[0] = (rand() % 6) + 1;
    algo_guess[1] = (rand() % 6) + 1;
    algo_guess[2] = (rand() % 6) + 1;
    algo_guess[3] = (rand() % 6) + 1;
    printf("here's the new guess:\n");
    chosen_colors(algo_guess[0], algo_guess[1], algo_guess[2], algo_guess[3]);
    


}










//core gameplay - does the guess match the color? display key pegs.
bool user_input(int color1, int color2, int color3, int color4, int human){
    int pegs[4] = {0,0,0,0};
    long NumberOfPegs = sizeof(pegs)/sizeof(pegs[0]);
    
    int color[4] = {color1, color2, color3, color4};
    
    int turn_limit = 0;
    
    int guess[4] = {1, 1, 2, 2}; //initial guess for solver
    int b[1296][4] = {0}; //this is the set of all possible codes [1, 1, 1, 1] to [6, 6, 6, 6]
    for (int i = 0; i<1296; i++) {
        
        b[i][0] = (i % 6) + 1;
        b[i][1] = ((i / 6) % 6) + 1;
        b[i][2] = ((i / 36) % 6) + 1;
        b[i][3] = ((i / 216) % 6) + 1;
        
        printf("%d %d %d %d\n", b[i][0], b[i][1], b[i][2], b[i][3]);
    }

//Keep track of the active guesses - this is part of the Knuth algorithm
    int active[1296] = {0};
    for (int i = 0; i<1296; i++) {
        printf("%d\n", active[i]);
    }
    
    
    
    
    
    
    
    while(pegs[0]+pegs[1]+pegs[2]+pegs[3] < 8){
        //while we haven't made a perfect guess, reset the key pegs.
        pegs[0] = 0;
        pegs[1] = 0;
        pegs[2] = 0;
        pegs[3] = 0;

        /*//test code for key pegs
        printf("The key pegs before each guess\n");
        
        for(int i = 0; i < NumberOfPegs; i++) {
            printf("%d ", pegs[i]);
        }
        printf("\n");
         */
        
        
        if(human == 1){
        //human entry of the guesses is here
        printf("Please enter in your guesses.\n");
        printf("first color:");
        scanf( "%d", &guess[0] );
        printf("second color:");
        scanf( "%d", &guess[1] );
        printf("third color:");
        scanf( "%d", &guess[2] );
        printf("fourth color:");
        scanf( "%d", &guess[3] );
        //display the colors chosen for this guess
        chosen_colors(guess[0], guess[1], guess[2], guess[3]);
        }

        
        


        //gameplay defined here - replace with testCode();
        //testCode(color, guess)
        printf("before testCode, peg count is now %d\n", pegs[0]+pegs[1]+pegs[2]+pegs[3]);
        *pegs = testCode(color, guess, pegs);
        printf("after testCode, peg count is now %d\n", pegs[0]+pegs[1]+pegs[2]+pegs[3]);


        //shuffle the key pegs so that people can't tell which was a perfect match
        //and which was a correct color out of place
        
        printf("The key pegs before shuffling\n");
        
        for(int i = 0; i < NumberOfPegs; i++) {
                printf("%d ", pegs[i]);
            }
        printf("\n");
        
        
        shuffle(pegs, NumberOfPegs);

        printf("The key pegs after shuffling\n");

        for(int i = 0; i < NumberOfPegs; i++) {
            printf("%d ", pegs[i]);
        }
        printf("\n");
        
        
        
        

        //random generation of the guesses is here
        //look we're a computer
        if(human == 0){
        printf("Going to go get an algorithmically generated guess\n");
        knuth_activate(pegs, guess, active, b, turn_limit);
        }

        
        
        
        
        
        //enforce turn limits
        
        turn_limit++;
        printf("this was turn number %d\n", turn_limit);
        if(turn_limit >= 12){
            printf("maximum number of turns exceeded!\n");

            return false;
        }
        
        //if code was broken, codebreaker wins

        if (pegs[0]+pegs[1]+pegs[2]+pegs[3] == 8){
            printf("Correct.\n");
            return true;
        }
    }
    printf("Neither victory nor turn limit exceeded. Something went horribly wrong.\n");
    return false;
}





//select colors as the codemaker
bool codemaker(int opponent){
    int yes_or_no;
    bool game_on_status;
    int human = 0;
    
    printf("inside codemaker function\n");
    printf("You are playing %d\n", opponent);
    printf("choose your secret color code\n");
    int code_peg1, code_peg2, code_peg3, code_peg4;

    printf("first color:");
    scanf( "%d", &code_peg1 );
    printf("second color:");
    scanf( "%d", &code_peg2 );
    printf("third color:");
    scanf( "%d", &code_peg3 );
    printf("fourth color:");
    scanf( "%d", &code_peg4 );
    
    chosen_colors(code_peg1, code_peg2, code_peg3, code_peg4);
    
    printf("are you a human (1) or a computer (0)?\n");
    scanf( "%d", &human);
    
    
    if (user_input(code_peg1, code_peg2, code_peg3, code_peg4, human) == false){
        printf("You lost this time.\n");
    }else{
        printf("Congratulations, you have won. Points awarded.\n");
    }
    

    
    printf("want to play again? y or n\n");
    scanf( "%d", &yes_or_no);
    //printf( "You entered %d\n", yes_or_no);
    yes_or_no = getchar();
    //printf("and now yes_or_no is:%d", yes_or_no);
    //printf("\n");
    //putchar(yes_or_no);
    //printf("\n");
    
    if (yes_or_no == 'y'){
        game_on_status = true;
    } else {
        game_on_status = false;
    }
    return(game_on_status);
    
}



bool is_game_on(){
    int yes_or_no;
    bool game_on_status;
    printf("Hello, Mastermind!!\n");
    printf("Shall we play a game?\n");
    scanf( "%d", &yes_or_no );
    //printf( "You entered %d\n", yes_or_no);
    yes_or_no = getchar();
    //printf("and now yes_or_no is:%d", yes_or_no);
    //printf("\n");
    //putchar(yes_or_no);
    //printf("\n");
    
    if (yes_or_no == 'y'){
        game_on_status = true;
    } else {
        game_on_status = false;
    }
    return(game_on_status);
}



int main(int argc, const char * argv[]) {
    int badge; //set badge = my own badge id number so that we can record that we played ourselves no problem
    badge = 31373; //temporary badge number so that I can test
    
    int my_opponent; //badge ID of remote opponent

    int codemaker_score = 0;
    
    int neighbors[] = {badge, 111, 222, 333, 44, 555};
    //int neighbors[] = {badge}; //to test when no one is around to play
 
    long NumberOfElements = sizeof(neighbors)/sizeof(neighbors[0]);
    bool game_on; //consent to be gamered
    
    game_on = is_game_on(); //go get an answer from the UI if we're playing a game or not
    while(game_on){
        if (view_opponents(neighbors, NumberOfElements) == 1){ //there are people within range to play
            my_opponent = select_opponent(neighbors, NumberOfElements);
            if (my_opponent != 0){
                game_on = codemaker(my_opponent); //we start game as codemaker
            }else{
                //opponents were in range, but no one accepted the challenge
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
                printf("Because your challenge was declined\nyou now have a more formidable opponent. Me.\n");
                printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

                game_on = codemaker(badge);
            }
            
        }else{
            //no opponents in range
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
            printf("Because no one happened to be within range,\nYou now have a more formidable opponent. Me.\n");
            printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

            game_on = codemaker(badge);
        }
    }
    return 0;
}



    

