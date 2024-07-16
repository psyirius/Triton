//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the Apache License 2.0.
*/

#include <triton/pythonBindings.hpp>
#include <triton/pythonObjects.hpp>
#include <triton/pythonUtils.hpp>
#include <triton/pythonXFunctions.hpp>


/* setup doctest context

>>> from triton import ARCH, TritonContext, REG
>>> ctxt = TritonContext()
>>> ctxt.setArchitecture(ARCH.X86_64)

*/

/*! \page py_REG_page REG
    \brief [**python api**] All information about the REG Python namespace.

\tableofcontents

\section REG_py_description Description
<hr>

According to the CPU architecture, the REG namespace contains all enums of registers.
It's possible to create a \ref py_Register_page from a register id using `getRegister`
like this:

~~~~~~~~~~~~~{.py}
>>> ah = ctxt.getRegister(REG.X86_64.AH)
>>> print ah
ah:8 bv[15..8]

~~~~~~~~~~~~~

Note that creating a \ref py_TritonContext_page, you can directly access the constructed
\ref py_Register_page according to your defined architecture.

~~~~~~~~~~~~~{.py}
>>> ctxt.setArchitecture(ARCH.X86_64)
>>> print ctxt.registers.zmm1
zmm1:512 bv[511..0]

~~~~~~~~~~~~~

\section REG_py_api Python API - Items of the REG namespace
<hr>

\subsection REG_X86_py_api x86 registers

\htmlinclude x86_reg

\subsection REG_X8664_py_api x86-64 registers

\htmlinclude x8664_reg

\subsection REG_ARM32_py_api ARM32 registers

\htmlinclude arm32_reg

\subsection REG_AArch64_py_api AArch64 registers

\htmlinclude aarch64_reg

*/



namespace triton {
  namespace bindings {
    namespace python {

      void initRegNamespace(PyObject* registersDict) {
        PyDict_Clear(registersDict);

        // Create X86 REG namespace

        PyObject* x86RegistersDict = xPyDict_New();

        #define REG_SPEC(UPPER_NAME, _1, _2, _3, _4, _5, _6, _7, X86_AVAIL) \
          if (X86_AVAIL) \
            xPyDict_SetItemString(x86RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_X86_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #include "triton/x86.spec"

        PyObject* x86RegistersDictClass = xPyClass_New(nullptr, x86RegistersDict, xPyString_FromString("X86"));
        xPyDict_SetItemString(registersDict, "X86", x86RegistersDictClass);

        // Create X86_64 REG namespace

        PyObject* x8664RegistersDict = xPyDict_New();

        #define REG_SPEC(UPPER_NAME, _1, _2, _3, _4, _5, _6, _7, _8) \
          xPyDict_SetItemString(x8664RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_X86_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #include "triton/x86.spec"

        PyObject* x8664RegistersDictClass = xPyClass_New(nullptr, x8664RegistersDict, xPyString_FromString("X86_64"));
        xPyDict_SetItemString(registersDict, "X86_64", x8664RegistersDictClass);

        // Create AArch64 REG namespace

        PyObject* aarch64RegistersDict = xPyDict_New();

        #define REG_SPEC(UPPER_NAME, _1, _2, _3, _4, _5) \
          xPyDict_SetItemString(aarch64RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_AARCH64_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #define SYS_REG_SPEC REG_SPEC
        #include "triton/aarch64.spec"

        PyObject* aarch64RegistersDictClass = xPyClass_New(nullptr, aarch64RegistersDict, xPyString_FromString("AARCH64"));
        xPyDict_SetItemString(registersDict, "AARCH64", aarch64RegistersDictClass);

        // Create ARM32 REG namespace

        PyObject* arm32RegistersDict = xPyDict_New();

        #define REG_SPEC(UPPER_NAME, _1, _2, _3, _4, _5) \
          xPyDict_SetItemString(arm32RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_ARM32_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #include "triton/arm32.spec"

        PyObject* arm32RegistersDictClass = xPyClass_New(nullptr, arm32RegistersDict, xPyString_FromString("ARM32"));
        xPyDict_SetItemString(registersDict, "ARM32", arm32RegistersDictClass);

        #ifdef COMPILE_RISCV
        // Create RISCV64 REG namespace

        PyObject* riscv64RegistersDict = xPyDict_New();

        #define REG_SPEC(_0, UPPER_NAME, _1, _2, _3, _4, _5) \
          xPyDict_SetItemString(riscv64RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_RV64_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #define SYS_REG_SPEC REG_SPEC
        #include "triton/riscv64.spec"

        PyObject* riscv64RegistersDictClass = xPyClass_New(nullptr, riscv64RegistersDict, xPyString_FromString("RV64"));
        xPyDict_SetItemString(registersDict, "RV64", riscv64RegistersDictClass);

        // Create RISCV32 REG namespace

        PyObject* riscv32RegistersDict = xPyDict_New();

        #define REG_SPEC(_0, UPPER_NAME, _1, _2, _3, _4, _5) \
          xPyDict_SetItemString(riscv32RegistersDict, #UPPER_NAME, PyLong_FromUint32(triton::arch::ID_REG_RV32_##UPPER_NAME));
        // Use REG not available in capstone as normal register
        #define REG_SPEC_NO_CAPSTONE REG_SPEC
        #define SYS_REG_SPEC REG_SPEC
        #include "triton/riscv32.spec"

        PyObject* riscv32RegistersDictClass = xPyClass_New(nullptr, riscv32RegistersDict, xPyString_FromString("RV32"));
        xPyDict_SetItemString(registersDict, "RV32", riscv32RegistersDictClass);
        #endif
      }

    }; /* python namespace */
  }; /* bindings namespace */
}; /* triton namespace */
