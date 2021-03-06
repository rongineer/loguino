/* Copyright 2014 David Irvine
 *
 * This file is part of Loguino
 *
 * Loguino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Loguino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Loguino.  If not, see "http://www.gnu.org/licenses/".
 *
*/


#ifdef ENABLE_DEBUG
  #ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 1
  #endif

  #if DEBUG_LEVEL > 0
    #define DEBUG_1(m) debug(__FILE__,__FUNCTION__, __LINE__, m)
  #else
    #define DEBUG_1(m)
  #endif

  #if DEBUG_LEVEL > 1
    #define DEBUG_2(m) debug(__FILE__,__FUNCTION__, __LINE__, m)
  #else
    #define DEBUG_2(m)
  #endif

  #if DEBUG_LEVEL > 2
    #define DEBUG_3(m) debug(__FILE__,__FUNCTION__, __LINE__, m)
  #else
    #define DEBUG_3(m)
  #endif

  #if DEBUG_LEVEL > 3
    #define DEBUG_4(m) debug(__FILE__,__FUNCTION__, __LINE__, m)
  #else
    #define DEBUG_4(m)
  #endif

  #if DEBUG_LEVEL > 4
    #define DEBUG_5(m) debug(__FILE__,__FUNCTION__, __LINE__, m)
  #else
    #define DEBUG_5(m)
  #endif

#else
  #define DEBUG_1(m)
  #define DEBUG_2(m)
  #define DEBUG_3(m)
  #define DEBUG_4(m)
  #define DEBUG_5(m)
#endif




extern unsigned int __heap_start;
extern void *__brkval;

/*
 *  * The free list structure as maintained by the
 *   * avr-libc memory allocation routines.
 *    */
struct __freelist {
	size_t sz;
	struct __freelist *nx;
};

/* The head of the free list structure */
extern struct __freelist *__flp;

int freeListSize() {
	struct __freelist* current;
	int total = 0;

	for (current = __flp; current; current = current->nx) {
		total += 2; /* Add two bytes for the memory block's header  */
		total += (int) current->sz;
	}

	return total;
}

int freeMemory() {
	int free_memory;

	if ((int)__brkval == 0) {
		free_memory = ((int)&free_memory) - ((int)&__heap_start);
	} else {
		free_memory = ((int)&free_memory) - ((int)__brkval);
		free_memory += freeListSize();
	}
	return free_memory;
}



void debug(const char * fname, const char * func, const int lnum, const  char * message){
	String s;
	char txt[20];
	sprintf(txt, "%9lu", millis() );
	s="#";
	s+= txt;
	s+= ", ";
	s+= fname;
	s+= ", ";
	s+= func;
	s+= ", ";
	s+= lnum;
	s+= ", ";
	s+= message;
	DEBUG_SERIAL_DEV.println(s);
}

void setup(){
	DEBUG_1("Begin");
	#ifdef ENABLE_DEBUG
		#ifdef DEBUG_SERIAL_DEV
			#ifdef DEBUG_SERIAL_BAUD
		        DEBUG_SERIAL_DEV.begin(DEBUG_SERIAL_BAUD);
		        while(!DEBUG_SERIAL_DEV){delay(1);}
			#endif
		#endif
	#endif
	DEBUG_5("Setting up Pollers");
	setupPollers();
	DEBUG_2("Succesfully setup Pollers");
	DEBUG_5("Setting up Outputs");
	setupLoggers();
	DEBUG_2("Succesfully setup Outputs");
	DEBUG_1("Finished");
}

unsigned long loopTime;

void loop(){
    loopTime=millis()+MIN_CYCLE_TIME;
	DEBUG_1("Begin");
	DEBUG_5("Reading Sensors");
	readSensors();
	DEBUG_2("Successfully read sensors");
	DEBUG_5("Flushing Loggers");
	flushLoggers();
	DEBUG_2("Successfully flushed loggers");
	while(millis() <= loopTime){;}
	DEBUG_1("Finished");
}

void logMessage(const char* name, bool value, const char* unit){
    DEBUG_1("Begin");
    if (value){
        logMessage(name, "True", unit);
    }else{
        logMessage(name, "False", unit);
    }
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}


void logMessage(const char* name, float value, const char* unit){
    DEBUG_1("Begin");
    char buf[33];
    dtostrf(value, 1, 2, buf);
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}

void logMessage(const char* name, unsigned long value, const char* unit){
    DEBUG_1("Begin");
    char buf[100];
    sprintf (buf, "%lu", value);
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}


void logMessage(const char* name, long value, const char* unit){
    DEBUG_1("Begin");
    char buf[100];
    sprintf (buf, "%ld", value);
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}

void logMessage(const char* name, int value, const char* unit){
    DEBUG_1("Begin");
    char buf[33];
    sprintf (buf, "%i", value);
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}

void logMessage(const char* name, unsigned int value, const char* unit){
    DEBUG_1("Begin");
    char buf[33];
    sprintf (buf, "%u", value);
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    DEBUG_1("Finished");
}

void logMessage(const char * name, String value, const char * unit){
    DEBUG_1("Begin");
    char *buf;
    buf = NULL;

    int size;
    size = value.length() + 1;
    DEBUG_3("Allocating");
    buf = (char *) malloc(size * sizeof(char));
    DEBUG_5("Logging");
    logMessage(name, buf, unit);
    DEBUG_4("Logged");
    free (buf);
    DEBUG_5("Freed");
    DEBUG_1("Finished");
}