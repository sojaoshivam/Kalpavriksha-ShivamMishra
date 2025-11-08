#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Configuration Constants ---

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME_LENGTH 50
#define MAX_FILE_BLOCKS 100
#define MAX_INPUT_BUFFER 1024

// --- Data Structure Definitions ---

typedef enum {
    NODE_FILE,
    NODE_DIR
} NodeType;

typedef struct FileNode {
    char name[MAX_NAME_LENGTH];
    NodeType type;
    struct FileNode* parent;

    // Directory-Specific
    struct FileNode* childList;
    struct FileNode* nextSibling;
    struct FileNode* prevSibling;

    // File-Specific
    int contentSize;
    int blockPointers[MAX_FILE_BLOCKS];
} FileNode;

typedef struct FreeBlockNode {
    int blockIndex;
    struct FreeBlockNode* next;
    struct FreeBlockNode* prev;
} FreeBlockNode;


// --- Global System Variables ---

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
FreeBlockNode* freeBlockHead = NULL;
FreeBlockNode* freeBlockTail = NULL;
int numFreeBlocks = 0;
FileNode* root = NULL;
FileNode* cwd = NULL;


// --- Function Prototypes ---

void initVfs();
void freeVfs();
void freeTreeRecursive(FileNode* node);

void vfsMkdir(const char* name);
void vfsCreate(const char* name);
void vfsLs();
void vfsCd(const char* name);
void vfsRmdir(const char* name);
void vfsWrite(const char* filename, const char* content);
void vfsRead(const char* filename);
void vfsDelete(const char* filename);
void vfsPwd();
void vfsDf();

FileNode* createNode(const char* name, NodeType type, FileNode* parent);
void insertNode(FileNode* parent, FileNode* newNode);
void unlinkNode(FileNode* node);
FileNode* findNodeByName(FileNode* parent, const char* name);
void printPromptPath(FileNode* node);
void freeFileBlocks(FileNode* file);
int getFreeBlock();


// --- Main Program ---

int main() {
    char inputBuffer[MAX_INPUT_BUFFER];
    char* command;
    char* arg1;
    char* arg2;

    initVfs();
    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    while (1) {
        printPromptPath(cwd);
        printf(" > ");
        fflush(stdout);

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            break;
        }

        inputBuffer[strcspn(inputBuffer, "\n")] = 0;

        command = strtok(inputBuffer, " \t");
        if (command == NULL) {
            continue;
        }

        arg1 = strtok(NULL, " \t");

        // Special handling for 'write'
        if (strcmp(command, "write") == 0 && arg1 != NULL) {
            arg2 = strtok(NULL, "");
            if (arg2) {
                while (isspace((unsigned char)*arg2)) arg2++;
                if (*arg2 == '"' || *arg2 == '\'') {
                    arg2++;
                    char* end = arg2 + strlen(arg2) - 1;
                    if (*end == '"' || *end == '\'') {
                        *end = '\0';
                    }
                }
            }
        } else {
            arg2 = NULL;
        }

        // --- Command Execution ---
        if (strcmp(command, "exit") == 0) {
            break;
        } 
        
        else if (strcmp(command, "mkdir") == 0) {
            if (arg1) vfsMkdir(arg1); else printf("Usage: mkdir <dirname>\n");
        } 
        
        else if (strcmp(command, "create") == 0) {
            if (arg1) vfsCreate(arg1); else printf("Usage: create <filename>\n");
        } 
        
        else if (strcmp(command, "ls") == 0) {
            vfsLs();
        } 
        
        else if (strcmp(command, "cd") == 0) {
            if (arg1) vfsCd(arg1); else printf("Usage: cd <path>\n");
        } 
        
        else if (strcmp(command, "rmdir") == 0) {
            if (arg1) vfsRmdir(arg1); else printf("Usage: rmdir <dirname>\n");
        } 
        
        else if (strcmp(command, "write") == 0) {
            if (arg1 && arg2) vfsWrite(arg1, arg2); else printf("Usage: write <filename> \"content\"\n");
        } 
        
        else if (strcmp(command, "read") == 0) {
            if (arg1) vfsRead(arg1); else printf("Usage: read <filename>\n");
        } 
        
        else if (strcmp(command, "delete") == 0) {
            if (arg1) vfsDelete(arg1); else printf("Usage: delete <filename>\n");
        } 
        
        else if (strcmp(command, "pwd") == 0) {
            vfsPwd();
        } 
        
        else if (strcmp(command, "df") == 0) {
            vfsDf();
        } 
        
        else {
            printf("Unknown command: %s\n", command);
        }
    }

    freeVfs();
    printf("Memory released. Exiting program...\n");
    return 0;
}


