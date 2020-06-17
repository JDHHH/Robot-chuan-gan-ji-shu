#ifndef __DHT11_H
#define __DHT11_H

void dht11_output_init(void);
void dht11_input_init(void);
void dht11_start(void);
int dht11_read_8bits(void);
void dht11_get_data(void);

#endif
