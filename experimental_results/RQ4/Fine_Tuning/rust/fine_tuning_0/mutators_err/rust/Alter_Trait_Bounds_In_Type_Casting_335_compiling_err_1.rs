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

pub struct Alter_Trait_Bounds_In_Type_Casting_335;

impl Mutator for Alter_Trait_Bounds_In_Type_Casting_335 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_In_Type_Casting_335"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Cast(expr_cast) = &**expr {
                                if let Type::TraitObject(type_trait_object) = &*expr_cast.ty {
                                    let mut bounds = type_trait_object.bounds.clone();
                                    let mut rng = thread_rng();
                                    let choice = rng.gen_range(0..=2);

                                    match choice {
                                        0 => {
                                            bounds.push(TypeParamBound::Trait(TraitBound {
                                                paren_token: None,
                                                modifier: TraitBoundModifier::None,
                                                lifetimes: None,
                                                path: syn::Path::from(Ident::new("Clone", Span::call_site())),
                                            }));
                                        }
                                        1 => {
                                            if bounds.len() > 1 {
                                                bounds.pop();
                                            }
                                        }
                                        2 => {
                                            if let Some(bound) = bounds.iter_mut().find(|b| matches!(b, TypeParamBound::Trait(_))) {
                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                    trait_bound.path.segments[0].ident = Ident::new("Sync", Span::call_site());
                                                }
                                            }
                                        }
                                        _ => {}
                                    }

                                    expr_cast.ty = Box::new(Type::TraitObject(TypeTraitObject {
                                        dyn_token: type_trait_object.dyn_token.clone(),
                                        bounds,
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type casting expressions involving `dyn* Trait` with additional trait bounds. It randomly adds, removes, or replaces trait bounds in these expressions, testing the compiler's handling of trait resolution and type checking in complex trait object scenarios."
    }
}