use crate::mutator::Mutator;
use syn::Item;
use syn::punctuated::Punctuated;
use syn::GenericParam;
use syn::LifetimeParam;
use syn::Lifetime;
use syn::GenericArgument;
use proc_macro2::Span;
use syn::TypeParam;
use syn::Ident;
use syn::PathArguments;
use syn::AngleBracketedGenericArguments;
use syn::token::{Lt, Gt, Colon, Eq, Const};
use rand::thread_rng;
use rand::prelude::SliceRandom;

pub struct Add_Const_Generic_Reference_97;

impl Mutator for Add_Const_Generic_Reference_97 {
    fn name(&self) -> &str {
        "Add_Const_Generic_Reference_97"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_enum_names = Vec::new();
        for item in &file.items {
            if let Item::Struct(s) = item {
                struct_enum_names.push(s.ident.to_string());
            } else if let Item::Enum(e) = item {
                struct_enum_names.push(e.ident.to_string());
            }
        }

        for item in &mut file.items {
            if let Item::Struct(s) = item {
                if s.generics.params.is_empty() {
                    continue;
                }

                let current_name = s.ident.to_string();
                let available_names: Vec<_> = struct_enum_names
                    .iter()
                    .filter(|name| *name != current_name)
                    .collect();

                if available_names.is_empty() {
                    continue;
                }

                let selected_name = available_names.choose(&mut thread_rng()).unwrap();

                let mut generic_args = Punctuated::new();
                for param in &s.generics.params {
                    match param {
                        GenericParam::Lifetime(LifetimeParam { lifetime: Lifetime { ident, .. }, .. }) => {
                            generic_args.push(GenericArgument::Lifetime(Lifetime::new(&ident.to_string(), Span::call_site())));
                        },
                        GenericParam::Type(TypeParam { ident, .. }) => {
                            generic_args.push(GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new(&ident.to_string(), Span::call_site())),
                            })));
                        },
                        _ => {}
                    }
                }

                let type_path = syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: {
                        let mut segments = Punctuated::new();
                        segments.push(syn::PathSegment {
                            ident: Ident::new(selected_name, Span::call_site()),
                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: Lt::default(),
                                args: generic_args,
                                gt_token: Gt::default(),
                            }),
                        });
                        syn::Path {
                            leading_colon: None,
                            segments,
                        }
                    },
                });

                let const_param = GenericParam::Const(syn::ConstParam {
                    attrs: Vec::new(),
                    const_token: Const::default(),
                    ident: Ident::new("C", Span::call_site()),
                    colon_token: Colon::default(),
                    ty: type_path,
                    default: Default::default(),
                    eq_token: Some(Eq::default()),
                });

                s.generics.params.push(const_param);
            } else if let Item::Enum(e) = item {
                if e.generics.params.is_empty() {
                    continue;
                }

                let current_name = e.ident.to_string();
                let available_names: Vec<_> = struct_enum_names
                    .iter()
                    .filter(|name| *name != current_name)
                    .collect();

                if available_names.is_empty() {
                    continue;
                }

                let selected_name = available_names.choose(&mut thread_rng()).unwrap();

                let mut generic_args = Punctuated::new();
                for param in &e.generics.params {
                    match param {
                        GenericParam::Lifetime(LifetimeParam { lifetime: Lifetime { ident, .. }, .. }) => {
                            generic_args.push(GenericArgument::Lifetime(Lifetime::new(&ident.to_string(), Span::call_site())));
                        },
                        GenericParam::Type(TypeParam { ident, .. }) => {
                            generic_args.push(GenericArgument::Type(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new(&ident.to_string(), Span::call_site())),
                            })));
                        },
                        _ => {}
                    }
                }

                let type_path = syn::Type::Path(syn::TypePath {
                    qself: None,
                    path: {
                        let mut segments = Punctuated::new();
                        segments.push(syn::PathSegment {
                            ident: Ident::new(selected_name, Span::call_site()),
                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: Lt::default(),
                                args: generic_args,
                                gt_token: Gt::default(),
                            }),
                        });
                        syn::Path {
                            leading_colon: None,
                            segments,
                        }
                    },
                });

                let const_param = GenericParam::Const(syn::ConstParam {
                    attrs: Vec::new(),
                    const_token: Const::default(),
                    ident: Ident::new("C", Span::call_site()),
                    colon_token: Colon::default(),
                    ty: type_path,
                    default: Default::default(),
                    eq_token: Some(Eq::default()),
                });

                e.generics.params.push(const_param);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a const generic parameter to structs/enums with existing generics, using another struct/enum from the same program as the const type. This leverages existing type/lifetime parameters to create invalid const generics, testing compiler handling of generic const types and cross-references between type definitions."
    }
}