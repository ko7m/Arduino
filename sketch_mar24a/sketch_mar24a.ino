#include "test.h"

void setup() {

    myClass foo;
    
    foo.print('a');
    foo.print("Hello world!");
    foo.print('a', HEX);
    foo.println();
    foo.printf("Hello %s\n", "world");

}

void loop() {


}
