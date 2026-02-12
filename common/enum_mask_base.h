// Part of the MyLang compiler project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYLANG_COMMON_ENUM_MASK_BASE_H_
#define MYLANG_COMMON_ENUM_MASK_BASE_H_

#include <bit>

#include "common/enum_base.h"
#include "llvm/ADT/StringExtras.h"

namespace MyLang::Internal {

// CRTP-style base class similar to `EnumBase`, but supporting mask enums.
// Enumerator values are consecutive bit shifts (1 << 0, 1 << 1, 1 << 2, 1 << 3,
// ...).
//
// Users must be in the `MyLang` namespace and should look like the following.
//
// In `my_kind.h`:
//   ```
//   #define MYLANG_MY_KIND(X) \
//       X(Enumerator1)        \
//       X(Enumerator2)        \
//       X(Enumerator3)        \
//       ...
//
//   MYLANG_DEFINE_RAW_ENUM_MASK(MyKind, uint32_t) {
//     MYLANG_MY_KIND(MYLANG_RAW_ENUM_MASK_ENUMERATOR)
//   };
//
//   class MyKind : public MYLANG_ENUM_MASK_BASE(MyKind) {
//    public:
//     MYLANG_MY_KIND(MYLANG_ENUM_MASK_CONSTANT_DECL)
//
//     // Plus, anything else you wish to include.
//   };
//
//   #define MYLANG_MY_KIND_WITH_TYPE(X) \
//     MYLANG_ENUM_MASK_CONSTANT_DEFINITION(MyKind, X)
//   MYLANG_MY_KIND(MYLANG_MY_KIND_WITH_TYPE)
//   #undef MYLANG_MY_KIND_WITH_TYPE
//   ```
//
// In `my_kind.cpp`:
//   ```
//   MYLANG_DEFINE_ENUM_MASK_NAMES(MyKind) {
//     MYLANG_MY_KIND(MYLANG_ENUM_MASK_NAME_STRING)
//   };
//   ```
template <typename DerivedT, typename EnumT, const llvm::StringLiteral Names[]>
class EnumMaskBase : public EnumBase<DerivedT, EnumT, Names> {
 public:
  // Provide a standard `None`.
  //
  // This uses a `&` to trigger slightly different instantiation behaviors in
  // Clang. For context on why this is needed, see http://wg21.link/CWG2800.
  // NOLINTNEXTLINE(readability-identifier-naming)
  static const DerivedT& None;

  // Returns true if there's a non-empty set intersection.
  constexpr auto HasAnyOf(DerivedT other) const -> bool {
    return !(*this & other).empty();
  }

  // Adds entries to the mask.
  auto Add(DerivedT other) -> void { *this = *this | other; }

  // Removes entries from the mask.
  auto Remove(DerivedT other) -> void { *this = *this & ~other; }

  constexpr auto empty() const -> bool { return this->AsInt() == 0; }

  constexpr auto operator|(DerivedT other) const -> DerivedT {
    return DerivedT::FromInt(this->AsInt() | other.AsInt());
  }

  constexpr auto operator&(DerivedT other) const -> DerivedT {
    return DerivedT::FromInt(this->AsInt() & other.AsInt());
  }

  constexpr auto operator~() const -> DerivedT {
    return DerivedT::FromInt(~this->AsInt());
  }

  // Use `Print` for mask entries. This hides `EnumBase::name`; it's not
  // compatible with `EnumMaskBase`.
  auto name() const -> llvm::StringRef = delete;

  // Prints this value as a `|`-separated list of mask entries, or `None`.
  //
  // This shadows EnumBase::Print.
  auto Print(llvm::raw_ostream& out) const -> void {
    int value = this->AsInt();
    if (value == 0) {
      out << "None";
      return;
    }
    llvm::ListSeparator sep("|");
    for (int bit = 0; value != 0; value >>= 1, ++bit) {
      if (value & 1) {
        out << sep << Names[bit];
      }
    }
  }
};

template <typename DerivedT, typename EnumT, const llvm::StringLiteral Names[]>
constexpr const DerivedT& EnumMaskBase<DerivedT, EnumT, Names>::None =
    DerivedT::FromInt(0);

}  // namespace MyLang::Internal

// Use this before defining a class that derives from `EnumMaskBase` to begin
// the definition of the raw `enum class`. It should be followed by the body of
// that raw enum class.
#define MYLANG_DEFINE_RAW_ENUM_MASK(EnumMaskName, UnderlyingType)              \
  namespace Internal {                                                         \
  struct EnumMaskName##Data {                                                  \
    static const llvm::StringLiteral Names[];                                  \
    /* For bit shifts, track the initial counter value. This will increment on \
     * each enum entry. */                                                     \
    static constexpr uint64_t BitShiftCounter = __COUNTER__ + 1;               \
    enum class RawEnum : UnderlyingType;                                       \
  };                                                                           \
  }                                                                            \
  enum class Internal::EnumMaskName##Data::RawEnum : UnderlyingType

// In the `MYLANG_DEFINE_RAW_ENUM_MASK` block, use this to generate each
// enumerator.
#define MYLANG_RAW_ENUM_MASK_ENUMERATOR(Name) \
  Name = 1 << (__COUNTER__ - BitShiftCounter),

// Use this to compute the `Internal::EnumMaskBase` specialization for a MyLang
// enum mask. It both computes the name of the raw enum and ensures all the
// namespaces are correct.
#define MYLANG_ENUM_MASK_BASE(EnumMaskName)                               \
  ::MyLang::Internal::EnumMaskBase<EnumMaskName,                          \
                                   Internal::EnumMaskName##Data::RawEnum, \
                                   Internal::EnumMaskName##Data::Names>

// Constants and names are declared equivalently as to `EnumBase`.
#define MYLANG_ENUM_MASK_CONSTANT_DECL(Name) MYLANG_ENUM_CONSTANT_DECL(Name)
#define MYLANG_ENUM_MASK_CONSTANT_DEFINITION(EnumMaskName, Name) \
  MYLANG_ENUM_CONSTANT_DEFINITION(EnumMaskName, Name)
#define MYLANG_DEFINE_ENUM_MASK_NAMES(EnumMaskName) \
  MYLANG_DEFINE_ENUM_CLASS_NAMES(EnumMaskName)
#define MYLANG_ENUM_MASK_NAME_STRING(Name) MYLANG_ENUM_CLASS_NAME_STRING(Name)

#endif  // MYLANG_COMMON_ENUM_MASK_BASE_H_
