#include "swkbd.h"
#include <switch.h>
#include <stdio.h>

#define KeyboardAppletID 0x11
#define KeyboardAppletAttribute 0
#define DefaultStorage 0x20


Result GetAppletProxy(Service *srv, Handle *handle_out) {

    IpcCommand c;
    ipcInitialize(&c);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));

    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 200;
    ipcSendHandleCopy(&c, 0xffff8001);
    Result rc = serviceIpcDispatch(srv);

    if (R_SUCCEEDED(rc)) {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;

        if (R_SUCCEEDED(rc)) {
            *handle_out = r.Handles[0];
        }
    }

    return rc;
}

void ShowKeyboard(Handle KeyboardAccess, Handle AppletCreatorHandle, int preset) {
    Result rc;
    printf("GotKeyboard\n");
    printf("%i", KeyboardAccess);
    printf("\n");
    Handle StorageHandle;
    rc = CreateStorage(AppletCreatorHandle, DefaultStorage, &StorageHandle);
    if (R_SUCCEEDED(rc)) {
        printf("GotStorage\n");
        printf("%i", StorageHandle);
        printf("\n");
        Handle StorageAccess;

        rc = OpenStorage(StorageHandle, &StorageAccess);
        if (R_SUCCEEDED(rc)) {
            printf("Storage Opened\n");
            printf("%i", StorageAccess);
            printf("\n");
            u32 common_args_header[] = {0x1, 0x20}; // {version?, size?}
            rc = StorageWrite(StorageAccess, 0, (uint8_t *) common_args_header, sizeof(common_args_header));
            printf("Writen\n");

            if (R_SUCCEEDED(rc)) {
                struct {
                    u32 unknown1;
                    u32 theme_color;
                    u8 unknown2;
                    u64 system_tick;
                } common_args_data = {0};

                static_assert(sizeof(common_args_data) == 0x18, "invalid common args data");
                common_args_data.unknown1 = 5;
                common_args_data.theme_color = 0;
                common_args_data.unknown2 = 0;
                common_args_data.system_tick = svcGetSystemTick();
                rc = StorageWrite(StorageAccess, sizeof(common_args_header), (uint8_t *) &common_args_data,
                                  sizeof(common_args_data));
                printf("Writen\n");
                u32 StorageDomain;
                ipcConvertSessionToDomain(StorageHandle, &StorageDomain);
                if (R_SUCCEEDED(rc)) {
                    Handle PushedData;
                    PushInData(KeyboardAccess, StorageDomain, &PushedData);
                    printf("Pushed\n");
                }
            }
        }
    }
}

void Initkeyboard() {
    Result rc;
    static Service appletAE;
    rc = smInitialize();
    if (R_SUCCEEDED(rc)) {
        rc = smGetService(&appletAE, "appletAE");
        if (R_SUCCEEDED(rc)) {
            Handle AppletProxyHandle;
            rc = GetAppletProxy(&appletAE, &AppletProxyHandle);
            if (R_SUCCEEDED(rc)) {
                Handle AppletCreatorHandle;
                rc = GetIAppletCreator(AppletProxyHandle, &AppletCreatorHandle);
                if (R_SUCCEEDED(rc)) {
                    Handle KeyboardAccess;
                    rc = CreateApplet(AppletCreatorHandle, KeyboardAppletID, KeyboardAppletAttribute, &KeyboardAccess);
                    if (R_SUCCEEDED(rc)) {
                        ShowKeyboard(KeyboardAccess, AppletCreatorHandle, 4);
                    }
                }
            }
        }
    }
}
