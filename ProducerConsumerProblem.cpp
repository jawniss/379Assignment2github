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
 * 
 * 
 * 
 * 
 * i chose to use mutex cus it's super straightforwad looking - if a thread is tryna
 * use an area, lock it. simple
 * 
 */



#include <pthread.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <chrono>
#include <iomanip>


int numOfTCommands = 0;                     // Number of T<n> commands
int numOfTotalCommands = 0;                 // Number of T<n> and S<n> commands
int commandsRemaining = 0;                  // Commands left to execute (T<n> and S<n>)
int globalUserDefinedBufferSize = 0;        // Buffer size
int numOfConsumerAsks;                      // Number of asks made by all consumers

float totalTime = 0.000;                    // Total real runtime

bool producerThreadCanExit = false;         // Bool for if the producer thread can terminate as it must be the last
int runningConsumerThreads = 0;             // Number of still actively running Consumer threads

int threadID = 1;                           // Thread ID for the consumers

using namespace std;

void Trans( int n );                        // Forward declarations of the provided functions without using header file
void Sleep( int n );

typedef struct {
    int numOfBufferItems;                   // Counter of how many items currently in the buffer
    int nextin, nextout;                    // Index of which buffer item is next to be taken out
    pthread_cond_t more;                    // Indicates there's more items in the buffer now
    pthread_cond_t less;                    // Indicates there's less items in the buffer now
    pthread_mutex_t mutex;                  // Mutual exclusion lock
} buffer_t;

buffer_t buffer;


vector<string> fullVecOfInputs;             // Inputted commands (T<n> and S<n>) that will be given to producer to read from

vector<int> completedConsumerTasks;         // Vector to keep track of how many tasks each consumer completed


/**
 * Function to get the desired log file number and make the full name of the file
 * 
 * @param prodconLogNumber The number the prodcon output file is to have (taken from the command line arguments)
 * @return logFile The concatenated name of the logfile
 */
string prodconLogFileToWriteTo( string prodconLogNumber )
{
    string logFileNamePart1 = "prodcon.";
    string logFileNamePart2 = ".log";
    string logFile = logFileNamePart1 + prodconLogNumber + logFileNamePart2;
    return logFile;
}


/**
 * https://stackoverflow.com/questions/8554441/how-to-output-with-3-digits-after-the-decimal-point-with-c-stream
 * User "paxdiablo" for flags
 * https://stackoverflow.com/questions/29944985/is-there-a-way-to-pass-auto-as-an-argument-in-c
 * User "Mike Seymour" for template t
 * 
 * Prints the current running real time when called
 * 
 * @param producerStartTime The time the program started
 * @param endOfMethod A check to see if the program is ended or not, if ended add the time to the total time
 */
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
    } else {
        cout << std::setprecision(3) << std::fixed << prodDurationInSeconds;
    }

    // Restore flags/precision.
    std::cout.flags (oldflags);
    std::cout.precision (oldprecision);
}


/**
 * http://www.cplusplus.com/reference/ios/ios/eof/
 * 
 * Function to take in the inputs and put it into the array the producer will get items from
 * Inputs from either an input file or from the keyboard
 */
void inputFileRedirection()
{
    vector<char> tempInput;
    char c;
    while ( cin.get(c) )
    {
        if( c != '\n' and c != ' ' )
        {
            tempInput.push_back( c );
        } else {
            std::string tempString(tempInput.begin(), tempInput.end());
            fullVecOfInputs.push_back( tempString );
            tempInput.clear();
        }
    }
    std::string tempString(tempInput.begin(), tempInput.end());     // Have to do it one more time because on the final eof
    fullVecOfInputs.push_back( tempString );                        // it doesn't push the final item into the vector
    tempInput.clear();

    for( int a = 0; a < fullVecOfInputs.size(); ++a )
    {
        if( fullVecOfInputs[a].at(0) == 'T' )                       // Counting how many T commands
        {
            numOfTCommands++;
            commandsRemaining++;
        }
        numOfTotalCommands++;
    }
}


/**
 * https://www.linuxquestions.org/questions/programming-9/passing-array-to-thread-function-825699/
 * User "paulsm4" on how to pass array to thread function using *parm
 * 
 * The producer, takes items one by one from the inputs vector and pushes them to the sharedbuffer
 * 
 * @param parm Pointer to the inputs array
 */
