#include "mesh.h"
//============================================================================//
//                                                                            //
// Memorypool                                                                 //
//                                                                            //
// A structure for memory allocation.                                         //
//                                                                            //
// firstblock is the first block of items. nowblock is the block from which   //
//   items are currently being allocated. nextitem points to the next slab    //
//   of free memory for an item. deaditemstack is the head of a linked list   //
//   (stack) of deallocated items that can be recycled.  unallocateditems is  //
//   the number of items that remain to be allocated from nowblock.           //
//                                                                            //
// Traversal is the process of walking through the entire list of items, and  //
//   is separate from allocation.  Note that a traversal will visit items on  //
//   the "deaditemstack" stack as well as live items.  pathblock points to    //
//   the block currently being traversed.  pathitem points to the next item   //
//   to be traversed.  pathitemsleft is the number of items that remain to    //
//   be traversed in pathblock.                                               //
//                                                                            //
//============================================================================//
template<typename T>
class SimpleMemoryPool {
private:
    struct Block {
        Block* next;
        char data[1];//placeholder

        Block(size_t block_size):next(nullptr){}
    };

    union FreeNode {
        FreeNode* next;
        char data[1];//placeholder
    };

    // member variable
    Block* firstblock;
    Block* nowblock;
    FreeNode* nextitem;
    FreeNode* deaditemstack;
    size_t itembytes;
    size_t itemsperblock;
    long items;
    int unallocateditems;
    //traverse
    Block* pathblock;
    FreeNode* pathitem;
    int pathitemleft;

public:
    SimpleMemoryPool(size_t bytecount,size_t itemcount)
        :firstblock(nullptr), nowblock(nullptr), nextitem(nullptr),deaditemstack(nullptr),
        itembytes(bytecount),itemsperblock(itemcount),items(0),unallocateditems(0),
        pathblock(nullptr),pathitem(nullptr),pathitemleft(0){
        if (itembytes < sizeof(FreeNode*)) {
            itembytes = sizeof(FreeNode*);
        }
    }

    ~SimpleMemoryPool() {
        clear();
    }

    void* alloc() {
        // First check the linked list of dead items.  If the list is not 
        // empty, allocate an item from the list rather than a fresh one.
        if (deaditemstack) {
            FreeNode* freeitem = deaditemstack;
            deaditemstack = freeitem->next;
            items++;
            return freeitem;
        }

        // Check if there are any free items left in the current block.
        if (unallocateditems == 0) {
            size_t block_size = sizeof(Block) + itemsperblock + itembytes;
            // Allocate a new block of items, pointed to by the previous block.
            Block* newblock = (Block*)malloc(block_size);
            if (newblock == nullptr) {
                std::cerr << "Error:Failed to allocate new block!" << std::endl;
                return nullptr;
            }

            nowblock == newblock;
            nextitem = (FreeNode*)newblock->data;
            unallocateditems = itemsperblock;
        }

        // Get an item from current block
        void* newitem = nextitem;
        nextitem = (FreeNode*)((char*)nextitem + itembytes);
        unallocateditems--;
        items++;
        return newitem;
    }

    void dealloc(void *item) {
        if (item == nullptr)
            return;
        
        FreeNode* freeitem = (FreeNode*)item;
        freeitem->next = deaditemstack;
        deaditemstack = freeitem;
        items--;
    }

    // free all memory in memory pool
    void clear() {
        Block* current = firstblock;
        while (current != nullptr) {
            Block* next = current->next;
            free(current);
            current = next;
        }

        // reset all member variable
        firstblock = nullptr;
        nowblock = nullptr;
        nextitem = nullptr;
        deaditemsack = nullptr;
        items = 0;
        unallocateditems = 0;
        pathblock = nullptr;
        pathitem = nullptr;
        pathitemsleft = 0;
    }

    //============================================================================//
    //                                                                            //
    // traversalinit()   Prepare to traverse the entire list of items.            //
    //                                                                            //
    // This routine is used in conjunction with traverse().                       //
    //                                                                            //
    //============================================================================//
    void traversalInit() {
        pathblock = firstblock;
        if (pathblock != nullptr) {
            pathitem = (FreeNode*)pathblock->data;
            pathitemsleft = itemsperblock;
        }
        else {
            pathitem = nullptr;
            pathitemsleft = 0;
        }
    }

    //============================================================================//
    //                                                                            //
    // traverse()   Find the next item in the list.                               //
    //                                                                            //
    // This routine is used in conjunction with traversalinit().  Be forewarned   //
    // that this routine successively returns all items in the list, including    //
    // deallocated ones on the deaditemqueue. It's up to you to figure out which  //
    // ones are actually dead.  It can usually be done more space-efficiently by  //
    // a routine that knows something about the structure of the item.            //
    //                                                                            //
    //============================================================================//
    void* traverse() {
        if (pathitem == nullptr) {
            return nullptr;
        }

        if (pathitemsleft == 0) {
            pathblock = pathblock->next;
            if (pathblock == nullptr) {
                pathitem = nullptr;
                return nullptr;
            }
            pathitem = (FreeNode*)pathblock->data;
            pathitemsleft = itemsperblock;
        }

        void* current = pathitem;
        pathitem = (FreeNode*)((char*)pathitem + itembytes);
        pathitemsleft--;

        return current;
    }
};

template<typename T, typename... Args>
class ObjectPool {
private:
    SimpleMemoryPool pool;
public:
    ObjectPool(size_t itemsperblock = 1024)
        :pool(sizeof(T), itemsperblock) {
    }

    // allocate and construct(placement new)
    template<typename... Args>
    T* construct(Args&&... args) {
        static_assert(sizeof(T) <= itembytes,
            "Type size exceeds pool item size");

        T* obj = (T*)alloc();
        if (obj != nullptr) {
            new (obj) T(std::forward<Args>(args)...);
        }
        return obj;
    }

    // dallocate and deconstruct
    template<typename T>
    void destroy(T* item) {
        if (item == nullptr) return;

        item->~T();

        dealloc(item);
    }
};
