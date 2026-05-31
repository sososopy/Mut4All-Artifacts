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

pub struct Modify_For_Binder_Const_Default_435;

impl Mutator for Modify_For_Binder_Const_Default_435 {
    fn name(&self) -> &str {
        "Modify_For_Binder_Const_Default_435"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::TraitObject(trait_object) = &mut predicate_type.bounded_ty {
                                if let Some(bound_lifetimes) = &mut trait_object.bounds.first_mut() {
                                    if let TypeParamBound::Trait(trait_bound) = bound_lifetimes {
                                        if let Some(for_lifetimes) = &mut trait_bound.lifetimes {
                                            self.mutate_for_binder(for_lifetimes, &mut rng);
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
        "The mutation operator targets where clauses containing for binders with const parameters that have default values. It modifies these default values by generating new constant expressions that use the bound const parameters. The new expressions include arithmetic operations, conditional expressions, and block expressions, all referencing previously bound const parameters. This transformation stresses the compiler's handling of const generics with default values in for binders, potentially exposing missing key errors in THIR construction when evaluating complex constant expressions involving bound parameters."
    }
}

impl Modify_For_Binder_Const_Default_435 {
    fn mutate_for_binder(&self, for_lifetimes: &mut BoundLifetimes, rng: &mut impl Rng) {
        let mut const_params_with_defaults = Vec::new();
        let mut bound_const_idents = Vec::new();
        
        for param in &for_lifetimes.lifetimes.params {
            if let GenericParam::Const(const_param) = param {
                bound_const_idents.push(const_param.ident.clone());
                if const_param.default.is_some() {
                    const_params_with_defaults.push(const_param);
                }
            }
        }
        
        if const_params_with_defaults.is_empty() || bound_const_idents.is_empty() {
            return;
        }
        
        for param in &mut for_lifetimes.lifetimes.params {
            if let GenericParam::Const(const_param) = param {
                if const_param.default.is_some() {
                    let chosen_ident = bound_const_idents.choose(rng).unwrap();
                    let pattern = rng.gen_range(0..4);
                    
                    let new_expr = match pattern {
                        0 => self.create_arithmetic_expr(chosen_ident),
                        1 => self.create_conditional_expr(chosen_ident),
                        2 => self.create_block_expr(chosen_ident),
                        3 => self.modify_existing_block(const_param.default.as_ref().unwrap(), chosen_ident),
                        _ => unreachable!(),
                    };
                    
                    const_param.default = Some(new_expr);
                }
            }
        }
    }
    
    fn create_arithmetic_expr(&self, ident: &Ident) -> Expr {
        let op = if rand::thread_rng().gen_bool(0.5) {
            parse_quote!(+)
        } else {
            parse_quote!(*)
        };
        let rhs = rand::thread_rng().gen_range(1..=5);
        
        parse_quote!({
            #ident #op #rhs
        })
    }
    
    fn create_conditional_expr(&self, ident: &Ident) -> Expr {
        parse_quote!({
            if #ident > 0 { #ident } else { 1 }
        })
    }
    
    fn create_block_expr(&self, ident: &Ident) -> Expr {
        parse_quote!({
            let x = #ident;
            x + 1
        })
    }
    
    fn modify_existing_block(&self, existing_expr: &Expr, ident: &Ident) -> Expr {
        if let Expr::Block(block) = existing_expr {
            let mut new_block = block.clone();
            if let Some(last_stmt) = new_block.stmts.last_mut() {
                if let Stmt::Expr(expr, _) = last_stmt {
                    if let Expr::Binary(binary) = expr {
                        if let Expr::Path(path) = &*binary.left {
                            if path.path.segments.last().unwrap().ident == *ident {
                                binary.op = if rand::thread_rng().gen_bool(0.5) {
                                    parse_quote!(-)
                                } else {
                                    parse_quote!(/)
                                };
                                if let Expr::Lit(lit) = &*binary.right {
                                    if let Lit::Int(int) = &lit.lit {
                                        let new_val = int.base10_parse::<u64>().unwrap() + 1;
                                        binary.right = Box::new(parse_quote!(#new_val));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Expr::Block(new_block)
        } else {
            self.create_block_expr(ident)
        }
    }
}