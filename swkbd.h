#include <switch.h>

Result GetAppletProxy(Service *srv, Handle *handle_out);

Result GetIAppletCreator(Handle srv, Handle *handle_out);

Result CreateApplet(Handle CreatorHandle, u32 applet_ID, u32 appletAttribute, Handle *handle_out);

Result CreateStorage(Handle appletCreatorHandle, size_t size, Handle *handle_out);

Result OpenStorage(Handle StorageHandle, Handle *handle_out);

Result PushInData(Handle srv, Handle Storage, Handle *handle_out);

Result StorageWrite(Handle srv, u64 offset, const uint8_t *data, size_t size);

Result StorageGetSize(Handle srv, u64* size);


void Initkeyboard();