void * producer( void * parm )
{
    auto producerStartTime = std::chrono::high_resolution_clock::now();
    int prodID = 0;

    string *localBuffer = (string *) parm;
    vector<string> prodArray;
    for( int a = 0; a < fullVecOfInputs.size(); ++a )
    {
        prodArray.push_back( fullVecOfInputs[a] );
    }
    
    for( int i = 0; i < numOfTotalCommands; i++ )
    {
        if ( prodArray.size() == 0 ) 
        {
            break;
        }
        pthread_mutex_lock( &( buffer.mutex ) );            // LOCK

        while (buffer.numOfBufferItems >= globalUserDefinedBufferSize)      // Producer is waiting to put more things
        {
            pthread_cond_wait(&(buffer.less), &(buffer.mutex) );
        }

        char currentProdArrayItem = prodArray[0].at(0);
        if( currentProdArrayItem == 'T' )                    // T<n> Command
        {
            localBuffer[buffer.nextin] = prodArray[0];
            buffer.nextin++;
            buffer.numOfBufferItems++;
            buffer.nextin %= globalUserDefinedBufferSize;

            cout << "   ";
            printProgramRealTime( producerStartTime, 0 );       
            cout << " " << "ID= " << prodID << " " << "Q= " << buffer.numOfBufferItems << " " << "Work       " << prodArray[0].substr( 1, prodArray.size() - 1 ) << endl;
        } else if ( currentProdArrayItem == 'S' ) {         // S<n> command
            cout << "   ";
            printProgramRealTime( producerStartTime, 0 );
            cout << " " << "ID= " << prodID << "     " << " " << "Sleep      " << prodArray[0].substr( 1, prodArray.size() - 1 ) << endl;

            string intOfSCommand = prodArray[0].substr( 1, prodArray.size() - 1 );
            int transTime = stoi( intOfSCommand );
            Sleep( transTime );   
        }
        prodArray.erase( prodArray.begin() );
        pthread_cond_signal(&(buffer.more));
        pthread_mutex_unlock(&(buffer.mutex));          // UNLOCK
    }
    printProgramRealTime( producerStartTime, 1 );
    while( runningConsumerThreads > 0 or producerThreadCanExit == false )   // Wait until all other threads are finished before exitting
    {

    }
    pthread_exit(0);
}


/**
 * The consumer threads, takes items one at a time from the shared buffer. Waits when producer is pushing items
 * 
 * @param parm A pointer to the shared buffer array
 */
void * consumer(void * parm)
{
    auto consumerStartTime = std::chrono::high_resolution_clock::now();
    int conID = threadID;
    threadID++;

    string *localBuffer = (string *) parm;
    int completedTasks = 0;

    runningConsumerThreads++;

    string conCurrItem;
    while( true )
    {
        if( commandsRemaining <= 0 )
        {
            runningConsumerThreads--;
            printProgramRealTime( consumerStartTime, 1 );
            completedConsumerTasks[ conID ] = completedTasks;

            cout << "   ";
            printProgramRealTime( consumerStartTime, 0 );
            cout << " " << "ID= " << conID << "      " << "Ask" << endl;

            pthread_exit(0);
        }

        pthread_mutex_lock( &(buffer.mutex) );                   // LOCK

        cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << "      " << "Ask" << endl;

        numOfConsumerAsks++;

        while( buffer.numOfBufferItems <= 0 && commandsRemaining > 0 )      // While buffer empty, wait
        {
            pthread_cond_wait( &(buffer.more), &(buffer.mutex) );            
        }

        if( commandsRemaining <= 0 )                // Put the exit check again in case the number of commands changes since first check
        {
            runningConsumerThreads--;
            printProgramRealTime( consumerStartTime, 1 );
            completedConsumerTasks[ conID ] = completedTasks;

            cout << "   ";
            printProgramRealTime( consumerStartTime, 0 );
            cout << " " << "ID= " << conID << "      " << "Ask" << endl;

            pthread_exit(0);
        }

        conCurrItem = localBuffer[buffer.nextout++];
        int commandsWaitingInBuffer = 0;

        if( buffer.numOfBufferItems > 0 )
        {
            commandsWaitingInBuffer = buffer.numOfBufferItems - 1;      // Consumer just took one out, so there's 1 less buffer item
        } else {
            commandsWaitingInBuffer = buffer.numOfBufferItems;
        }

        cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );
        cout << " " << "ID= " << conID << " " << "Q= " << commandsWaitingInBuffer << " " << "Recieve    " << conCurrItem.substr( 1, conCurrItem.size() -1 ) << endl;

        buffer.nextout %= globalUserDefinedBufferSize;
        buffer.numOfBufferItems--;
        commandsRemaining--;

        pthread_cond_signal(&(buffer.less)); 
        pthread_mutex_unlock(&(buffer.mutex));                         // UNLOCK

        string intOfTCommand = conCurrItem.substr( 1, conCurrItem.size() - 1 );
        int transTime = stoi( intOfTCommand );
        Trans( transTime );

        cout << "   ";
        printProgramRealTime( consumerStartTime, 0 );       // for the completed
        cout << " " << "ID= " << conID << "      " << "Complete   " << conCurrItem.substr( 1, conCurrItem.size() - 1 ) << endl;

        completedTasks++;
    }
    runningConsumerThreads--;
    completedConsumerTasks[ conID ] = completedTasks;

    cout << "   ";
    printProgramRealTime( consumerStartTime, 0 );
    cout << " " << "ID= " << conID << "      " << "Ask" << endl;

    printProgramRealTime( consumerStartTime, 1 );                  // Consumer should never reach here because it should've
    pthread_exit(0);                                               // exitted before, but just in case
}

