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

pub struct Modify_Trait_Bound_With_Const_Trait_Implementation_196;

impl Mutator for Modify_Trait_Bound_With_Const_Trait_Implementation_196 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Const_Trait_Implementation_196"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                    for arg in args.args.iter_mut() {
                                        if let GenericArgument::Constraint(constraint) = arg {
                                            let mut new_bounds = Punctuated::new();
                                            for bound in &constraint.bounds {
                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                    let mut new_trait_bound = trait_bound.clone();
                                                    new_trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("Clone", Span::call_site());
                                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                                } else {
                                                    new_bounds.push(bound.clone());
                                                }
                                            }
                                            *arg = GenericArgument::Constraint(syn::Constraint {
                                                bounds: new_bounds,
                                                ..constraint.clone()
                                            });
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
        "The mutation operator identifies function definitions with a trait bound specifying a trait with a const trait implementation. It modifies the trait bound to include a different const trait, such as changing `F: ~const Fn()` to `F: ~const Clone`. This mutation targets const trait bounds and modifies the trait to another const-compatible trait, potentially triggering compiler bugs related to const trait handling and resolution."
    }
}