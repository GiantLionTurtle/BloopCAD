
#ifndef EXPUNGE_HPP_
#define EXPUNGE_HPP_

// definetly over the top but wont be confused with anything 
#define expunge(pointer) delete(pointer); pointer = nullptr;

#endif