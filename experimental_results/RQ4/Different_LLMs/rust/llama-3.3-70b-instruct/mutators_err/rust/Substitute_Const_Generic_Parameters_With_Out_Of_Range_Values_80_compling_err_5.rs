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
                if let Some(generics) = &enum_item.generics {
                    let generics = generics.clone();
                    for param in &generics.params {
                        if let GenericParam::Const(param) = param {
                            if let Some(default) = &param.default {
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
                                            "u16" => 256u8,
                                            "u32" => 256u8,
                                            "u64" => 256u8,
                                            "i8" => 128i8,
                                            "i16" => 128i8,
                                            "i32" => 128i8,
                                            "i64" => 128i8,
                                            _ => unreachable!(),
                                        }
                                    }
                                    _ => unreachable!(),
                                };
                                let new_default = parse_quote!(#out_of_range_value);
                                param.default = Some(new_default);
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