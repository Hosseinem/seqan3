// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides the seqan3::record template and the seqan3::field enum.
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 */

#pragma once

#include <limits>
#include <tuple>

#include <seqan3/core/detail/template_inspection.hpp>

namespace seqan3
{

// ----------------------------------------------------------------------------
// enum field
// ----------------------------------------------------------------------------

/*!\brief An enumerator for the fields used in file formats.
 * \ingroup io
 *
 * Some of the fields are shared between formats.
 *
 * The following table shows the usage of fields in the respective files
 * (Note that each valid format for a file must handle all of its fields):
 *
 * | Field          | Sequence IO | Alignment IO | Structure IO |
 * | -------------- | ----------- | ------------ | ------------ |
 * | seq            |      ✅      |      ✅       |       ✅      |
 * | id             |      ✅      |      ✅       |       ✅      |
 * | qual           |      ✅      |      ✅       |       ✅      |
 * | seq_qual       |      ✅      |              |              |
 * | offset         |             |      ✅       |       ✅      |
 * | bpp            |             |              |       ✅      |
 * | structure      |             |              |       ✅      |
 * | structured_seq |             |              |       ✅      |
 * | energy         |             |              |       ✅      |
 * | react          |             |              |       ✅      |
 * | react_err      |             |              |       ✅      |
 * | comment        |             |              |       ✅      |
 * | alignment      |             |      ✅       |              |
 * | ref_id         |             |      ✅       |              |
 * | ref_seq        |             |      ✅       |              |
 * | ref_offset     |             |      ✅       |              |
 * | header_ptr     |             |      ✅       |              |
 * | flag           |             |      ✅       |              |
 * | mate           |             |      ✅       |              |
 * | mapq           |             |      ✅       |              |
 * | cigar          |             |      ✅       |              |
 * | tags           |             |      ✅       |              |
 * | bit_score      |             |      ✅       |              |
 * | evalue         |             |      ✅       |              |
 */
enum class field
{
    // Fields used in multiple contexts ........................................
    seq,            //!< The "sequence", usually a range of nucleotides or amino acids.
    id,             //!< The identifier, usually a string.
    qual,           //!< The qualities, usually in Phred score notation.

    offset,         //!< Sequence (seqan3::field::seq) relative start position (0-based), unsigned value.

    // Fields unique to structure io ...........................................
    bpp,            //!< Base pair probability matrix of interactions, usually a matrix of float numbers.
    structure,      //!< Fixed interactions, usually a string of structure alphabet characters.
    structured_seq, //!< Sequence and fixed interactions combined in one range.
    energy,         //!< Energy of a folded sequence, represented by one float number.
    react,          //!< Reactivity values of the sequence characters given in a vector of float numbers.
    react_err,      //!< Reactivity error values given in a vector corresponding to seqan3::field::react.
    comment,        //!< Comment field of arbitrary content, usually a string.

    // Fields unique to alignment io ...........................................
    alignment,      //!< The (pairwise) alignment stored in an object that models seqan3::detail::pairwise_alignment.
    ref_id,         //!< The identifier of the (reference) sequence that seqan3::field::seq was aligned to.
    ref_seq,        //!< The (reference) "sequence" information, usually a range of nucleotides or amino acids.
    ref_offset,     //!< Sequence (seqan3::field::ref_seq) relative start position (0-based), unsigned value.
    header_ptr,     //!< A pointer to the seqan3::sam_file_header object storing header information.

    flag,           //!< The alignment flag (bit information), `uint16_t` value.
    mate,           //!< The mate pair information given as a std::tuple of reference name, offset and template length.
    mapq,           //!< The mapping quality of the seqan3::field::seq alignment, usually a Phred-scaled score.
    cigar,          //!< The cigar vector (std::vector<seqan3::cigar>) representing the alignment in SAM/BAM format.
    tags,           //!< The optional tags in the SAM format, stored in a dictionary.

    bit_score,      //!< The bit score (statistical significance indicator), unsigned value.
    evalue,         //!< The e-value (length normalized bit score), `double` value.

    // User defined field aliases .. ...........................................
    user_defined_0, //!< Identifier for user defined file formats and specialisations.
    user_defined_1, //!< Identifier for user defined file formats and specialisations.
    user_defined_2, //!< Identifier for user defined file formats and specialisations.
    user_defined_3, //!< Identifier for user defined file formats and specialisations.
    user_defined_4, //!< Identifier for user defined file formats and specialisations.
    user_defined_5, //!< Identifier for user defined file formats and specialisations.
    user_defined_6, //!< Identifier for user defined file formats and specialisations.
    user_defined_7, //!< Identifier for user defined file formats and specialisations.
    user_defined_8, //!< Identifier for user defined file formats and specialisations.
    user_defined_9  //!< Identifier for user defined file formats and specialisations.
};

// ----------------------------------------------------------------------------
// fields
// ----------------------------------------------------------------------------

/*!\brief       A class template that holds a choice of seqan3::field.
 * \ingroup io
 * \tparam fs   The fields you wish to be present in the seqan3::record returned by your file.
 * \see         seqan3::record
 *
 * This class acts as a compile time list of seqan3::field elements. It is used in specialising file classes
 * to determine the elements in a seqan3::record.
 *
 * ### Example
 *
 * \include test/snippet/io/record_1.cpp
 *
 */
template <field ...fs>
struct fields
{
    //!\privatesection
    //!\brief The template parameters stored in an array for easy access.
    static constexpr std::array<field, sizeof...(fs)> as_array{fs...};

