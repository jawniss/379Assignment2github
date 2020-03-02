/*
buffer using mutex and condition variables. There is a single producer and a single consumer. 
The producer has 6 items, each being a character in the string “HELLO.” it declares at the 
beginning of the producer thread. The producer inserts these 6 items one by one into the 
buffer which can only hold up to 3 items. These items will be inserted into the 3 buffer 
positions following the order 0, 1, 2, 0, 1, 2... If the buffer is full at some point, the 
producer will wait for the consumer to retrieve items until an empty position appears. Then 
the producer can continue. On the other hand, the consumer retrieves and prints items from 
the buffer positions 0, 1, 2, 0, 1, 2,... until there is nothing to be read, at which point, 
it will wait for a new item to be deposited by the producer to proceed. 

Note that the an int member of a user-defined struct is by default initialized to 0.
*/



/**
 * 
 * I think so it takes the T<5> 
 *                          T<2>
 * 
 * so i can take each int, put them into an array buffer
 * then it'll take one by one th ebuffer items, like in this
 * where the buffer holds "HELLO." but instead we'll do buffer holding integers
 * 
 * maybe i can simply change this from the consumers from doign wheatver they're doing with
 * the char array, to then go into the trans function
 * or no first do the same thing - grab an item from the nunber array
 *  then input that number into the trans function
 *      - will have to find a way to make the consumer exit from taking
 *          stuff from array and do trans, cus rn i think
 *          the consumers take a thing, then keep taking,
 *          not just take one and leave type of deal
 * 
 * for the ID of the thread, maybe I can make a global "ID[NUMBER OF THREADS]" array
 * and each thread will access it using a global counter and it will be assigned 
 * ID = ID[globealcounter]
 * globalcounter++          so next htread that takes the id = command will get the next id
 * 
 * 
 * 
 * 
 * 
 * 
 * the example output is how they run the program but idk how they're doing that
 * Charles brought this up
 * 
 * 
 * 
 * 
 * BUFFER.OCCUPIED CAN BE USED AS THE "Q" VALUE: BUFFER.OCCUPIED IS EQUIVILANT TO 
 * HOW MANY ITEMS CURRENTLY IN THE BUFFER
 * 
 */



#include <pthread.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>      // std::ifstream
#include <array>
#include <string>
#include <chrono>
#include <iomanip>


// #define BUFFERSIZE 3                // buffersize = 2 * number of consumers
// #define NUMITEMS 4                  // has to be the number of elements in the producer array of stuff
const int bufferSize = 5;
int numOfTCommands = 0;
int numOfTotalCommands = 0;

int commandsRemaining = 0;
int globalUserDefinedBufferSize = 0;        // cus the other const version can't be passed

int numOfConsumerAsks;



float totalTime = 0.000;


bool producerThreadCanExit = false;
int runningThreads = 0;



int threadID = 1;

using namespace std;


void Trans( int n );                // forward declarations of the functions without using header file cus assign
void Sleep( int n );                // says not to modify tands.c which i have to mod to #include "tands.h"


typedef struct {
    int occupied;                   // counting semaphore
    int nextin, nextout;            // think these are the variables saying which item is to be nextin, nextout
    pthread_mutex_t mutex;
    pthread_cond_t more;            // i think more and less is like 'theres more in the buffer now' or 
    pthread_cond_t less;            // now there's less items cus one was taken
} buffer_t;



buffer_t buffer;
void * producer(void *);
void * consumer(void *);
// #define NUM_THREADS 3               // it looks like this actually makes as many threads as needed
// pthread_t tid[NUM_THREADS];         // thread id?


vector<string> fullVecOfInputs;         // global vector for now cus idk how to pass it into producer with th way producer used by thread
                                        // initialisation in the main




vector<int> completedConsumerTasks;




