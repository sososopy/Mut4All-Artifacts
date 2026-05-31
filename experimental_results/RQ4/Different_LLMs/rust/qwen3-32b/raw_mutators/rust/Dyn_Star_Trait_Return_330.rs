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

pub struct Dyn_Star_Trait_Return_330;

impl Mutator for Dyn_Star_Trait_Return_330 {
    fn name(&self) -> &str {
        "Dyn_Star_Trait_Return_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_trait = false;
                        let mut has_lifetime = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if !trait_bound.path.segments.is_empty() {
                                    has_trait = true;
                                }
                                if trait_bound.lifetimes.is_some() {
                                    has_lifetime = true;
                                }
                            }
                        }
                        if has_trait && has_lifetime {
                            if let Block::Block(block) = &item_fn.block {
                                if let Some(stmt) = block.stmts.last() {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        let is_coercion = if let Expr::Cast(expr_cast) = expr {
                                            if let Type::Infer(_) = *expr_cast.ty {
                                                true
                                            } else {
                                                false
                                            }
                                        } else {
                                            false
                                        };
                                        if !is_coercion {
                                            let mut new_block = item_fn.block.clone();
                                            if let Some(last_stmt) = new_block.stmts.last_mut() {
                                                if let Stmt::Expr(expr, _) = last_stmt {
                                                    let original_expr = expr.clone();
                                                    let new_expr = parse_quote!(#original_expr as _);
                                                    *expr = new_expr;
                                                }
                                            }
                                            item_fn.block = Box::new(new_block);
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