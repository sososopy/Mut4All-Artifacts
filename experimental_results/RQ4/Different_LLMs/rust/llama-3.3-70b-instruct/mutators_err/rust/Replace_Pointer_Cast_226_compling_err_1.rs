use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Pointer_Cast_226;

impl Mutator for Replace_Pointer_Cast_226 {
    fn name(&self) -> &str {
        "Replace_Pointer_Cast_226"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Cast(expr_cast) = expr {
                            if let Type::Pointer(pointer_type) = *expr_cast.expr {
                                let new_type = match pointer_type.mutability {
                                    syn::Mutability::Not => {
                                        Type::Pointer(syn::PointerType {
                                            mutability: syn::Mutability::Mut,
                                            elem: pointer_type.elem.clone(),
                                            colon_token: pointer_type.colon_token,
                                        })
                                    }
                                    syn::Mutability::Mut => {
                                        Type::Pointer(syn::PointerType {
                                            mutability: syn::Mutability::Not,
                                            elem: pointer_type.elem.clone(),
                                            colon_token: pointer_type.colon_token,
                                        })
                                    }
                                };
                                *expr_cast.expr = Box::new(new_type);
                            }
                        } else if let Expr::Call(expr_call) = expr {
                            if let Expr::Path(expr_path) = *expr_call.func {
                                if expr_path.path.segments.last().unwrap().ident == "transmute" {
                                    let new_type = match expr_call.args.first().unwrap() {
                                        syn::Expr::Cast(expr_cast) => {
                                            if let Type::Pointer(pointer_type) = *expr_cast.expr {
                                                Type::Pointer(syn::PointerType {
                                                    mutability: syn::Mutability::Mut,
                                                    elem: pointer_type.elem.clone(),
                                                    colon_token: pointer_type.colon_token,
                                                })
                                            } else {
                                                *expr_cast.expr.clone()
                                            }
                                        }
                                        _ => unreachable!(),
                                    };
                                    expr_call.args.first_mut().unwrap().make_clone();
                                    *expr_call.args.first_mut().unwrap() = Box::new(new_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces pointer casts with other pointer casts of compatible types to trigger bugs related to pointer provenance and transmutation."
    }
}