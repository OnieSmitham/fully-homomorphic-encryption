// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <ios>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "transpiler/tests/openfhe_test_util.h"
#include "xls/common/status/matchers.h"

namespace fully_homomorphic_encryption {
namespace transpiler {
namespace {

using ::fully_homomorphic_encryption::transpiler::TranspilerTestBase;

template <typename InputType, typename OutputType>
struct TranspilerControlStructureTestCase {
  const std::string test_name;
  const InputType input;
  const OutputType expected_output;
};
using TranspilerControlStructureSwitchTestCase =
    TranspilerControlStructureTestCase<char, int>;

class OpenFheArrayOfStructsTest : public TranspilerTestBase {};

TEST_F(OpenFheArrayOfStructsTest, DynamicOneDimArrayAssignViaRef) {
  OpenFheArray<unsigned char> c_dyn_one_dim(2, cc());
  OpenFheArrayRef<unsigned char> c_dyn_one_dim_ref = c_dyn_one_dim;

  OpenFhe<unsigned char> a(cc());
  a.SetEncrypted('a', sk());
  XLS_CHECK_EQ(a.Decrypt(sk()), 'a');
  OpenFheRef<unsigned char> a_ref = a;
  XLS_CHECK_EQ(a_ref.Decrypt(sk()), 'a');

  OpenFhe<unsigned char> b(cc());
  b.SetEncrypted('b', sk());
  XLS_CHECK_EQ(b.Decrypt(sk()), 'b');
  OpenFheRef<unsigned char> b_ref = b;
  XLS_CHECK_EQ(b_ref.Decrypt(sk()), 'b');

  c_dyn_one_dim_ref[0] = a;
  c_dyn_one_dim_ref[1] = b;
  auto decoded_via_ref = c_dyn_one_dim_ref.Decrypt(sk());
  XLS_CHECK_EQ(decoded_via_ref[0], 'a');
  XLS_CHECK_EQ(decoded_via_ref[1], 'b');
  auto decoded = c_dyn_one_dim.Decrypt(sk());
  XLS_CHECK_EQ(decoded[0], 'a');
  XLS_CHECK_EQ(decoded[1], 'b');

  XLS_CHECK_EQ(c_dyn_one_dim[0].Decrypt(sk()), 'a');
  XLS_CHECK_EQ(c_dyn_one_dim_ref[0].Decrypt(sk()), 'a');
  XLS_CHECK_EQ(c_dyn_one_dim[1].Decrypt(sk()), 'b');
  XLS_CHECK_EQ(c_dyn_one_dim_ref[1].Decrypt(sk()), 'b');
}

TEST_F(OpenFheArrayOfStructsTest, FixedLenOneDimArrayAssignViaRef) {
  OpenFheArray<unsigned char, 2> c_fixed_len_one_dim(cc());
  OpenFheArrayRef<unsigned char, 2> c_fixed_len_one_dim_ref =
      c_fixed_len_one_dim;

  OpenFhe<unsigned char> a(cc());
  a.SetEncrypted('a', sk());
  XLS_CHECK_EQ(a.Decrypt(sk()), 'a');
  OpenFheRef<unsigned char> a_ref = a;
  XLS_CHECK_EQ(a_ref.Decrypt(sk()), 'a');

  OpenFhe<unsigned char> b(cc());
  b.SetEncrypted('b', sk());
  XLS_CHECK_EQ(b.Decrypt(sk()), 'b');
  OpenFheRef<unsigned char> b_ref = b;
  XLS_CHECK_EQ(b_ref.Decrypt(sk()), 'b');

  c_fixed_len_one_dim_ref[0] = a;
  c_fixed_len_one_dim_ref[1] = b;
  auto decoded_via_ref = c_fixed_len_one_dim_ref.Decrypt(sk());
  XLS_CHECK_EQ(decoded_via_ref[0], 'a');
  XLS_CHECK_EQ(decoded_via_ref[1], 'b');
  auto decoded = c_fixed_len_one_dim.Decrypt(sk());
  XLS_CHECK_EQ(decoded[0], 'a');
  XLS_CHECK_EQ(decoded[1], 'b');

  XLS_CHECK_EQ(c_fixed_len_one_dim[0].Decrypt(sk()), 'a');
  XLS_CHECK_EQ(c_fixed_len_one_dim_ref[0].Decrypt(sk()), 'a');
  XLS_CHECK_EQ(c_fixed_len_one_dim[1].Decrypt(sk()), 'b');
  XLS_CHECK_EQ(c_fixed_len_one_dim_ref[1].Decrypt(sk()), 'b');
}

TEST_F(OpenFheArrayOfStructsTest, FixedLenTwoDimArrayAssignViaRef) {
  int i_2x2[2][2] = {{12, 34}, {56, 78}};
  OpenFheArray<int, 2, 2> i_fixed_len_two_dim(cc());
  OpenFheArrayRef<int, 2, 2> i_fixed_len_two_dim_ref = i_fixed_len_two_dim;
  i_fixed_len_two_dim.SetEncrypted(i_2x2, sk());

  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[0], 12);
  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[1], 34);
  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[0], 56);
  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[1], 78);

  int new_row_0[2] = {21, 43}, new_row_1[2] = {65, 87};
  OpenFheArray<int, 2> encoded_new_row_0(cc()), encoded_new_row_1(cc());
  encoded_new_row_0.SetEncrypted(new_row_0, sk());
  encoded_new_row_1.SetEncrypted(new_row_1, sk());

  i_fixed_len_two_dim_ref[0] = encoded_new_row_0;
  i_fixed_len_two_dim_ref[1] = encoded_new_row_1;

  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[0], 21);
  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[1], 43);
  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[0], 65);
  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[1], 87);

  i_fixed_len_two_dim[0] = encoded_new_row_1;
  i_fixed_len_two_dim[1] = encoded_new_row_0;

  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[0], 21);
  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[0], 65);
  XLS_CHECK_EQ(i_fixed_len_two_dim[0].Decrypt(sk())[1], 87);
  XLS_CHECK_EQ(i_fixed_len_two_dim[1].Decrypt(sk())[1], 43);
}