// --- Initialization and Cleanup Implementations ---

 
void initVfs() {
    memset(virtualDisk, 0, sizeof(virtualDisk));

    freeBlockHead = NULL;
    freeBlockTail = NULL;
    numFreeBlocks = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        FreeBlockNode* newNode = (FreeBlockNode*)malloc(sizeof(FreeBlockNode));
        if (!newNode) {
            perror("Failed to allocate memory for free block list");
            exit(1);
        }
        newNode->blockIndex = i;
        newNode->next = NULL;
        newNode->prev = freeBlockTail;

        if (freeBlockTail == NULL) {
            freeBlockHead = newNode;
            freeBlockTail = newNode;
        } else {
            freeBlockTail->next = newNode;
            freeBlockTail = newNode;
        }
        numFreeBlocks++;
    }

    root = createNode("/", NODE_DIR, NULL);
    root->parent = root;

    cwd = root;
}

 
void freeVfs() {
    freeTreeRecursive(root);

    FreeBlockNode* current = freeBlockHead;
    while (current != NULL) {
        FreeBlockNode* temp = current;
        current = current->next;
        free(temp);
    }
}

 
void freeTreeRecursive(FileNode* node) {
    if (node == NULL) {
        return;
    }

    if (node->type == NODE_DIR && node->childList != NULL) {
        FileNode* child = node->childList;
        child->prevSibling->nextSibling = NULL; // Break the circle

        while (child != NULL) {
            FileNode* next = child->nextSibling;
            freeTreeRecursive(child);
            child = next;
        }
    }

    free(node);
}


// --- Command Handler Implementations ---

void vfsMkdir(const char* name) {
    if (findNodeByName(cwd, name)) {
        printf("Error: '%s' already exists.\n", name);
        return;
    }
    FileNode* newDir = createNode(name, NODE_DIR, cwd);
    insertNode(cwd, newDir);
    printf("Directory '%s' created successfully.\n", name);
}

void vfsCreate(const char* name) {
    if (findNodeByName(cwd, name)) {
        printf("Error: '%s' already exists.\n", name);
        return;
    }
    FileNode* newFile = createNode(name, NODE_FILE, cwd);
    insertNode(cwd, newFile);
    printf("File '%s' created successfully.\n", name);
}

void vfsLs() {
    if (cwd->childList == NULL) {
        printf("(empty)\n");
        return;
    }

    FileNode* current = cwd->childList;
    do {
        if (current->type == NODE_DIR) {
            printf("%s/ \n", current->name);
        } else {
            printf("%s \n", current->name);
        }
        current = current->nextSibling;
    } while (current != cwd->childList);
}

void vfsCd(const char* name) {
    if (strcmp(name, "/") == 0) {
        cwd = root;
        printf("Moved to /\n");
        return;
    }

    if (strcmp(name, "..") == 0) {
        cwd = cwd->parent;
        printf("Moved to parent directory.\n");
        return;
    }

    FileNode* target = findNodeByName(cwd, name);
    if (target == NULL) {
        printf("Error: Directory '%s' not found.\n", name);
    } else if (target->type == NODE_FILE) {
        printf("Error: '%s' is a file, not a directory.\n", name);
    } else {
        cwd = target;
        printf("Moved to /%s\n", name);
    }
}

void vfsRmdir(const char* name) {
    FileNode* target = findNodeByName(cwd, name);
    if (target == NULL) {
        printf("Error: Directory '%s' not found.\n", name);
        return;
    }
    if (target->type == NODE_FILE) {
        printf("Error: '%s' is a file.\n", name);
        return;
    }
    if (target->childList != NULL) {
        printf("Error: Directory '%s' is not empty.\n", name);
        return;
    }

    unlinkNode(target);
    free(target);
    printf("Directory removed successfully.\n");
}

void vfsWrite(const char* filename, const char* content) {
    FileNode* file = findNodeByName(cwd, filename);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }
    if (file->type == NODE_DIR) {
        printf("Error: '%s' is a directory.\n", filename);
        return;
    }

    freeFileBlocks(file);

    int dataSize = strlen(content);
    if (dataSize == 0) {
        file->contentSize = 0;
        printf("Data written successfully (file is now empty).\n");
        return;
    }

    int blocksNeeded = (dataSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocksNeeded > MAX_FILE_BLOCKS) {
        printf("Error: File size exceeds maximum of %d blocks.\n", MAX_FILE_BLOCKS);
        return;
    }
    if (blocksNeeded > numFreeBlocks) {
        printf("Error: Not enough free space on disk.\n");
        return;
    }

    file->contentSize = dataSize;
    const char* dataPtr = content;

    for (int i = 0; i < blocksNeeded; i++) {
        int blockIndex = getFreeBlock();
        if (blockIndex == -1) {
            printf("Error: Ran out of blocks unexpectedly.\n");
            file->contentSize = 0;
            freeFileBlocks(file);
            return;
        }

        file->blockPointers[i] = blockIndex;

        int chunkSize = (dataSize > BLOCK_SIZE) ? BLOCK_SIZE : dataSize;
        memcpy(virtualDisk[blockIndex], dataPtr, chunkSize);

        dataPtr += chunkSize;
        dataSize -= chunkSize;
    }

    printf("Data written successfully (size=%d bytes).\n", file->contentSize);
}

