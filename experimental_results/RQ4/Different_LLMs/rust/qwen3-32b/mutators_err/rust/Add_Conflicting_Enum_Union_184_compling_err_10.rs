use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMod, ItemStruct, ItemEnum, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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
                content: Some(Box::new(syn::ModContent {
                    brace_token: token::Brace::default(),
                    items: Punctuated::from_iter(vec![
                        Item::Enum(ItemEnum {
                            attrs: vec![
                                parse_quote! { #[repr(packed)] },
                                parse_quote! { #[repr(u32)] },
                            ],
                            ident: Ident::new("E", Span::call_site()),
                            generics: Generics::default(),
                            enum_token: Default::default(),
                            brace_token: Default::default(),
                            variants: Punctuated::from_iter(vec![
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
                            ]),
                            vis: Visibility::Inherited,
                        }),
                        Item::Union(ItemUnion {
                            attrs: vec![],
                            ident: Ident::new("InvalidTag", Span::call_site()),
                            generics: Generics::default(),
                            union_token: Default::default(),
                            brace_token: token::Brace::default(),
                            fields: syn::Fields::Named(FieldsNamed {
                                brace_token: token::Brace::default(),
                                named: Punctuated::from_iter(vec![
                                    syn::Field {
                                        attrs: vec![].into_iter().collect(),
                                        ident: Some(Ident::new("int", Span::call_site())),
                                        colon_token: Default::default(),
                                        ty: syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("u32", Span::call_site())),
                                        }),
                                        mutability: None,
                                        vis: Visibility::Inherited,
                                    },
                                    syn::Field {
                                        attrs: vec![].into_iter().collect(),
                                        ident: Some(Ident::new("e", Span::call_site())),
                                        colon_token: Default::default(),
                                        ty: syn::Type::Path(syn::TypePath {
                                            qself: None,
                                            path: syn::Path::from(Ident::new("E", Span::call_site())),
                                        }),
                                        mutability: None,
                                        vis: Visibility::Inherited,
                                    },
                                ]),
                            }),
                        }),
                        Item::Fn(ItemFn {
                            attrs: vec![],
                            vis: Visibility::Inherited,
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
                                output: ReturnType::Default,
                                fn_token: Default::default(),
                                variadic: None,
                            },
                            block: Box::new(syn::Block {
                                brace_token: Default::default(),
                                stmts: Punctuated::from_iter(vec![Stmt::Local(Local {
                                    let_token: token::Let {
                                        span: Span::call_site(),
                                    },
                                    pat: Box::new(Pat::Ident(PatIdent {
                                        attrs: vec![],
                                        ident: Ident::new("x", Span::call_site()),
                                        mutability: None,
                                        subpat: None,
                                        colon_token: None,
                                        eq_token: None,
                                        pat: None,
                                    })),
                                    colon_token: Some(token::Colon {
                                        span: Span::call_site(),
                                    }),
                                    ty: Some(Box::new(syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path::from(Ident::new("InvalidTag", Span::call_site())),
                                    }))),
                                    eq_token: Some(token::Eq {
                                        span: Span::call_site(),
                                    }),
                                    expr: Box::new(Expr::Struct(ExprStruct {
                                        attrs: vec![],
                                        qself: None,
                                        path: SynPath::from(Ident::new("InvalidTag", Span::call_site())),
                                        brace_token: token::Brace::default(),
                                        fields: Punctuated::from_iter(vec![ExprField {
                                            attrs: vec![],
                                            member: Member::Named(Ident::new("int", Span::call_site())),
                                            colon_token: token::Colon::default(),
                                            expr: Box::new(Expr::Lit(ExprLit {
                                                attrs: vec![],
                                                lit: Lit::Int(LitInt::new("42", Span::call_site())),
                                            })),
                                        }]),
                                        dot2_token: None,
                                        rest: None,
                                    })),
                                    semi_token: Some(token::Semi {
                                        span: Span::call_site(),
                                    }),
                                    init: None,
                                })]),
                            }),
                    ]),
                })),
                ..Default::default()
            };
            file.items.push(Item::Mod(new_module));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an enum with conflicting repr attributes (packed and u32) and a union combining the enum and its underlying integer type. This forces the compiler to reconcile incompatible layout and alignment expectations, potentially exposing bugs in MIR optimizations or layout validation during compilation."
    }
}