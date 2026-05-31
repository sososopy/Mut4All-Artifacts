use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::GenericParam;
use syn::Type;
use syn::TypePath;
use syn::punctuated::Punctuated;
use syn::PathSegment;
use syn::Ident;
use proc_macro2::Span;
use syn::PathArguments;
use syn::Path;

pub struct Invalid_Const_Generic_Type_Assignment_375;

impl Mutator for Invalid_Const_Generic_Type_Assignment_375 {
    fn name(&self) -> &str {
        "Invalid_Const_Generic_Type_Assignment_375"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Path(TypePath {
                            qself: None,
                            path: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("str", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                Path {
                                    leading_colon: None,
                                    segments,
                                }
                            },
                        });
                        const_param.default = Some(parse_quote!(10));
                    }
                }
            } else if let Item::Enum(item_enum) = item {
                for param in &mut item_enum.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Path(TypePath {
                            qself: None,
                            path: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("str", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                Path {
                                    leading_colon: None,
                                    segments,
                                }
                            },
                        });
                        const_param.default = Some(parse_quote!(10));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}