// cc(), sk()
TEST_F(OpenFheArrayOfStructsTest, DynamicOneDimArray) {
  unsigned char c_array[2] = {'a', 'b'};
  short s_array[2] = {0x1234, 0x5678};
  unsigned i_array[2] = {0x789abcde, 0xc0deba7e};

  // Dynamic arrays with 2 elements.
  OpenFheArray<unsigned char> c_dyn_one_dim(2, cc());
  EXPECT_EQ(c_dyn_one_dim.length(), 2);
  EXPECT_EQ(c_dyn_one_dim.bit_width(), 2 * sizeof(unsigned char) * 8);
  EXPECT_EQ(c_dyn_one_dim.get().size(), c_dyn_one_dim.bit_width());
  c_dyn_one_dim.SetEncrypted(c_array, 2, sk());
  unsigned char c_decoded[2];
  c_dyn_one_dim.Decrypt(c_decoded, 2, sk());

  OpenFheArray<short> s_dyn_one_dim(2, cc());
  EXPECT_EQ(s_dyn_one_dim.length(), 2);
  EXPECT_EQ(s_dyn_one_dim.bit_width(), 2 * sizeof(short) * 8);
  EXPECT_EQ(s_dyn_one_dim.get().size(), s_dyn_one_dim.bit_width());
  s_dyn_one_dim.SetEncrypted(s_array, 2, sk());
  short s_decoded[2];
  s_dyn_one_dim.Decrypt(s_decoded, 2, sk());

  OpenFheArray<unsigned> i_dyn_one_dim(2, cc());
  EXPECT_EQ(i_dyn_one_dim.length(), 2);
  EXPECT_EQ(i_dyn_one_dim.bit_width(), 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(i_dyn_one_dim.get().size(), i_dyn_one_dim.bit_width());
  i_dyn_one_dim.SetEncrypted(i_array, 2, sk());
  unsigned i_decoded[2];
  i_dyn_one_dim.Decrypt(i_decoded, 2, sk());

  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(c_array[i], c_decoded[i]);
    EXPECT_EQ(s_array[i], s_decoded[i]);
    EXPECT_EQ(i_array[i], i_decoded[i]);
  }

  absl::FixedArray<unsigned char> c_another_decoded =
      c_dyn_one_dim.Decrypt(sk());
  absl::FixedArray<short> s_another_decoded = s_dyn_one_dim.Decrypt(sk());
  absl::FixedArray<unsigned> i_another_decoded = i_dyn_one_dim.Decrypt(sk());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(c_another_decoded[i], c_decoded[i]);
    EXPECT_EQ(s_another_decoded[i], s_decoded[i]);
    EXPECT_EQ(i_another_decoded[i], i_decoded[i]);
  }

  OpenFheRef<unsigned char> c_ref = c_dyn_one_dim[1];
  EXPECT_EQ(c_ref.length(), 1);
  EXPECT_EQ(c_ref.bit_width(), sizeof(unsigned char) * 8);
  EXPECT_EQ(c_ref.get().size(), c_ref.bit_width());

  OpenFheRef<short> s_ref = s_dyn_one_dim[1];
  EXPECT_EQ(s_ref.length(), 1);
  EXPECT_EQ(s_ref.bit_width(), sizeof(short) * 8);
  EXPECT_EQ(s_ref.get().size(), s_ref.bit_width());

  OpenFheRef<unsigned int> i_ref = i_dyn_one_dim[1];
  EXPECT_EQ(i_ref.length(), 1);
  EXPECT_EQ(i_ref.bit_width(), sizeof(unsigned) * 8);
  EXPECT_EQ(i_ref.get().size(), i_ref.bit_width());

  OpenFheRef<unsigned char> c_ref_ref = c_ref;
  EXPECT_EQ(c_ref_ref.length(), 1);
  EXPECT_EQ(c_ref_ref.bit_width(), sizeof(unsigned char) * 8);
  EXPECT_EQ(c_ref_ref.get().size(), c_ref_ref.bit_width());

  OpenFheRef<short> s_ref_ref = s_ref;
  EXPECT_EQ(s_ref_ref.length(), 1);
  EXPECT_EQ(s_ref_ref.bit_width(), sizeof(short) * 8);
  EXPECT_EQ(s_ref_ref.get().size(), s_ref_ref.bit_width());

  OpenFheRef<unsigned int> i_ref_ref = i_ref;
  EXPECT_EQ(i_ref_ref.length(), 1);
  EXPECT_EQ(i_ref_ref.bit_width(), sizeof(unsigned) * 8);
  EXPECT_EQ(i_ref_ref.get().size(), i_ref_ref.bit_width());

  unsigned char decoded_c_ref = c_ref.Decrypt(sk());
  short decoded_s_ref = s_ref.Decrypt(sk());
  unsigned decoded_i_ref = i_ref.Decrypt(sk());
  EXPECT_EQ(decoded_c_ref, c_decoded[1]);
  EXPECT_EQ(decoded_s_ref, s_decoded[1]);
  EXPECT_EQ(decoded_i_ref, i_decoded[1]);

  // Get a reference to the whole array.
  OpenFheArrayRef<unsigned char> c_dyn_one_dim_ref = c_dyn_one_dim;
  EXPECT_EQ(c_dyn_one_dim_ref.length(), 2);
  EXPECT_EQ(c_dyn_one_dim_ref.bit_width(), 2 * sizeof(unsigned char) * 8);
  EXPECT_EQ(c_dyn_one_dim_ref.get().size(), c_dyn_one_dim_ref.bit_width());

  OpenFheArrayRef<short> s_dyn_one_dim_ref = s_dyn_one_dim;
  EXPECT_EQ(s_dyn_one_dim_ref.length(), 2);
  EXPECT_EQ(s_dyn_one_dim_ref.bit_width(), 2 * sizeof(short) * 8);
  EXPECT_EQ(s_dyn_one_dim_ref.get().size(), s_dyn_one_dim_ref.bit_width());

  OpenFheArrayRef<unsigned> i_dyn_one_dim_ref = i_dyn_one_dim;
  EXPECT_EQ(i_dyn_one_dim_ref.length(), 2);
  EXPECT_EQ(i_dyn_one_dim_ref.bit_width(), 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(i_dyn_one_dim_ref.get().size(), i_dyn_one_dim_ref.bit_width());

  {
    // Check the array again via the reference.
    unsigned char c_decoded_via_ref[2];
    c_dyn_one_dim_ref.Decrypt(c_decoded_via_ref, 2, sk());
    short s_decoded_via_ref[2];
    s_dyn_one_dim_ref.Decrypt(s_decoded_via_ref, 2, sk());
    unsigned i_decoded_via_ref[2];
    i_dyn_one_dim_ref.Decrypt(i_decoded_via_ref, 2, sk());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(c_decoded_via_ref[i], c_decoded[i]);
      EXPECT_EQ(s_decoded_via_ref[i], s_decoded[i]);
      EXPECT_EQ(i_decoded_via_ref[i], i_decoded[i]);
    }

    absl::FixedArray<unsigned char> c_another_decoded_via_ref =
        c_dyn_one_dim_ref.Decrypt(sk());
    absl::FixedArray<short> s_another_decoded_via_ref =
        s_dyn_one_dim_ref.Decrypt(sk());
    absl::FixedArray<unsigned> i_another_decoded_via_ref =
        i_dyn_one_dim_ref.Decrypt(sk());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(c_decoded_via_ref[i], c_another_decoded_via_ref[i]);
      EXPECT_EQ(s_decoded_via_ref[i], s_another_decoded_via_ref[i]);
      EXPECT_EQ(i_decoded_via_ref[i], i_another_decoded_via_ref[i]);
    }

    OpenFheRef<unsigned char> c_ref_via_ref = c_dyn_one_dim_ref[1];
    EXPECT_EQ(c_ref_via_ref.length(), 1);
    EXPECT_EQ(c_ref_via_ref.bit_width(), sizeof(unsigned char) * 8);
    EXPECT_EQ(c_ref_via_ref.get().size(), c_ref_via_ref.bit_width());
    OpenFheRef<short> s_ref_via_ref = s_dyn_one_dim_ref[1];
    EXPECT_EQ(s_ref_via_ref.length(), 1);
    EXPECT_EQ(s_ref_via_ref.bit_width(), sizeof(short) * 8);
    EXPECT_EQ(s_ref_via_ref.get().size(), s_ref_via_ref.bit_width());
    OpenFheRef<unsigned int> i_ref_via_ref = i_dyn_one_dim_ref[1];
    EXPECT_EQ(i_ref_via_ref.length(), 1);
    EXPECT_EQ(i_ref_via_ref.bit_width(), sizeof(unsigned) * 8);
    EXPECT_EQ(i_ref_via_ref.get().size(), i_ref_via_ref.bit_width());

    unsigned char c_decoded_ref_via_ref = c_ref_via_ref.Decrypt(sk());
    short s_decoded_ref_via_ref = s_ref_via_ref.Decrypt(sk());
    unsigned i_decoded_ref_via_ref = i_ref_via_ref.Decrypt(sk());
    EXPECT_EQ(c_decoded_ref_via_ref, c_decoded[1]);
    EXPECT_EQ(s_decoded_ref_via_ref, s_decoded[1]);
    EXPECT_EQ(i_decoded_ref_via_ref, i_decoded[1]);
  }
}

