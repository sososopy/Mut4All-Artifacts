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

pub struct Mutator_Function_With_Trait_Bound_397;

impl Mutator for Mutator_Function_With_Trait_Bound_397 {
    fn name(&self) -> &str {
        "Mutator_Function_With_Trait_Bound_397"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_stmts = vec![];
                for input in &func.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Path(TypePath { path, .. }) = &**ty {
                            for segment in &path.segments {
                                if let PathArguments::AngleBracketed(ref args) = segment.arguments {
                                    for arg in &args.args {
                                        if let GenericArgument::Type(Type::TraitObject(trait_object)) = arg {
                                            for bound in &trait_object.bounds {
                                                if let TypeParamBound::Trait(TraitBound { path, modifier, .. }) = bound {
                                                    if matches!(modifier, TraitBoundModifier::Maybe(_)) {
                                                        let trait_ident = &path.segments.last().unwrap().ident;
                                                        let stmt: Stmt = parse_quote! {
                                                            let _: dyn #trait_ident = item;
                                                        };
                                                        new_stmts.push(stmt);
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
                func.block.stmts.splice(0..0, new_stmts);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies functions with parameters that have trait bounds, especially those marked with `?Sized`. It introduces a new local variable of type `dyn Trait` within the function body where `Trait` matches the trait bound in the function signature. This mutation leverages the use of `dyn` pointers, increasing the likelihood of interacting with the bug related to the `dyn_star` feature."
    }
}