/*
// http://www.cplusplus.com/reference/ios/ios/eof/
void readFromFile( string fileName )
{
    cout << "reading" << endl;
    ifstream is( fileName );   // open file
    // vector<string> fullVecOfInputs;
    vector<char> tempInput;

  char c;
  while (is.get(c))                  // loop getting single characters
    {
        if( c != '\n' )              // problem is c is just one char, it'll stay here cus that 1 char never changes
        {
            tempInput.push_back( c );                   // IT"S NOT PUSHING INTO THING
        } else {                        // hitting here means it is new line
            std::string tempString(tempInput.begin(), tempInput.end());
            // cout << "tempstring " << tempString << endl;
            fullVecOfInputs.push_back( tempString );
            tempInput.clear();
        }
    }
    // here at end i might have to do one more tempstring push to fullvecofinputs cus 
    // i think it skips the last time doing it
    std::string tempString(tempInput.begin(), tempInput.end());
    cout << "tempstring " << tempString << endl;
    fullVecOfInputs.push_back( tempString );
    tempInput.clear();

  if ( is.eof() )                      // check for EOF
    cout << "[EoF reached]\n";
  else
    cout << "[error reading]\n";


    for( int i = 0; i < fullVecOfInputs.size(); ++i )
    {
        cout << fullVecOfInputs[i] << endl;
    }


    for( int a = 0; a < fullVecOfInputs.size(); ++a )
    {
        if( fullVecOfInputs[a].at(0) == 'T' )
        {
            // cout << "T " << 
            numOfTCommands++;
            commandsRemaining++;
        }
        numOfTotalCommands++;
    }


    // numOfTCommands = fullVecOfInputs.size();
    // commandsRemaining = fullVecOfInputs.size();
  is.close();                        // close file
    cout << "file done bein read" << endl;
}
*/








string prodconLogFileToWriteTo( string prodconLogNumber )
{
    string logFileNamePart1 = "prodcon.";
    string logFileNamePart2 = ".log";
    string logFile = logFileNamePart1 + prodconLogNumber + logFileNamePart2;
    // cout << "Logfile: " << logFile << endl;
    return logFile;
}



// https://stackoverflow.com/questions/8554441/how-to-output-with-3-digits-after-the-decimal-point-with-c-stream
// paxdiablo for flags
// https://stackoverflow.com/questions/29944985/is-there-a-way-to-pass-auto-as-an-argument-in-c
// mike seymour for template t
template <typename T>
void printProgramRealTime( T producerStartTime, int endOfMethod )
{
    auto producerCommandTime = std::chrono::high_resolution_clock::now();
    float prodDuration = std::chrono::duration_cast<std::chrono::microseconds>( producerCommandTime - producerStartTime ).count();

    // Save flags/precision.
    std::ios_base::fmtflags oldflags = std::cout.flags();
    std::streamsize oldprecision = std::cout.precision();

    float prodDurationInSeconds = prodDuration / 1000000;
    if( endOfMethod == 1 )
    {
        totalTime = prodDurationInSeconds;
        // cout << "Total TIME: " << std::setprecision(3) << std::fixed << totalTime << endl;

        // after i would do totalTime / numOFCommands
    } else {
        cout << std::setprecision(3) << std::fixed << prodDurationInSeconds;
    }

    // Restore flags/precision.
    std::cout.flags (oldflags);
    std::cout.precision (oldprecision);
    

}








// http://www.cplusplus.com/reference/ios/ios/eof/
// coudl also put if c == enter
void inputFileRedirection()
{
    // vector<string> fullVecOfInputs;
    vector<char> tempInput;

  char c;
  while ( cin.get(c) and !cin.eof() )                  // loop getting single characters
    {
        if( cin.eof() )
        {
            break;
        }
        if( c != '\n' and c != ' ' and !cin.eof() )              // problem is c is just one char, it'll stay here cus that 1 char never changes
        {
            tempInput.push_back( c );                   // IT"S NOT PUSHING INTO THING
        } else {                        // hitting here means it is new line
            std::string tempString(tempInput.begin(), tempInput.end());
            // cout << "tempstring " << tempString << endl;
            fullVecOfInputs.push_back( tempString );
            tempInput.clear();
        }
    }
    // here at end i might have to do one more tempstring push to fullvecofinputs cus 
    // i think it skips the last time doing it
    std::string tempString(tempInput.begin(), tempInput.end());
    // cout << "tempstring " << tempString << endl;
    fullVecOfInputs.push_back( tempString );
    tempInput.clear();


    // for( int i = 0; i < fullVecOfInputs.size(); ++i )
    // {
    //     cout << fullVecOfInputs[i] << endl;
    // }


    for( int a = 0; a < fullVecOfInputs.size(); ++a )
    {
        if( fullVecOfInputs[a].at(0) == 'T' )
        {
            // cout << "T " << 
            numOfTCommands++;
            commandsRemaining++;
        }
        numOfTotalCommands++;
    }
    // cout << "finput file datekn" << endl;
}





















