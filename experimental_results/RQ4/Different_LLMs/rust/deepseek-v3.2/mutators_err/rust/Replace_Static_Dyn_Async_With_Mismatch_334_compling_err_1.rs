use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Static_Dyn_Async_With_Mismatch_334;

impl Mutator for Replace_Static_Dyn_Async_With_Mismatch_334 {
    fn name(&self) -> &str {
        "Replace_Static_Dyn_Async_With_Mismatch_334"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_types = Vec::new();
        let mut enum_types = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                struct_types.push(item_struct.ident.clone());
            }
            if let Item::Enum(item_enum) = item {
                enum_types.push(item_enum.ident.clone());
            }
        }
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::TraitObject(type_trait_object) = &*item_static.ty {
                    if type_trait_object.dyn_token.is_some() {
                        if let Expr::Async(expr_async) = &*item_static.expr {
                            let candidate_types: Vec<Ident> = struct_types.iter().chain(enum_types.iter()).cloned().collect();
                            let selected_type = if candidate_types.is_empty() {
                                let new_struct_name = Ident::new("NonSendStruct", Span::call_site());
                                let new_struct = ItemStruct {
                                    attrs: Vec::new(),
                                    vis: syn::Visibility::Public(syn::VisPublic {
                                        pub_token: token::Pub::default(),
                                    }),
                                    struct_token: token::Struct::default(),
                                    ident: new_struct_name.clone(),
                                    generics: syn::Generics::default(),
                                    fields: syn::Fields::Named(syn::FieldsNamed {
                                        brace_token: token::Brace::default(),
                                        named: {
                                            let mut punctuated = Punctuated::new();
                                            punctuated.push(syn::Field {
                                                attrs: Vec::new(),
                                                vis: syn::Visibility::Public(syn::VisPublic {
                                                    pub_token: token::Pub::default(),
                                                }),
                                                ident: Some(Ident::new("raw_ptr", Span::call_site())),
                                                colon_token: Some(token::Colon::default()),
                                                ty: syn::Type::Ptr(syn::TypePtr {
                                                    star_token: token::Star::default(),
                                                    const_token: None,
                                                    mutability: None,
                                                    elem: Box::new(syn::Type::Path(syn::TypePath {
                                                        qself: None,
                                                        path: syn::Path::from(Ident::new("i32", Span::call_site())),
                                                    })),
                                                }),
                                            });
                                            punctuated
                                        },
                                    }),
                                    semi_token: None,
                                };
                                file.items.insert(0, Item::Struct(new_struct));
                                new_struct_name
                            } else {
                                candidate_types.choose(&mut rng).unwrap().clone()
                            };
                            let new_expr: Expr = parse_quote! {
                                async { #selected_type }
                            };
                            item_static.expr = Box::new(new_expr);
                        }
                    }
                }
            }
            if let Item::Const(item_const) = item {
                if let Type::TraitObject(type_trait_object) = &*item_const.ty {
                    if type_trait_object.dyn_token.is_some() {
                        if let Expr::Async(expr_async) = &*item_const.expr {
                            let candidate_types: Vec<Ident> = struct_types.iter().chain(enum_types.iter()).cloned().collect();
                            let selected_type = if candidate_types.is_empty() {
                                let new_struct_name = Ident::new("NonSendEnum", Span::call_site());
                                let new_enum = ItemEnum {
                                    attrs: Vec::new(),
                                    vis: syn::Visibility::Public(syn::VisPublic {
                                        pub_token: token::Pub::default(),
                                    }),
                                    enum_token: token::Enum::default(),
                                    ident: new_struct_name.clone(),
                                    generics: syn::Generics::default(),
                                    brace_token: token::Brace::default(),
                                    variants: {
                                        let mut punctuated = Punctuated::new();
                                        punctuated.push(syn::Variant {
                                            attrs: Vec::new(),
                                            ident: Ident::new("VariantWithPointer", Span::call_site()),
                                            fields: syn::Fields::Named(syn::FieldsNamed {
                                                brace_token: token::Brace::default(),
                                                named: {
                                                    let mut inner_punctuated = Punctuated::new();
                                                    inner_punctuated.push(syn::Field {
                                                        attrs: Vec::new(),
                                                        vis: syn::Visibility::Public(syn::VisPublic {
                                                            pub_token: token::Pub::default(),
                                                        }),
                                                        ident: Some(Ident::new("ptr", Span::call_site())),
                                                        colon_token: Some(token::Colon::default()),
                                                        ty: syn::Type::Ptr(syn::TypePtr {
                                                            star_token: token::Star::default(),
                                                            const_token: None,
                                                            mutability: None,
                                                            elem: Box::new(syn::Type::Path(syn::TypePath {
                                                                qself: None,
                                                                path: syn::Path::from(Ident::new("u8", Span::call_site())),
                                                            })),
                                                        }),
                                                    });
                                                    inner_punctuated
                                                },
                                            }),
                                            discriminant: None,
                                        });
                                        punctuated
                                    },
                                };
                                file.items.insert(0, Item::Enum(new_enum));
                                new_struct_name
                            } else {
                                candidate_types.choose(&mut rng).unwrap().clone()
                            };
                            let new_expr: Expr = parse_quote! {
                                async { #selected_type }
                            };
                            item_const.expr = Box::new(new_expr);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static and const declarations with dyn* types initialized with async blocks. It replaces the async block's inner expression with a type that likely does not implement the required traits (Send/Sync). If no suitable struct/enum exists in scope, it introduces a new non-Send type (struct with raw pointer or enum with pointer field). This preserves the problematic combination of dyn* and async blocks while creating type mismatches, aiming to trigger internal compiler errors in the incomplete dyn* feature combined with const async blocks."
    }
}