void vfsRead(const char* filename) {
    FileNode* file = findNodeByName(cwd, filename);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }
    if (file->type == NODE_DIR) {
        printf("Error: '%s' is a directory.\n", filename);
        return;
    }
    if (file->contentSize == 0) {
        printf("(empty file)\n");
        return;
    }

    int bytesLeft = file->contentSize;
    for (int i = 0; i < MAX_FILE_BLOCKS && file->blockPointers[i] != -1; i++) {
        if (bytesLeft == 0) break;

        int blockIndex = file->blockPointers[i];
        int bytesToRead = (bytesLeft > BLOCK_SIZE) ? BLOCK_SIZE : bytesLeft;

        printf("%.*s", bytesToRead, virtualDisk[blockIndex]);

        bytesLeft -= bytesToRead;
    }
    printf("\n");
}

void vfsDelete(const char* filename) {
    FileNode* file = findNodeByName(cwd, filename);
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }
    if (file->type == NODE_DIR) {
        printf("Error: '%s' is a directory. Use rmdir.\n", filename);
        return;
    }

    freeFileBlocks(file);
    unlinkNode(file);
    free(file);

    printf("File deleted successfully.\n");
}

void vfsPwd() {
    printPromptPath(cwd);
    printf("\n");
}

void vfsDf() {
    int total = NUM_BLOCKS;
    // Renamed 'free' to 'freeCount' to avoid conflict with stdlib function
    int freeCount = numFreeBlocks; 
    int used = total - freeCount;
    double usage = (total > 0) ? ((double)used / total * 100.0) : 0.0;

    printf("--- Disk Usage ---\n");
    printf("Total Blocks: %d\n", total);
    printf("Used Blocks:  %d\n", used);
    printf("Free Blocks:  %d\n", freeCount);
    printf("Disk Usage:   %.2f%%\n", usage);
}


// --- Helper Function Implementations ---

FileNode* createNode(const char* name, NodeType type, FileNode* parent) {
    FileNode* newNode = (FileNode*)malloc(sizeof(FileNode));
    if (!newNode) {
        perror("Failed to allocate memory for new node");
        exit(1);
    }
    
    strncpy(newNode->name, name, MAX_NAME_LENGTH - 1);
    newNode->name[MAX_NAME_LENGTH - 1] = '\0';
    newNode->type = type;
    newNode->parent = parent;

    if (type == NODE_DIR) {
        newNode->childList = NULL;
    } else {
        newNode->contentSize = 0;
        memset(newNode->blockPointers, -1, sizeof(newNode->blockPointers));
    }
    
    newNode->nextSibling = NULL;
    newNode->prevSibling = NULL;

    return newNode;
}

void insertNode(FileNode* parent, FileNode* newNode) {
    if (parent->childList == NULL) {
        parent->childList = newNode;
        newNode->nextSibling = newNode;
        newNode->prevSibling = newNode;
    } else {
        FileNode* tail = parent->childList->prevSibling;

        tail->nextSibling = newNode;
        newNode->prevSibling = tail;
        newNode->nextSibling = parent->childList;
        parent->childList->prevSibling = newNode;
    }
}

void unlinkNode(FileNode* node) {
    if (node->parent == NULL) return;

    FileNode* parent = node->parent;

    if (node->nextSibling == node) {
        parent->childList = NULL;
    } else {
        node->prevSibling->nextSibling = node->nextSibling;
        node->nextSibling->prevSibling = node->prevSibling;

        if (parent->childList == node) {
            parent->childList = node->nextSibling;
        }
    }

    node->nextSibling = NULL;
    node->prevSibling = NULL;
    node->parent = NULL;
}

FileNode* findNodeByName(FileNode* parent, const char* name) {
    if (parent->childList == NULL) {
        return NULL;
    }

    FileNode* current = parent->childList;
    do {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->nextSibling;
    } while (current != parent->childList);

    return NULL;
}

void printPromptPath(FileNode* node) {
    if (node == root) {
        printf("/");
        return;
    }
    
    printPromptPath(node->parent);
    
    if (node->parent != root) {
        printf("/");
    }
    printf("%s", node->name);
}

void freeFileBlocks(FileNode* file) {
    for (int i = 0; i < MAX_FILE_BLOCKS; i++) {
        int blockIndex = file->blockPointers[i];
        if (blockIndex == -1) {
            continue;
        }

        FreeBlockNode* newNode = (FreeBlockNode*)malloc(sizeof(FreeBlockNode));
        newNode->blockIndex = blockIndex;
        newNode->next = NULL;
        newNode->prev = freeBlockTail;

        if (freeBlockTail == NULL) {
            freeBlockHead = newNode;
            freeBlockTail = newNode;
        } else {
            freeBlockTail->next = newNode;
            freeBlockTail = newNode;
        }
        numFreeBlocks++;

        file->blockPointers[i] = -1;
    }
    file->contentSize = 0;
}

int getFreeBlock() {
    if (freeBlockHead == NULL) {
        return -1;
    }

    FreeBlockNode* blockNode = freeBlockHead;
    int blockIndex = blockNode->blockIndex;

    freeBlockHead = blockNode->next;
    if (freeBlockHead != NULL) {
        freeBlockHead->prev = NULL;
    } else {
        freeBlockTail = NULL;
    }

    free(blockNode);
    numFreeBlocks--;
    return blockIndex;
}