void * producer( void * parm )          // idk how to pass the fullVecofInputs in to this method with the thead stuff in mains
{
    auto producerStartTime = std::chrono::high_resolution_clock::now();

    int prodID = 0;

    string *localBuffer = (string *) parm;
    vector<string> prodArray;
    for( int a = 0; a < fullVecOfInputs.size(); ++a )
    {
        prodArray.push_back( fullVecOfInputs[a] );
    }
    

    
    // printf("producer started.\n");
    // int i;
    for( int i = 0; i < numOfTotalCommands; i++ )
    { /* produce an item, one character from item[] */
        if ( prodArray.size() == 0 ) 
        {
            // cout << "prodArray size is 0" << endl;
            break;  /* Quit if at end of string. */
        }

        pthread_mutex_lock( &( buffer.mutex ) );            // HERES THE LOCK



        // MAYBE HERE DURING THE PRODUCER LOCK IS WHERE WE GRAB THE INPUT, THE WHOLE THING PAUSES HERE RIGHT
        // no maybe i can't do it here cus maybe there won't be another input
        // and the prod would just be stuck here

        // if (buffer.occupied >= globalUserDefinedBufferSize)              // buffer.occupied is like a counting semaphore
        // {
        //     printf("the producer is WAITING.\n");
        // }

        while (buffer.occupied >= globalUserDefinedBufferSize)
        {
            pthread_cond_wait(&(buffer.less), &(buffer.mutex) );
        }
        // printf("producer executing.\n");


        // cout << "Pushing into sharedbuff: " << prodArray[0] << endl;
        // I SHOULD PUT A IF CHECK HERE, IF THE ITEM IS START WITH S DON'T PUSH IT,
        // JUST DO THE SLEEP COMMAND FUNCTION. ONLY PUSH INTO SHAREDBUFFER IF IT"S
        // A T<n>
        char currentProdArrayItem = prodArray[0].at(0);




        // i put the time in both rather than after the if statements cus when it sleeps it eats a lotta time
        if( currentProdArrayItem == 'T' )               // how to see if first letter in string is S
        {
            // cout << "T Found" << endl;
            localBuffer[buffer.nextin] = prodArray[0];
            buffer.nextin++;
            buffer.occupied++;
            buffer.nextin %= globalUserDefinedBufferSize;
//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------
            cout << "   ";
            printProgramRealTime( producerStartTime, 0 );       // for the work recieved'
            cout << " " << "ID= " << prodID << " " << "Q= " << buffer.occupied << " " << "Work       " << prodArray[0].substr( 1, prodArray.size() - 1 ) << endl;
            
            
            // auto producerCommandTime = std::chrono::high_resolution_clock::now();
            // auto prodDuration = std::chrono::duration_cast<std::chrono::microseconds>( producerCommandTime - producerStartTime ).count();
            // cout << "Producer Time: " << std::fixed << std::setprecision(3) << prodDuration << endl;






        } else if ( currentProdArrayItem == 'S' ) {
            // cout << "S found" << endl;
            // auto producerCommandTime = std::chrono::high_resolution_clock::now();
            // auto prodDuration = std::chrono::duration_cast<std::chrono::microseconds>( producerCommandTime - producerStartTime ).count();
            // cout << "Producer Time: " << std::fixed << std::setprecision(3) << prodDuration << endl;

            cout << "   ";
            printProgramRealTime( producerStartTime, 0 );   // for the sleep
            cout << " " << "ID= " << prodID << "     " << " " << "Sleep      " << prodArray[0].substr( 1, prodArray.size() - 1 ) << endl;


            string intOfSCommand = prodArray[0].substr( 1, prodArray.size() - 1 );
            int transTime = stoi( intOfSCommand );
            // cout << "Sleeptime" << transTime << endl;
            Sleep( transTime );   
        }





        prodArray.erase( prodArray.begin() );



        // cout << "stuff left in the producer arraY:" << " ";
        // for( int a = 0; a < prodArray.size(); a++ )
        // {
        //     cout << prodArray[a] << " ";
        // }
        // cout << endl;
        // buffer.nextin++;
        // buffer.nextin %= bufferSize;



        // buffer.occupied++;



        
        /* now: either buffer.occupied < BUFFERSIZE and buffer.nextin is the index
         of the next empty slot in the buffer, or
         buffer.occupied == BUFFERSIZE and buffer.nextin is the index of the
         next (occupied) slot that will be emptied by a consumer
         (such as buffer.nextin == buffer.nextout) */
        pthread_cond_signal(&(buffer.more));
        pthread_mutex_unlock(&(buffer.mutex));          // UNLOCK  - writing to the file should happen before the unlock
    }
    
    // cout << "sharedbuffer" << " " << localBuffer[0] << " " 
    //     << localBuffer[1] << " " << localBuffer[2] << endl;


    printProgramRealTime( producerStartTime, 1 );       // "end of input for producer" 
    


        /*
        THE ASSIGNEMENT SAID 'DONT LEAVE ANYTHING RUNNING AFTER THE PRODUCER EXITS, YOU'LL BE PENALISED
        SO I THINK I SHOULD BUSY WAIT HERE WHILE CONSUMERS STILL RUNNING
        MAKE ANOTHER LGOBAL VARIABLE COUNTER, IF == 0 THEN EXIT PRODUCER
        */
    while( runningThreads > 0 or producerThreadCanExit == false )
    {

    }
    // printf("producer exiting.\n");

    pthread_exit(0);
}

















