use syn::{parse_quote, Item, GenericParam, Type, TypePath, Generics};
use crate::mutator::Mutator;

struct Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80;

impl Mutator for Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Parameters_With_Out_Of_Range_Values_80"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if let Some(generics) = &mut enum_item.generics {
                    let generics_mut = generics;
                    for param in &mut generics_mut.params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &mut param.default {
                                let out_of_range_value = match param.ty {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            segments: ref segments,
                                            ..
                                        },
                                        ..
                                    }) => {
                                        let segment = segments.first().unwrap();
                                        match segment.ident.to_string().as_str() {
                                            "u8" => 256u8,
                                            "u16" => 65536u16,
                                            "u32" => 4294967296u32,
                                            "u64" => 18446744073709551616u64,
                                            "i8" => 128i8,
                                            "i16" => 32768i16,
                                            "i32" => 2147483648i32,
                                            "i64" => 9223372036854775808i64,
                                            _ => unreachable!(),
                                        }
                                    }
                                    _ => unreachable!(),
                                };
                                let new_default = parse_quote!(#out_of_range_value);
                                *default = new_default;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes const generic parameters with out-of-range values, testing the handling of such cases by the Rust compiler. It targets enum definitions with const generics and default values calculated using expressions involving other const generic parameters."
    }
}