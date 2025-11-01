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

pub struct Modify_Const_Expressions_In_Generic_Constraints_65;

impl Mutator for Modify_Const_Expressions_In_Generic_Constraints_65 {
    fn name(&self) -> &str {
        "Modify_Const_Expressions_In_Generic_Constraints_65"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Block(expr_block) = expr {
                                                        let new_expr: Expr = parse_quote!({
                                                            from_options(ASSUME_ALIGNMENT, some_other_condition(), ASSUME_SAFETY, ASSUME_VALIDITY)
                                                        });
                                                        *expr_block = syn::ExprBlock {
                                                            attrs: vec![],
                                                            label: None,
                                                            block: syn::Block {
                                                                brace_token: syn::token::Brace { span: Span::call_site() },
                                                                stmts: vec![Stmt::Expr(new_expr)],
                                                            },
                                                        };
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
        "The mutation operator targets constant expressions within generic constraints in function signatures using const generics. It identifies such expressions and introduces runtime logic, replacing constant expressions with non-constant ones to enforce runtime evaluation. This is achieved by injecting a non-constant function call, ensuring the expression cannot be resolved at compile time."
    }
}

fn some_other_condition() -> bool {
    use std::time::{SystemTime, UNIX_EPOCH};
    let now = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
    now.as_secs() % 2 == 0
}