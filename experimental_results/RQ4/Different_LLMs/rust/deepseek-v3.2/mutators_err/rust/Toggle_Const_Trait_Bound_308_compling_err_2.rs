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

pub struct Toggle_Const_Trait_Bound_308;

impl Mutator for Toggle_Const_Trait_Bound_308 {
    fn name(&self) -> &str {
        "Toggle_Const_Trait_Bound_308"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    mutate_generics(&mut func.sig.generics, &mut rng);
                }
                Item::Impl(impl_item) => {
                    mutate_generics(&mut impl_item.generics, &mut rng);
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Fn(func) = impl_item {
                            mutate_generics(&mut func.sig.generics, &mut rng);
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    mutate_generics(&mut struct_item.generics, &mut rng);
                }
                Item::Enum(enum_item) => {
                    mutate_generics(&mut enum_item.generics, &mut rng);
                }
                Item::Trait(trait_item) => {
                    mutate_generics(&mut trait_item.generics, &mut rng);
                }
                Item::Type(type_item) => {
                    mutate_generics(&mut type_item.generics, &mut rng);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait bounds that involve the ~const modifier. It scans for trait bounds in generic parameters, where clauses, and impl headers. When a bound with ~const modifier is found, it randomly toggles between ~const, const, or plain trait bound (removing modifier). For bounds without ~const, it may add ~const if the trait is likely const. This transformation creates mismatches in const trait implementation checking, targeting compiler bugs in const trait lowering and host param id handling, especially in non-const contexts."
    }
}

fn mutate_generics(generics: &mut Generics, rng: &mut impl Rng) {
    for param in &mut generics.params {
        if let GenericParam::Type(type_param) = param {
            mutate_bounds(&mut type_param.bounds, rng);
        }
    }
    if let Some(where_clause) = &mut generics.where_clause {
        for predicate in &mut where_clause.predicates {
            if let WherePredicate::Type(pred_type) = predicate {
                mutate_bounds(&mut pred_type.bounds, rng);
            }
        }
    }
}

fn mutate_bounds(bounds: &mut Punctuated<TypeParamBound, Plus>, rng: &mut impl Rng) {
    for bound in bounds.iter_mut() {
        if let TypeParamBound::Trait(trait_bound) = bound {
            match trait_bound.modifier {
                TraitBoundModifier::Maybe(_) => {
                    let choice = rng.gen_range(0..3);
                    match choice {
                        0 => trait_bound.modifier = TraitBoundModifier::Const(_),
                        1 => trait_bound.modifier = TraitBoundModifier::None,
                        _ => {}
                    }
                }
                TraitBoundModifier::Const(_) => {
                    let choice = rng.gen_range(0..3);
                    match choice {
                        0 => trait_bound.modifier = TraitBoundModifier::Maybe(syn::token::Question::default()),
                        1 => trait_bound.modifier = TraitBoundModifier::None,
                        _ => {}
                    }
                }
                TraitBoundModifier::None => {
                    let choice = rng.gen_range(0..2);
                    if choice == 0 {
                        trait_bound.modifier = TraitBoundModifier::Maybe(syn::token::Question::default());
                    } else {
                        trait_bound.modifier = TraitBoundModifier::Const(syn::token::Const::default());
                    }
                }
                _ => {}
            }
        }
    }
}