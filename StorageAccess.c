#include <stdio.h>
#include "swkbd.h"


Result StorageWrite(Handle srv, u64 offset, const uint8_t *data, size_t size) {
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
        u64 offset;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 10;
    raw->offset = offset;
    //ipcAddSendBuffer(&c, (void *) data, size, 0x21);
    ipcAddSendSmart(&c,0,(void *) data,size,0);
    Result rc = ipcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result StorageGetSize(Handle srv, u64* size) {
    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 0;

    Result rc = ipcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            u64 size;
        } *resp = r.Raw;
        *size = resp->size;
        rc = resp->result;
    }

    return rc;
}