    //!\brief Special value that indicates that index_of() failed.
    static constexpr size_t npos = std::numeric_limits<size_t>::max();

    //!\brief The size of fields.
    static constexpr size_t size = sizeof...(fs);

    //!\brief Retrieve the position of field in the parameter pack.
    static constexpr size_t index_of(field f)
    {
        for (size_t i = 0; i < sizeof...(fs); ++i)
            if (as_array[i] == f)
                return i;
        return npos;
    }

    //!\brief Whether a field is contained in the parameter pack.
    static constexpr bool contains(field f)
    {
        return index_of(f) != npos;
    }

    static_assert([] () constexpr
                  {
                      for (size_t i = 0; i < as_array.size(); ++i)
                          for (size_t j = i + 1; j < as_array.size(); ++j)
                              if (as_array[i] == as_array[j])
                                  return false;

                      return true;
                  } (), "You may not include a field twice into fields<>.");
};

// ----------------------------------------------------------------------------
// record
// ----------------------------------------------------------------------------

/*!\brief The class template that file records are based on; behaves like a std::tuple.
 * \ingroup io
 * \implements seqan3::tuple_like
 * \tparam field_types The types of the fields in this record as a seqan3::type_list.
 * \tparam field_ids   A seqan3::fields type with seqan3::field IDs corresponding to field_types.
 *
 * This class template behaves just like a std::tuple, with the exception that it provides an additional
 * get-interface that takes a seqan3::field identifier. The traditional get interfaces (via index and
 * via type) are also supported, but discouraged, because accessing via seqan3::field is unambiguous and
 * better readable.
 *
 * ### Example
 *
 * For input files this template is specialised automatically and provided by the file via its `record_type` member.
 * For output files you my define it locally and pass instances of this to the output file's `push_back()`.
 *
 * This is how it works:
 *
 * \include test/snippet/io/record_2.cpp
 */
template <typename field_types, typename field_ids>
struct record : detail::transfer_template_args_onto_t<field_types, std::tuple>
{
private:
    //!\brief Auxiliary functions for clear().
    template <typename t>
    //!\cond
        requires requires (t & v) { v.clear(); }
    //!\endcond
    static constexpr void clear_element(t & v) noexcept(noexcept(v.clear()))
    {
        v.clear();
    }

    //!\overload
    template <typename t>
    static constexpr void clear_element(t & v) noexcept(noexcept(std::declval<t &>() = t{}))
    {
        v = {};
    }

    //!\brief A lambda function that expands a pack and calls `clear_element` on every argument in the pack.
    static constexpr auto expander = [] (auto & ...args) { (clear_element(args), ...); };

public:
    //!\brief A specialisation of std::tuple.
    using base_type = detail::transfer_template_args_onto_t<field_types, std::tuple>;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    record() = default;                           //!< Defaulted.
    record(record const &) = default;             //!< Defaulted.
    record & operator=(record const &) = default; //!< Defaulted.
    record(record &&) = default;                  //!< Defaulted.
    record & operator=(record &&) = default;      //!< Defaulted.
    ~record() = default;                          //!< Defaulted.

    //!\brief Inherit std::tuple's constructors.
    using base_type::base_type;
    //!\}

    static_assert(field_types::size() == field_ids::as_array.size(),
                  "You must give as many IDs as types to seqan3::record.");

    //!\brief Clears containers that provide `.clear()` and (re-)initialises all other elements with `= {}`.
    void clear() noexcept(noexcept(std::apply(expander, std::declval<record &>())))
    {
        std::apply(expander, *this);
    }

protected:
    //!\privatesection

    //!\brief A type alias for std::integral_constant
    template <field f>
    using field_constant = std::integral_constant<field, f>;

    //!\brief This is basically the seqan3::get<f>(static_cast<tuple>(record)) implementation
    template <field f, typename tuple_t>
    static decltype(auto) get_impl(field_constant<f>, tuple_t && record_as_tuple)
    {
        static_assert(field_ids::contains(f), "The record does not contain the field you wish to retrieve.");
        return std::get<field_ids::index_of(f)>(std::forward<tuple_t>(record_as_tuple));
    }
};

} // namespace seqan3

namespace std
{

/*!\brief Provides access to the number of elements in a tuple as a compile-time constant expression.
 * \implements seqan3::unary_type_trait
 * \relates seqan3::record
 * \see std::tuple_size_v
 */
template <typename field_types, typename field_ids>
struct tuple_size<seqan3::record<field_types, field_ids>>
    : tuple_size<typename seqan3::record<field_types, field_ids>::base_type>
{};

/*!\brief Obtains the type of the specified element.
 * \implements seqan3::transformation_trait
 * \relates seqan3::record
 * \see [std::tuple_element](https://en.cppreference.com/w/cpp/utility/tuple/tuple_element)
 */
template <size_t elem_no, typename field_types, typename field_ids>
struct tuple_element<elem_no, seqan3::record<field_types, field_ids>>
    : tuple_element<elem_no, typename seqan3::record<field_types, field_ids>::base_type>
{};

} // namespace std
