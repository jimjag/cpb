cpb
====

The "Simple C Protocol Buffer (cpb)" provides fast encoding and decoding of Google Protocol Buffers in C and was based on the 'lwcp' C library released by Simon Kallweit.

Some distinctive things about cpb:

  * ANSI C-89 compatible

  * cpb does not require a codegen step.

  * cpb does not force OO on the programmer. Encoding and decoding works with plain old structures in C.

  * cpb is fast and small. The C library is 31kb stripped and [performs well](#performance) in benchmarks.

  * cpb does not depend on Google's C++ API or anything outside the Python standard libraries.

  * cpb supports [most features](#completeness) of the protocol buffer serialization format -- probably, all the ones you care about. :)

C library
---------

To compile:

    make

To test it:

    make check

Usage
-----

```
package test;

message Info { required int32 result = 1; required string msg = 2; }

message TestMessage { required int32 count = 1; required Info info = 2; }
```

**Encoder**

The following example will encode a simple message of the type 'TestMessage':

```
void encode_example(void) {

struct cpb_encoder encoder;
unsigned char buf[128];
size_t len;

// Initialize the encoder
cpb_encoder_init(&encoder);

// Start encoding a message of type 'test.TestMessage' into buf
cpb_encoder_start(&encoder, test_TestMessage, buf, sizeof(buf));

// Encode a 55 to the field 'count'
cpb_encoder_add_int32(&encoder, test_TestMessage_count, 55);

// Start encoding the nested message of type 'test.Info' in field 'info'
cpb_encoder_nested_start(&encoder, test_TestMessage_info);

// Encode a -1 to the field 'result'
cpb_encoder_add_int32(&encoder, test_Info_result, -1);

// Encode a "Unknown" to the field 'msg'
cpb_encoder_add_string(&encoder, test_Info_msg, "Unknown");

// Finish encoding the nested message of type 'test.Info'
cpb_encoder_nested_end(&encoder);

// Finish encoding the message of type 'test.TestMessage'
len = cpb_encoder_finish(&encoder);

// buf now holds the encoded message which is len bytes long
}
```

**Decoder**

The following example will decode a simple message of the type 'TestMessage':

```
// Structure to decode into struct TestMessage { int32 count; struct { int32 result; char msg[32]; } info; }

void msg_start_handler(struct cpb_decoder *decoder, const struct cpb_msg_desc *msg_desc, void *arg) { // We don't use the message start handler }

void msg_end_handler(struct cpb_decoder *decoder, const struct cpb_msg_desc *msg_desc, void *arg) { // We don't use the message end handler }

void field_handler(struct cpb_decoder *decoder, const struct cpb_msg_desc *msg_desc, const struct cpb_field_desc *field_desc, union cpb_value *value, void *arg) { struct TestMessage *msg = arg;

// Copy fields into local structure
if (msg_desc == test_TestMessage) {
    if (field_desc == test_TestMessage_count)
        msg->count = value->int32;
} else if (msg_desc == test_Info) {
    if (field_desc == test_Info_result)
        msg->info.result = value->int32;
    if (field_desc == test_Info_msg)
        strncpy(msg->info.msg, sizeof(msg->info.msg), value->string.str);
}
}

void decode_example(void) {

struct cpb_decoder decoder;
unsigned char buf[128];
size_t len;
struct TestMessage msg;

// Initialize the decoder
cpb_decoder_init(&decoder);

// Register a pointer to our local structure we want to decode into as
// the argument for the decoder event handlers
cpb_decoder_arg(&decoder, &msg);

// Register event handlers
cpb_decoder_msg_handler(&decoder, msg_start_handler, msg_end_handler);
cpb_decoder_field_handler(&decoder, field_handler);

// Decode the binary buffer from the encode example
cpb_decoder_decode(&decoder, test_TestMessage, buf, len, NULL);

// The local structure 'msg' will now hold the decoded values
}
```

Also review the test programs as well for usage hints.

Performance
-----------

From a [recent benchmark][fastpb-benchmark] which included the cpb module (lower times are better):

    JSON
    3.56521892548

    SimpleJSON 
    0.727998971939

    Protocol Buffer (fast)
    0.38397192955

    Protocol Buffer (standard)
    4.86640501022

    Protocol Buffer (cpb)
    0.323328971863

    cPickle
    0.811990976334

[fastpb-benchmark]: https://github.com/Greplin/fast-python-pb/tree/master/benchmark


Completeness
------------

Supported types (protobuf wire types 1, 2, and 5):

  * INT32
  * UINT32
  * INT64
  * UINT64
  * FIXED32
  * FIXED64
  * SFIXED32
  * SFIXED64
  * DOUBLE
  * FLOAT
  * BOOL
  * STRING
  * MESSAGE
  * BYTES
  * ENUM

That is to say, everything but GROUP, which Google has deprecated.

Supported options:

  * packed repeated fields

Not supported:

  * extensions
  * `required` fields are not enforced†
  * default values for optional fields in decoding†

†: by design

Maintainer
----------

  * Jim Jagielski

Previous authors (lwpb):

  * Simon Kallweit <simon.kallweit@intefo.ch>
  * Alan Grow <alangrow@gmail.com>
  * Nick Vatamaniuc <https://github.com/nickva>

