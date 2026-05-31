use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident as SynIdent,
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

pub struct Replace_Param_With_TAIT_Destructure_479;

impl Mutator for Replace_Param_With_TAIT_Destructure_479 {
    fn name(&self) -> &str {
        "Replace_Param_With_TAIT_Destructure_479"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_tait = false;
        let mut has_foo = false;
        for item in &file.items {
            if let syn::Item::Type(ty) = item {
                if ty.ident == "Tait" {
                    has_tait = true;
                }
            }
            if let syn::Item::Struct(strukt) = item {
                if strukt.ident == "Foo" {
                    has_foo = true;
                }
            }
        }
        if !has_tait {
            let tait_type = parse_quote! {
                pub type Tait = impl Sized;
            };
            file.items.insert(0, syn::Item::Type(tait_type));
        }
        if !has_foo {
            let foo_struct = parse_quote! {
                pub struct Foo { field: String }
            };
            file.items.insert(0, syn::Item::Struct(foo_struct));
        }

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(first_input) = func.sig.inputs.first_mut() {
                    if let FnArg::Typed(pat_type) = first_input {
                        pat_type.ty = Box::new(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                    ident: Ident::new("Tait", Span::call_site()),
                                    arguments: syn::PathArguments::None,
                                }]),
                            },
                        }));
                        if let syn::Pat::Ident(ident_pat) = &*pat_type.pat {
                            let param_name = ident_pat.ident.to_string();
                            let new_stmt = syn::Stmt::Local(syn::Local {
                                let_token: Default::default(),
                                pat: syn::Pat::Struct(syn::PatStruct {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: Ident::new("Foo", Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        }]),
                                    },
                                    brace_token: Default::default(),
                                    fields: {
                                        let mut fields = syn::punctuated::Punctuated::new();
                                        fields.push(syn::PatField {
                                            attrs: Vec::new(),
                                            ident: Some(Ident::new("field", Span::call_site())),
                                            colon_token: Default::default(),
                                            pat: Box::new(syn::Pat::Wild(Default::default())),
                                            is_shorthand: false,
                                        });
                                        fields
                                    },
                                    rest: None,
                                }),
                                init: Some(Box::new(syn::LocalInit {
                                    eq_token: syn::token::Eq::default(),
                                    expr: Box::new(syn::Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                                ident: Ident::new(&param_name, Span::call_site()),
                                                arguments: syn::PathArguments::None,
                                            }]),
                                        },
                                    })),
                                    underscore_token: None,
                                })),
                                attrs: Vec::new(),
                                semi_token: Default::default(),
                            });
                            let dummy_stmt = syn::Stmt::Expr(syn::Expr::Assign(syn::ExprAssign {
                                attrs: Vec::new(),
                                left: Box::new(syn::Expr::Path(syn::ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: Ident::new("_", Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        }]),
                                    },
                                })),
                                eq_token: syn::token::Eq::default(),
                                right: Box::new(syn::Expr::Path(syn::ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: syn::punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                            ident: Ident::new("field", Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        }]),
                                    },
                                })),
                            }), None);
                            func.block.stmts.insert(0, new_stmt);
                            func.block.stmts.insert(1, dummy_stmt);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}