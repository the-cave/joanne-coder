# Joanne Coder

## What is it?

Joanne Coder is a cryptography agnostic [rolling code](https://en.wikipedia.org/wiki/Rolling_code) implementation.
It is also platform agnostic, hardware agnostic, and will run anywhere as long as C language works.
It is here to stay to the end of the world.

## What is a rolling code?

A remote controller can be implemented in many ways; using a fixed code is one way to do it. The issue with a fixed code is that somebody can eavesdrop the signal and recreate them later, effectively duplicating the controller.  

A way to prevent that is to change the code every time it has been used. And the method to do that is called the "rolling code".

This library implements the receiver side of the rolling code.

## Agnostic implementation

This library is not just agnostic to hardware; it also agnostic to the transmission layer. Whether you use UDP on an ESP8266, a web browser on your iPhone, a signal from an infrared LED, this library made no assumption on that, effectively make it a timeless library lasting longer than any iPhone or ESP8266.

## What about the security and encryption?

This library made no assumption on the security you choose too. It will work with any block cipher like the [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) or any authenticated encryption at your hand.

The only caveat here is that it does not work solely on its own and depends on an external encryption scheme to function. However, if you look for a ready-made solution optimized for microcontrollers, look no further than [Embeddable SPARX](https://github.com/the-cave/embeddable-sparx) by the same author of this library.

## Porting?
No need; the code is hardware and platform agnostic. It will run everywhere as long as C language works. You can even [run the test](#Testing) on your PC.

However, since the rolling counter used is 32-bit, a 32-bit processor like [Arm Cortex-M0](https://www.arm.com/products/silicon-ip-cpu/cortex-m/cortex-m0) is recommended.

## Usages

See [the test](./test/main.c) for an example.  

There are only 3 simple APIs.
* joanne_coder__init(&config, &state, &tracking_state);
* joanne_coder__push_command(&config, &state, slot, access_code, command);
* joanne_coder__sync(&config, &state, slot, access_code);

Autoinit mode is possible; simply supply `NULL` pointer as a `tracking_state` to the init function.

Most of other usage details can be obtained from [the test](./test/main.c).

## Prerequisites

* [GNU Compiler Collection](https://gcc.gnu.org/)
* [GNU Make](https://www.gnu.org/software/make/)

## Testing

Just clone the repository with submodules and `make run`.

## What is in the name?

[It is a pun.](https://en.wikipedia.org/wiki/Joanne_Rowling)

## License

Joanne Coder is released under the [BSD 3-Clause License](LICENSE.md). :tada:
