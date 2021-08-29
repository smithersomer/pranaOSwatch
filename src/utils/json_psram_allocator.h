#ifndef SPI_RAM_JSON_DOC_H_
#define SPI_RAM_JSON_DOC_H_

struct SpiRamAllocator {
    void* allocate(size_t size) {
        void *ram = MALLOC( size );
        if ( ram ) {
            return( ram );
        }
        else {
            log_e("allocate %d bytes (%p) json psram failed", size, ram );
            if ( size == 0 ) {
                log_e("allocate zero bytes? really? abort");
            }
            return( ram );
        }
    }
    void deallocate(void* pointer) {
        free(pointer);
    }
};

using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

#endif
