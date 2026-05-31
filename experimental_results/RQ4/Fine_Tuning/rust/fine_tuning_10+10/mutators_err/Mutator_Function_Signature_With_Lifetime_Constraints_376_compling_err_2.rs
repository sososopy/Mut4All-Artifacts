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

pub struct Mutator_Function_Signature_With_Lifetime_Constraints_376;

impl Mutator for Mutator_Function_Signature_With_Lifetime_Constraints_376 {
    fn name(&self) -> &str {
        "Mutator_Function_Signature_With_Lifetime_Constraints_376"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut lifetime_replaced = false;
                let mut new_lifetime = Lifetime::new("'b", Span::call_site());

                for generic_param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(lifetime_param) = generic_param {
                        lifetime_replaced = true;
                        new_lifetime = Lifetime::new("'b", lifetime_param.lifetime.span());
                        lifetime_param.lifetime = new_lifetime.clone();
                        break;
                    }
                }

                if lifetime_replaced {
                    if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                        for where_predicate in &mut where_clause.predicates {
                            if let syn::WherePredicate::Type(predicate_type) = where_predicate {
                                for bound in &mut predicate_type.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(lifetimes) = &mut trait_bound.lifetimes {
                                            for lifetime in &mut lifetimes.lifetimes {
                                                lifetime.lifetime = new_lifetime.clone();
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
        "This mutator targets functions with generic parameters and lifetime constraints. It identifies functions with a generic parameter that includes a lifetime constraint and modifies the lifetime in the function signature. The mutation introduces a new lifetime parameter or alters an existing one, affecting the trait bounds. This change can expose issues in how the Rust compiler resolves lifetimes in relation to trait bounds, potentially leading to internal compiler errors."
    }
}