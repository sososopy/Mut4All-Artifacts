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

pub struct Substitute_Const_Generic_In_Trait_Bound_89;

impl Mutator for Substitute_Const_Generic_In_Trait_Bound_89 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_In_Trait_Bound_89"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let impl_const_params: Vec<Ident> = item_impl.generics.params.iter()
                    .filter_map(|param| match param {
                        GenericParam::Const(const_param) => Some(const_param.ident.clone()),
                        _ => None,
                    })
                    .collect();

                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(pred_type) = predicate {
                                    let mut new_bounds = Punctuated::new();
                                    for bound in &pred_type.bounds {
                                        match bound {
                                            TypeParamBound::Trait(trait_bound) => {
                                                let mut new_path = trait_bound.path.clone();
                                                let mut modified = false;
                                                for segment in &mut new_path.segments {
                                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                        for arg in &mut args.args {
                                                            if let GenericArgument::Const(const_arg) = arg {
                                                                if let Expr::Path(expr_path) = &const_arg {
                                    if expr_path.path.segments.len() == 1 {
                                        let ident = &expr_path.path.segments[0].ident;
                                        if impl_const_params.contains(ident) {
                                            let mut rng = thread_rng();
                                            if impl_const_params.len() > 1 {
                                                let other_consts = impl_const_params.iter()
                                                    .filter(|&c| c != ident)
                                                    .collect::<Vec<_>>();
                                                if !other_consts.is_empty() {
                                                    let chosen = other_consts.choose(&mut rng).unwrap();
                                                    *const_arg = parse_quote!(#chosen);
                                                    modified = true;
                                                }
                                            } else {
                                                let possible_literals = vec![
                                                    parse_quote!(0),
                                                    parse_quote!(1),
                                                    parse_quote!(2),
                                                ];
                                                let chosen = possible_literals.choose(&mut rng).unwrap();
                                                *const_arg = chosen.clone();
                                                modified = true;
                                            }
                                        }
                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                if modified {
                                                    new_bounds.push(TypeParamBound::Trait(TraitBound {
                                                        paren_token: trait_bound.paren_token,
                                                        modifier: trait_bound.modifier,
                                                        lifetimes: trait_bound.lifetimes,
                                                        path: new_path,
                                                    }));
                                                } else {
                                                    new_bounds.push(bound.clone());
                                                }
                                            }
                                            _ => new_bounds.push(bound.clone()),
                                        }
                                    }
                                    pred_type.bounds = new_bounds;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds within impl blocks where a const generic parameter is used as a type argument within the bound. It substitutes the const generic parameter in the trait bound with another const parameter from the same impl block or a simple literal (like 0,1,2) if no other suitable const exists. This transformation alters the const argument in trait bounds while keeping the rest of the function signature intact, potentially creating mismatched const arguments that expose bugs in compiler's handling of const generic arguments in trait bounds during type checking or const evaluation."
    }
}