/*
the way this prod con system works is the prod will keep tryna put things into the buffer till either the buffer's full
or there's nothing left for it to push to the buffer - only then will the consumer start to work

idk if it's supposed to be like producer will put a thing, the second a thing is available the conumser
will start to try to take thing




sometimes with a lotta threads it'll work, but sometimes not. i have no idea why. like run ./prodcon 5 1 < inputs
might not work, put ./prodcon 5 1 < inputs in again it might work, might not, might, wth



i'm assuming the inputfile is with extension .txt, do i need to change it
so it auto pends .txt to file name?


my implementation of the time inludes the time it sleeps
*/


/**
 * Main function
 * 
 * @param argc Number of command line arguments
 * @param argv The actual command line arguments
 */
int main( int argc, char *argv[] ) 
{
    pthread_mutex_lock( &(buffer.mutex) );            // Lock while reading inputs just in case

    inputFileRedirection();

    pthread_mutex_unlock(&(buffer.mutex));            // Unlock

    string fileName;
    if( argc == 3 )
    {
        fileName = prodconLogFileToWriteTo( argv[2] );  // 3 arguments, ex "./prodcon 3 1 < inputexample"
    } else {
        fileName = prodconLogFileToWriteTo( "0" );      // Only 2 arguments, ex "./prodcon 3 < inputexample"
    }

    /**
     * https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
     * User "Nawaz" on how to do file output redirection
    */

    ofstream out( fileName );
    std::streambuf *coutbuf = std::cout.rdbuf();            //save old buf
    std::cout.rdbuf(out.rdbuf());                           //redirect std::cout to fileName

    int userDefinedBufferSize = stoi( argv[1] );

    const int finalSizeOfBuffer = userDefinedBufferSize * 2;        // Buffer size is twice the number of consumer threads
    globalUserDefinedBufferSize = finalSizeOfBuffer;

    string localNonGlobalBuffer[ finalSizeOfBuffer ];

    int i;

    pthread_cond_init( &(buffer.more), NULL );
    pthread_cond_init( &(buffer.less), NULL );
    if ( pthread_mutex_init( &buffer.mutex, NULL) != 0 )
    printf( "mutex init failed\n" );

    int numOfThreads = stoi(argv[1]); 
    pthread_t tid[numOfThreads];

    completedConsumerTasks.resize( numOfThreads );          // Double check size of "tasks completed by each thread" vector

    pthread_create(&tid[0], NULL, producer, localNonGlobalBuffer );          // Create producer thread
    
    for( int a = 0; a < numOfThreads; ++a )
    {
        pthread_create(&tid[a], NULL, consumer, localNonGlobalBuffer );          // Create consumer threads
    }
    
    for ( i = 0; i < numOfThreads; i++)
        pthread_join(tid[i], NULL);                         // Join all threads

    cout << "Summary: " << endl;
    cout << "    Work         " << numOfTCommands << endl;
    cout << "    Ask          " << numOfConsumerAsks << endl;
    cout << "    Recieve      " << numOfTCommands << endl;                 // If it reaches here in the program, all tasks were completed
    cout << "    Complete     " << numOfTCommands << endl;
    cout << "    Sleep        " << numOfTotalCommands - numOfTCommands << endl;

    for( int i = 1; i <= numOfThreads; ++i )
    {
        cout << "    Thread  " << i << "    " << completedConsumerTasks[i] << endl;;
    }

    cout << "Transactions per second: " << std::setprecision( 2 ) << std::fixed << numOfTCommands / totalTime << endl;

    producerThreadCanExit = true;
    std::cout.rdbuf(coutbuf); //reset to standard output again
    return 0;
}