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

pub struct Replace_Trait_Bound_With_Const_Trait_311;

impl Mutator for Replace_Trait_Bound_With_Const_Trait_311 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Const_Trait_311"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                // Skip const functions
                if func.sig.constness.is_some() {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(trait_object) = &mut *pat_type.ty {
                            // Remove dyn_token
                            trait_object.dyn_token = None;
                            // Modify each bound to have ~const modifier
                            for bound in &mut trait_object.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    // Use the correct variant for `~const` modifier
                                    trait_bound.modifier = TraitBoundModifier::Const;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid trait bounds in function parameters with `~const Trait`, omitting `dyn` if present. This creates invalid trait bounds that bypass standard syntax rules, testing the compiler's handling of mixed const/lifetime parameters and potentially exposing assertion failures in trait bound analysis."
    }
}