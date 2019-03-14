cpb
====

The "Simple C Protocol Buffer (cpb)" provides fast encoding and decoding of Google Protocol Buffers in C and was based on the 'lwcp' C library released by Simon Kallweit. More documentation on the C API can be found here: [ http://code.google.com/p/lwpb/ ](http://code.google.com/p/lwpb/).

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

<span id="performance"></span>

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

<span id="completeness"></span>

Completeness
------------

Supported types:

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

Maintainer:

  * Jim Jagielski

Previous authors (lwpb):

  * Simon Kallweit <simon.kallweit@intefo.ch>
  * Alan Grow <alangrow@gmail.com>
  * Nick Vatamaniuc <https://github.com/nickva>

