use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMod, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Add_Conflicting_Enum_Union_184;

impl Mutator for Add_Conflicting_Enum_Union_184 {
    fn name(&self) -> &str {
        "Add_Conflicting_Enum_Union_184"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut module_exists = false;
        for item in &mut file.items {
            if let syn::Item::Mod(module) = item {
                if module.ident == "invalid_layouts" {
                    module_exists = true;
                    break;
                }
            }
        }

        if !module_exists {
            let new_module = ItemMod {
                attrs: vec![],
                vis: Visibility::Inherited,
                ident: Ident::new("invalid_layouts", Span::call_site()),
                content: Some(Box::new(ModContent {
                    brace_token: Default::default(),
                    items: {
                        let mut items = Vec::new();

                        // Add enum with conflicting repr attributes
                        let enum_attrs = vec![
                            parse_quote! { #[repr(packed)] },
                            parse_quote! { #[repr(u32)] },
                        ];
                        let variants = Punctuated::from_iter(vec![
                            syn::ItemEnumVariant {
                                attrs: vec![].into_iter().collect(),
                                ident: Ident::new("A", Span::call_site()),
                                fields: syn::Fields::Unit,
                                discriminant: None,
                            },
                            syn::ItemEnumVariant {
                                attrs: vec![].into_iter().collect(),
                                ident: Ident::new("B", Span::call_site()),
                                fields: syn::Fields::Unit,
                                discriminant: None,
                            },
                            syn::ItemEnumVariant {
                                attrs: vec![].into_iter().collect(),
                                ident: Ident::new("C", Span::call_site()),
                                fields: syn::Fields::Unit,
                                discriminant: None,
                            },
                        ]);
                        let enum_e = ItemEnum {
                            attrs: enum_attrs,
                            ident: Ident::new("E", Span::call_site()),
                            generics: Generics::default(),
                            brace_token: Default::default(),
                            variants,
                        };
                        items.push(Item::Enum(enum_e));

                        // Add union combining enum and its underlying type
                        let union_invalid_tag = ItemUnion {
                            attrs: vec![].into_iter().collect(),
                            ident: Ident::new("InvalidTag", Span::call_site()),
                            generics: Generics::default(),
                            brace_token: Default::default(),
                            fields: Fields::Named(syn::FieldsNamed {
                                brace_token: Default::default(),
                                named: Punctuated::from_iter(vec![
                                    syn::Field {
                                        attrs: vec![].into_iter().collect(),
                                        ident: Some(Ident::new("int", Span::call_site())),
                                        colon_token: Default::default(),
                                        ty: Box::new(syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("u32", Span::call_site())),
                                        })),
                                    },
                                    syn::Field {
                                        attrs: vec![].into_iter().collect(),
                                        ident: Some(Ident::new("e", Span::call_site())),
                                        colon_token: Default::default(),
                                        ty: Box::new(syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("E", Span::call_site())),
                                        })),
                                    },
                                ]),
                            },
                        };
                        items.push(Item::Union(union_invalid_tag));

                        // Add function to trigger union initialization
                        let test_function = ItemFn {
                            attrs: vec![].into_iter().collect(),
                            vis: Visibility::Inherited,
                            unsafety: Some(token::Unsafe {
                                span: Span::call_site(),
                            }),
                            sig: Signature {
                                constness: None,
                                asyncness: None,
                                unsafety: Some(token::Unsafe {
                                    span: Span::call_site(),
                                }),
                                abi: None,
                                ident: Ident::new("test_invalid_tag", Span::call_site()),
                                generics: Generics::default(),
                                paren_token: Default::default(),
                                inputs: Punctuated::new(),
                                output: ReturnType::Default(Default::default()),
                            },
                            block: Box::new(syn::Block {
                                brace_token: Default::default(),
                                stmts: Punctuated::from_iter(vec![Stmt::Local(parse_quote! {
                                    let x: InvalidTag = InvalidTag { int: 42 };
                                })]),
                            }),
                        };
                        items.push(Item::Fn(test_function));

                        items
                    },
                }),
                ..Default::default()
            };
            file.items.push(Item::Mod(new_module));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an enum with conflicting repr attributes (packed and u32) and a union combining the enum and its underlying integer type. This forces the compiler to reconcile incompatible layout and alignment expectations, potentially exposing bugs in MIR optimizations or layout validation during compilation."
    }
}