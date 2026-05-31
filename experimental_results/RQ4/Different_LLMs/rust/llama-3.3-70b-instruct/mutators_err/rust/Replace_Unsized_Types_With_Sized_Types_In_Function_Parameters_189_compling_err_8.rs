use syn::{FnArg, Type, TypePath, PathSegment, Ident, PathArguments, AngleBracketedGenericArguments, GenericArgument};
use syn::token::{Lt, Gt};
use proc_macro2::Span;
use crate::mutator::Mutator;
use syn::punctuated::Punctuated;

struct Replace_Unsized_Types_With_Sized_Types_In_Function_Parameters_189;

impl Mutator for Replace_Unsized_Types_With_Sized_Types_In_Function_Parameters_189 {
    fn name(&self) -> &str {
        "Replace_Unsized_Types_With_Sized_Types_In_Function_Parameters_189"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ref_type) = *pat_type.ty {
                            if let Type::Slice(slice_type) = &*ref_type.elem {
                                // Replace slice with Vec
                                *pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("Vec", Span::call_site()),
                                            arguments: PathArguments::AngleBracketed(
                                                AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: Lt::default(),
                                                    args: Punctuated::from_iter(vec![
                                                        GenericArgument::Type(*slice_type.elem.clone()),
                                                    ]),
                                                    gt_token: Gt::default(),
                                                },
                                            ),
                                        }]),
                                    },
                                }));
                            } else if let Type::TraitObject(trait_object) = &*ref_type.elem {
                                // Replace trait object with a sized type
                                *pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("String", Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(ref_type) = *pat_type.ty {
                                    if let Type::Slice(slice_type) = &*ref_type.elem {
                                        // Replace slice with Vec
                                        *pat_type.ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("Vec", Span::call_site()),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: Lt::default(),
                                                            args: Punctuated::from_iter(vec![
                                                                GenericArgument::Type(*slice_type.elem.clone()),
                                                            ]),
                                                            gt_token: Gt::default(),
                                                        },
                                                    ),
                                                }]),
                                            },
                                        }));
                                    } else if let Type::TraitObject(trait_object) = &*ref_type.elem {
                                        // Replace trait object with a sized type
                                        *pat_type.ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("String", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                        }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces unsized types in function parameters with sized types. It targets function parameters that contain unsized types such as slices or trait objects and replaces them with sized types like Vec or String. This transformation aims to trigger bugs related to layout and sizing issues in the Rust compiler."
    }
}