void * consumer(void * parm)
{
    auto consumerStartTime = std::chrono::high_resolution_clock::now();
    int conID = threadID;
    threadID++;

    string *localBuffer = (string *) parm;
    int completedTasks = 0;


    runningThreads++;

    string conCurrItem;
    int i;
    // printf("consumer started.\n");
    // cout << "consumer " << conID << " started" << endl;

    // for( i = 0; i < numOfTotalCommands; i++ )
    // while( commandsRemaining > 0 )
    while( true )
    {
        // cout << "consumer " << conID  << "Commands remaining " << commandsRemaining << endl;

        // cout << "consumer " << conID << " " << i << endl;


        // cout << "consumer " << conID  << "before lock " << endl;






        if( commandsRemaining <= 0 )
        {
            // break;
            // cout << "consumer " << conID << " exiting" << endl;
            runningThreads--;
            printProgramRealTime( consumerStartTime, 1 );
            // cout << "Con " << conID << "Compleeted " << completedTasks;
            //  completedConsumerTasks[ conID ].push_back( completedTasks );
            // completedConsumerTasks[ conID ].push_back( completedTasks );

            // completedConsumerTasks[ conID ][ 1 ] = completedTasks;
            // completedConsumerTasks.insert( completedConsumerTasks.begin() + conID, completedTasks );
    completedConsumerTasks[ conID ] = completedTasks;
        cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << "      " << "Ask" << endl;



            pthread_exit(0);
        }






        pthread_mutex_lock( &(buffer.mutex) );                   // LOCK
        // cout << "consumer " << conID  << "after lock"  << endl;


        // HERE CONSUMER ASKS FOR WORK
        // cout << "consumer " << conID << " asking for work " << endl;
        cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << "      " << "Ask" << endl;
        numOfConsumerAsks++;



        // printProgramRealTime( consumerStartTime, 0 );   // for the ask
        // cout << "Buffer.occupied value " << buffer.occupied << endl;








        // if ( buffer.occupied <= 0 ) 
        // {
        //     // cout << "consumer " << conID << " WAITING" << endl;            // This is strickly waiting
        // }

        while( buffer.occupied <= 0 && commandsRemaining > 0 )
        {
            pthread_cond_wait( &(buffer.more), &(buffer.mutex) );            
        }


        if( commandsRemaining <= 0 )
        {
            // break;
            // cout << "consumer " << conID << " exiting" << endl;
            runningThreads--;
            printProgramRealTime( consumerStartTime, 1 );
            // cout << "Con " << conID << "Compleeted " << completedTasks;

            // completedConsumerTasks[ conID ].push_back( completedTasks );
            // completedConsumerTasks[ conID ][ 1 ] = completedTasks;
            // completedConsumerTasks[ conID ].push_back( completedTasks );
            // completedConsumerTasks[ conID ][ 1 ] = completedTasks;
            // completedConsumerTasks.insert( completedConsumerTasks.begin() + conID, completedTasks );
    completedConsumerTasks[ conID ] = completedTasks;
    cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << "      " << "Ask" << endl;




            pthread_exit(0);
        }

        

        // cout << "consumer " << conID << " executing " << endl;
            conCurrItem = localBuffer[buffer.nextout++];
            // cout << "                          Current CONSUMER " << conID << " item: " << conCurrItem << endl;
            
            int commandsWaitingInBuffer = 0;
            if( buffer.occupied > 0 )
            {
                commandsWaitingInBuffer = buffer.occupied - 1;
            } else {
                commandsWaitingInBuffer = buffer.occupied;
            }

            cout << "   ";
            printProgramRealTime( consumerStartTime, 0 );
            cout << " " << "ID= " << conID << " " << "Q= " << commandsWaitingInBuffer << " " << "Recieve    " << conCurrItem.substr( 1, conCurrItem.size() -1 ) << endl;

            buffer.nextout %= globalUserDefinedBufferSize;
            buffer.occupied--;




            commandsRemaining--;


            /* now: either buffer.occupied > 0 and buffer.nextout is the index
            of the next occupied slot in the buffer, or
            buffer.occupied == 0 and buffer.nextout is the index of the next
            (empty) slot that will be filled by a producer (such as
            buffer.nextout == buffer.nextin) */




            pthread_cond_signal(&(buffer.less)); 
            pthread_mutex_unlock(&(buffer.mutex));                         // UNLOCK

            string intOfTCommand = conCurrItem.substr( 1, conCurrItem.size() - 1 );
            int transTime = stoi( intOfTCommand );
            // cout << "Transtime" << transTime << endl;

            // printProgramRealTime( consumerStartTime, 0 );       // for the recieved

            Trans( transTime );

            cout << "   ";
            printProgramRealTime( consumerStartTime, 0 );       // for the completed
            cout << " " << "ID= " << conID << "      " << "Complete   " << conCurrItem.substr( 1, conCurrItem.size() - 1 ) << endl;
            completedTasks++;


            // pthread_cond_signal(&(buffer.less)); 
            // pthread_mutex_unlock(&(buffer.mutex));                         // UNLOCK



        // cout << "consumer " << conID << " finished trans " << endl;
            // break;                                           // with this break each consumer thread takes 1 item and that's it done exits
        
    }

    // this would be in case for some reason it reached the end of the method, failsafe
    // cout << "consumer " << conID << " exiting" << endl;
    runningThreads--;


    // vector<int> tempCompletedConTasksitem( conID, completedTasks );
    // cout << "Con " << conID << "Compleeted " << completedTasks;
    // completedConsumerTasks[ conID ].push_back( completedTasks );
    // completedConsumerTasks[ conID ][ 1 ] = completedTasks;
            // completedConsumerTasks[ conID ].push_back( completedTasks );
            // completedConsumerTasks[ conID ][ 1 ] = completedTasks;
    // completedConsumerTasks.insert( completedConsumerTasks.begin() + conID, completedTasks );
    completedConsumerTasks[ conID ] = completedTasks;
cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << "      " << "Ask" << endl;


    printProgramRealTime( consumerStartTime, 1 );
    pthread_exit(0);
}















