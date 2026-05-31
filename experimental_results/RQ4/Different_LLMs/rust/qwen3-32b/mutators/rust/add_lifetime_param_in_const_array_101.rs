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

pub struct Add_Lifetime_Param_In_Const_Array_101;

impl Mutator for Add_Lifetime_Param_In_Const_Array_101 {
    fn name(&self) -> &str {
        "Add_Lifetime_Param_In_Const_Array_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_const_fns = HashSet::new();

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    let has_lifetime = func.sig.generics.params.iter().any(|param| {
                        matches!(param, GenericParam::Lifetime(_))
                    });
                    if !has_lifetime {
                        let lifetime = Lifetime::new("'a", Span::call_site());
                        func.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            lifetime: lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                            attrs: Vec::new(),
                        }));
                        
                        let bounded_ty = Type::Reference(TypeReference {
                            and_token: token::And::default(),
                            lifetime: Some(lifetime.clone()),
                            mutability: None,
                            elem: Box::new(Type::Tuple(TypeTuple {
                                paren_token: token::Paren::default(),
                                elems: Punctuated::new(),
                            })),
                        });
                        let trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath::from(Ident::new("Sized", Span::call_site())),
                        };
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(trait_bound));
                        let predicate = WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty,
                            colon_token: token::Colon::default(),
                            bounds,
                        });
                        let mut where_clause = func.sig.generics.where_clause.take();
                        if let Some(ref mut clause) = where_clause {
                            clause.predicates.push(predicate);
                        } else {
                            where_clause = Some(WhereClause {
                                where_token: token::Where::default(),
                                predicates: Punctuated::from_iter(vec![predicate]),
                            });
                        }
                        func.sig.generics.where_clause = where_clause;
                        modified_const_fns.insert(func.sig.ident.to_string());
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let has_lifetime = func.sig.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Lifetime(_))
                });
                if has_lifetime {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Local(local) = stmt {
                            if let Pat::Type(pat_type) = &mut local.pat {
                                let ty = &mut pat_type.ty;
                                if let Type::Array(array_type) = &mut **ty {
                                    if let Expr::Call(call_expr) = &array_type.len {
                                        if let Expr::Path(path_expr) = &*call_expr.func {
                                            let function_name = path_expr.path.segments[0].ident.to_string();
                                            if modified_const_fns.contains(&function_name) {
                                                let new_call = parse_quote!(#function_name::<'a>());
                                                array_type.len = new_call;
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
        "The mutation operator introduces a lifetime parameter into const functions lacking them, adds a where clause for reference and Sized, and modifies array size expressions in functions with lifetimes to use the updated const function with the new lifetime. This increases complexity in const evaluation and region inference, potentially exposing bugs in array size evaluation and lifetime propagation."
    }
}