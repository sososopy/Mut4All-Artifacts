use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Constant_Generics_With_Constants_90;

impl Mutator for Replace_Constant_Generics_With_Constants_90 {
    fn name(&self) -> &str {
        "Replace_Constant_Generics_With_Constants_90"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in generics.params.iter_mut() {
                    if let GenericParam::Const(param) = param {
                        let const_value = match param.ty {
                            Type::Path(ref type_path) => {
                                if type_path.path.is_ident("usize") {
                                    quote!(10usize)
                                } else if type_path.path.is_ident("isize") {
                                    quote!(10isize)
                                } else if type_path.path.is_ident("bool") {
                                    quote!(true)
                                } else if type_path.path.is_ident("char") {
                                    quote!('a')
                                } else {
                                    quote!(10usize)
                                }
                            }
                            _ => quote!(10usize),
                        };
                        let let_stmt = Stmt::Local(Local {
                            attrs: vec![],
                            let_token: token::Let {
                                spans: [Span::call_site()],
                            },
                            pat: Pat::Ident(Ident::new(&param.ident.to_string(), Span::call_site()).into()),
                            init: Some(Expr::Verbatim(const_value).into()),
                            semi_token: token::Semi {
                                spans: [Span::call_site()],
                            },
                        });
                        func.block.stmts.insert(0, let_stmt);
                        param.default = Some(syn::Expr::Verbatim(const_value));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        for param in generics.params.iter_mut() {
                            if let GenericParam::Const(param) = param {
                                let const_value = match param.ty {
                                    Type::Path(ref type_path) => {
                                        if type_path.path.is_ident("usize") {
                                            quote!(10usize)
                                        } else if type_path.path.is_ident("isize") {
                                            quote!(10isize)
                                        } else if type_path.path.is_ident("bool") {
                                            quote!(true)
                                        } else if type_path.path.is_ident("char") {
                                            quote!('a')
                                        } else {
                                            quote!(10usize)
                                        }
                                    }
                                    _ => quote!(10usize),
                                };
                                let let_stmt = Stmt::Local(Local {
                                    attrs: vec![],
                                    let_token: token::Let {
                                        spans: [Span::call_site()],
                                    },
                                    pat: Pat::Ident(Ident::new(&param.ident.to_string(), Span::call_site()).into()),
                                    init: Some(Expr::Verbatim(const_value).into()),
                                    semi_token: token::Semi {
                                        spans: [Span::call_site()],
                                    },
                                });
                                func.block.stmts.insert(0, let_stmt);
                                param.default = Some(syn::Expr::Verbatim(const_value));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant generics with concrete constant values, testing the compiler's handling of constant generics and their interaction with function and method signatures."
    }
}