/*
the way this prod con system works is the prod will keep tryna put things into the buffer till either the buffer's full
or there's nothing left for it to push to the buffer - only then will the consumer start to work

idk if it's supposed to be like producer will put a thing, the second a thing is available the conumser
will start to try to take thing





At the start producer has lock control until the entire buffer is full - should try to make it so it puts 1 then
unlocks so consumer can instantly try to grab?




sometimes with a lotta threads it'll work, but sometimes not. i have no idea why. like run ./prodcon 5 1 < inputs
might not work, put ./prodcon 5 1 < inputs in again it might work, might not, might, wth



i'm assuming the inputfile is with extension .txt, do i need to change it
so it auto pends .txt to file name?





my implementation of the time inludes the time it sleeps
*/
int main( int argc, char *argv[] ) 
{

    // cout << argv[1] << argv[2]<< argv[3] << endl;
    // i can do the if argc == 3 (means prodcon 3 1)
    // i still gotta do keybaord inputs



    // string prodconInputCommand;
    // getline( cin, prodconInputCommand );
    // vector<string> separatedInput = splitInputCommand( prodconInputCommand );

    // if( separatedInput.size() == 2 )
    // {
    //     cout << "two" << endl;
    //     separatedInput.push_back("0");                // sets the default ID of log file to 0, later will hav to to intToString(sepInput[2])
    // }


    // try to delete the buffer.sharedBuffer and make the new size of it?





    pthread_mutex_lock( &(buffer.mutex) );            // i wanna lock while it's reading from file just in case



    // string fileName;


    // int prodConSize = argc;
    






    // fileName = "exampleInput.txt";
    // cout << fileName <<  " saf"<< endl;



    // readFromFile( fileName );
    inputFileRedirection();



    pthread_mutex_unlock(&(buffer.mutex));              // done readin from file


    string fileName;
    if( argc == 3 )
    {
        fileName = prodconLogFileToWriteTo( argv[2] );
    } else {
        fileName = prodconLogFileToWriteTo( "0" );
    }

    /*
https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
Nawaz
    */

    ofstream out( fileName );
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!





    int userDefinedBufferSize = stoi( argv[1] );


    const int finalSizeOfBuffer = userDefinedBufferSize * 2;
    globalUserDefinedBufferSize = finalSizeOfBuffer;



    string localNonGlobalBuffer[ finalSizeOfBuffer ];

    



    







    int i;
    /* array of thread IDs */
    pthread_cond_init( &(buffer.more), NULL );
    pthread_cond_init( &(buffer.less), NULL );
    // pthread_mutex_init( &buffer.mutex, NULL );
    if ( pthread_mutex_init( &buffer.mutex, NULL) != 0 )
    printf( "mutex init failed\n" );



    // i can actually define how many threads from the user here
    // int numOfThreads = stoi(separatedInput[1]);           // it looks like this actually makes as many threads as needed
    int numOfThreads = stoi(argv[1]); 
    pthread_t tid[numOfThreads];         // thread id?






    completedConsumerTasks.resize( numOfThreads );          




    


    // pthread_create(&tid[1], NULL, consumer, NULL);          // 'consumer' is the method that the thread will start at
    pthread_create(&tid[0], NULL, producer, localNonGlobalBuffer );          // but theres 2 consumers no? why only 1 consumer create? unless &tid[1] makes 2? [0] then [1]?   
    
    
    for( int a = 0; a < numOfThreads; ++a )
    {
        pthread_create(&tid[a], NULL, consumer, localNonGlobalBuffer );          // 'consumer' is the method that the thread will start at
    }
    

    for ( i = 0; i < numOfThreads; i++)
        pthread_join(tid[i], NULL);                         // must be something here with the 2 consumers


    // Should i hav this before the producer finishes? like do they mean producer should be th eabssolute final
    // process to finish, including the main? if so i can throw another global variable here,
    // then put the global variable check into producer's final while loop too
    // printf("\nmain() reporting that all %d threads have terminated\n", i);
        // cout << "Total TIME: " << std::setprecision(3) << std::fixed << totalTime << endl;
        // cout << "Transactions per second: " << std::setprecision(3) << std::fixed << numOfTCommands / totalTime << endl;


    cout << "Summary: " << endl;
    cout << "    Work         " << numOfTCommands << endl;
    cout << "    Ask          " << numOfConsumerAsks << endl;
    cout << "    Recieve      " << numOfTCommands << endl;                        // if it reaches here in the program, all tasks were completed
    cout << "    Complete     " << numOfTCommands << endl;
    cout << "    Sleep        " << numOfTotalCommands - numOfTCommands << endl;
    // for( int i = 1; i <= numOfThreads; ++i )
    // {
    //     cout << "    Thread  " << i << "    " << completedConsumerTasks[i] << endl;;
    // }
    for( int i = 1; i <= numOfThreads; ++i )
    {
        cout << "    Thread  " << i << "    " << completedConsumerTasks[i] << endl;;
    }
        cout << "Transactions per second: " << std::setprecision( 2 ) << std::fixed << numOfTCommands / totalTime << endl;



    producerThreadCanExit = true;
    std::cout.rdbuf(coutbuf); //reset to standard output again
    return 0;
}




/*
    things elft to do
    - get each output line the same
    - rewrite the output to file called prodcon._.log
    - get the summary part to go

*/