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

pub struct Generator_Yield_Return_Mismatch_165;

impl Mutator for Generator_Yield_Return_Mismatch_165 {
    fn name(&self) -> &str {
        "Generator_Yield_Return_Mismatch_165"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                let path = &trait_bound.path;
                                let is_generator = path.segments.iter().any(|seg| seg.ident == "Generator");
                                if is_generator {
                                    let mut yield_ty = None;
                                    let mut return_ty = None;
                                    for seg in &path.segments {
                                        if seg.ident == "Generator" {
                                            if let syn::PathArguments::AngleBracketed(args) = &seg.arguments {
                                                for arg in &args.args {
                                                    if let syn::GenericArgument::AssocType(ty) = arg {
                                                        if ty.ident == "Yield" {
                                                            yield_ty = Some(ty.ty);
                                                        } else if ty.ident == "Return" {
                                                            return_ty = Some(ty.ty);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if let Some(yield_ty) = yield_ty {
                                        for stmt in &mut func.block.stmts {
                                            if let syn::Stmt::Expr(expr, _) = stmt {
                                                if let syn::Expr::Yield(yield_expr) = &mut **expr {
                                                    let new_expr = match yield_ty {
                                                        syn::Type::Tuple(t) if t.elems.is_empty() => {
                                                            parse_quote!(1i32)
                                                        },
                                                        syn::Type::Path(p) => {
                                                            if p.path.segments.iter().any(|seg| seg.ident == "i32") {
                                                                parse_quote!("string")
                                                            } else if p.path.segments.iter().any(|seg| seg.ident == "str") {
                                                                parse_quote!(1i32)
                                                            } else {
                                                                parse_quote!(())
                                                            }
                                                        },
                                                        _ => {
                                                            parse_quote!(())
                                                        },
                                                    };
                                                    *yield_expr.expr = new_expr;
                                                } else if let syn::Expr::Return(return_expr) = &mut **expr {
                                                    if let Some(return_ty) = return_ty {
                                                        let new_expr = match return_ty {
                                                            syn::Type::Tuple(t) if t.elems.is_empty() => {
                                                                parse_quote!(1i32)
                                                            },
                                                            syn::Type::Path(p) => {
                                                                if p.path.segments.iter().any(|seg| seg.ident == "i32") {
                                                                    parse_quote!("string")
                                                                } else if p.path.segments.iter().any(|seg| seg.ident == "str") {
                                                                    parse_quote!(1i32)
                                                                } else {
                                                                    parse_quote!(())
                                                                }
                                                            },
                                                            _ => {
                                                                parse_quote!(())
                                                            },
                                                        };
                                                        return_expr.expr = Some(new_expr);
                                                    }
                                                }
                                            }
                                        }
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
        ""
    }
}