TEST_F(OpenFheArrayOfStructsTest, FixedWidthOneDimArray) {
  short array[2] = {0x1234, 0x5678};

  // Static array with 2 elements.
  // OpenFheArray<Struct, 2> fixed_one_dim;
  OpenFheArray<short, 2> fixed_one_dim(cc());
  EXPECT_EQ(fixed_one_dim.length(), 2);
  EXPECT_EQ(fixed_one_dim.bit_width(), 2 * sizeof(short) * 8);

  fixed_one_dim.SetEncrypted(array, sk());

  short decoded[2];
  fixed_one_dim.Decrypt(decoded, sk());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(array[i], decoded[i]);
  }

  absl::FixedArray<short> another_decoded = fixed_one_dim.Decrypt(sk());
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(another_decoded[i], decoded[i]);
  }

  OpenFheRef<short> ref = fixed_one_dim[1];
  short decoded_ref = ref.Decrypt(sk());
  EXPECT_EQ(decoded_ref, decoded[1]);

  {
    // Get a reference to the whole array.
    OpenFheArrayRef<short, 2> fixed_one_dim_ref = fixed_one_dim;
    EXPECT_EQ(fixed_one_dim_ref.length(), 2);
    EXPECT_EQ(fixed_one_dim_ref.bit_width(), 2 * sizeof(short) * 8);

    // Check the array again via the reference.
    short decoded_via_ref[2];
    fixed_one_dim_ref.Decrypt(decoded_via_ref, sk());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i], decoded[i]);
    }

    absl::FixedArray<short> another_decoded_via_ref =
        fixed_one_dim_ref.Decrypt(sk());
    for (int i = 0; i < 2; i++) {
      EXPECT_EQ(decoded_via_ref[i], another_decoded_via_ref[i]);
    }
  }
}

