#include <iostream>
#include <pthread.h>

using namespace std;

void *thredHelloWorld(void *threadId) {
    long id = (long)threadId;
    cout << "Executing Thread: " << id << endl;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];

    for (int i = 0; i < 5; ++i) {
        cout << "Inside main(). Creating thread " << i << endl;
        int rc = pthread_create(&threads[i], NULL, thredHelloWorld, (void *)i);
        
        if (rc) {
            cout << "Error" << endl;
            break;
        }
    }
    pthread_exit(NULL);
    return 0;
}