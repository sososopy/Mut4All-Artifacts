use rand::thread_rng;
use rand::Rng;
use syn::{Item, Fields, Type, TypePath, PathSegment, Ident, PathArguments};
use proc_macro2::Span;

struct Replace_Enum_Variant_With_Incompatible_Type_241;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Enum_Variant_With_Incompatible_Type_241 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_With_Incompatible_Type_241"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    match &mut variant.fields {
                        Fields::Named(fields) => {
                            for field in &mut fields.named {
                                if let Type::Reference(_) = field.ty {
                                    let incompatible_type = if thread_rng().gen_bool(0.5) {
                                        Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: syn::punctuated::Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("String", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        })
                                    } else {
                                        Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: syn::punctuated::Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("i32", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        })
                                    };
                                    field.ty = incompatible_type;
                                }
                            }
                        }
                        Fields::Unnamed(fields) => {
                            for field in &mut fields.unnamed {
                                if let Type::Reference(_) = field.ty {
                                    let incompatible_type = if thread_rng().gen_bool(0.5) {
                                        Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: syn::punctuated::Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("String", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        })
                                    } else {
                                        Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: syn::punctuated::Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("i32", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        })
                                    };
                                    field.ty = incompatible_type;
                                }
                            }
                        }
                        Fields::Unit => {}
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces enum variants with incompatible types to trigger potential bugs in the Rust compiler's pattern matching and type checking mechanisms."
    }
}