TEST_F(OpenFheArrayOfStructsTest, FixedWidth4x3x2Array) {
  unsigned array[4][3][2] = {{{0x789abcde, 0xc0deba7e},
                              {0xbcde789a, 0xba7ec0de},
                              {0xde9abc78, 0x7edebac0}},

                             {{0x11223344, 0x55667788},
                              {0x33441122, 0x77885566},
                              {0x44223311, 0x7edebac0}},

                             {{0x99aabbcc, 0xddeeff00},
                              {0xbbcc99aa, 0xff00ddee},
                              {0xccaabb99, 0x00eeffdd}},

                             {{0x12345678, 0x9abcdef0},
                              {0x56781234, 0xdef09abc},
                              {0x78345612, 0xf0bcde9a}}};

  // Static array with 4x3x2 elements.
  OpenFheArray<unsigned, 4, 3, 2> fixed_4x3x2dim(cc());
  EXPECT_EQ(fixed_4x3x2dim.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim.bit_width(), 4 * 3 * 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(fixed_4x3x2dim.get().size(), fixed_4x3x2dim.bit_width());
  fixed_4x3x2dim.SetEncrypted(array, sk());
  unsigned decoded[4][3][2];
  fixed_4x3x2dim.Decrypt(decoded, sk());
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        EXPECT_EQ(array[i][j][k], decoded[i][j][k]);
      }
    }
  }

  // Get a reference to the whole array and check it.
  // OpenFheStructArrayRef<4, 3, 2> fixed_4x3x2dim_ref = fixed_4x3x2dim;
  OpenFheArrayRef<unsigned, 4, 3, 2> fixed_4x3x2dim_ref = fixed_4x3x2dim;
  EXPECT_EQ(fixed_4x3x2dim_ref.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim_ref.bit_width(), 4 * 3 * 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(fixed_4x3x2dim_ref.get().size(), fixed_4x3x2dim_ref.bit_width());
  unsigned decoded_via_ref[4][3][2];
  fixed_4x3x2dim_ref.Decrypt(decoded_via_ref, sk());
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 2; k++) {
        EXPECT_EQ(array[i][j][k], decoded_via_ref[i][j][k]);
      }
    }
  }

  // Obtain a copy of the whole-array reference
  OpenFheArrayRef<unsigned, 4, 3, 2> fixed_4x3x2dim_ref_ref =
      fixed_4x3x2dim_ref;
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.length(), 4);
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.bit_width(),
            4 * 3 * 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(fixed_4x3x2dim_ref_ref.get().size(),
            fixed_4x3x2dim_ref_ref.bit_width());

  // Obtain a subarray from the array by subscript and check it.
  // TODO: replace this with auto and static-assert the type
  OpenFheArrayRef<unsigned, 3, 2> fixed_3x2dim_ref = fixed_4x3x2dim[1];
  EXPECT_EQ(fixed_3x2dim_ref.length(), 3);
  unsigned subarray_3x2[3][2];
  fixed_3x2dim_ref.Decrypt(subarray_3x2, sk());
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 2; k++) {
      EXPECT_EQ(array[1][j][k], subarray_3x2[j][k]);
    }
  }

  // Obtain a subarray from the subarray via the reference, and check it.
  OpenFheArrayRef<unsigned, 2> fixed_2dim_ref = fixed_3x2dim_ref[2];
  EXPECT_EQ(fixed_2dim_ref.length(), 2);
  EXPECT_EQ(fixed_2dim_ref.bit_width(), 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(fixed_2dim_ref.get().size(), fixed_2dim_ref.bit_width());
  unsigned subarray_2[2];
  fixed_2dim_ref.Decrypt(subarray_2, sk());
  for (int k = 0; k < 2; k++) {
    EXPECT_EQ(array[1][2][k], subarray_2[k]);
  }

  // Obtain the subarray directly from the array.
  OpenFheArrayRef<unsigned, 2> fixed_2dim_direct_ref = fixed_4x3x2dim[1][2];
  EXPECT_EQ(fixed_2dim_direct_ref.length(), 2);
  EXPECT_EQ(fixed_2dim_direct_ref.bit_width(), 2 * sizeof(unsigned) * 8);
  EXPECT_EQ(fixed_2dim_direct_ref.get().size(),
            fixed_2dim_direct_ref.bit_width());
  unsigned subarray_direct_2[2];
  fixed_2dim_direct_ref.Decrypt(subarray_direct_2, sk());
  for (int k = 0; k < 2; k++) {
    EXPECT_EQ(array[1][2][k], subarray_direct_2[k]);
  }
}

}  // namespace
}  // namespace transpiler
}  // namespace fully_homomorphic_encryption
