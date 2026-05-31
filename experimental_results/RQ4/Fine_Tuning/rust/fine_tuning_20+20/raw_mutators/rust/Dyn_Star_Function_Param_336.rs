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

pub struct Dyn_Star_Function_Param_336;

impl Mutator for Dyn_Star_Function_Param_336 {
    fn name(&self) -> &str {
        "Dyn_Star_Function_Param_336"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found = false;
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(segment) = type_path.path.segments.last() {
                                if let PathArguments::AngleBracketed(angle_args) =
                                    &segment.arguments
                                {
                                    for arg in &angle_args.args {
                                        if let GenericArgument::Constraint(constraint) = arg {
                                            if constraint.bounds.iter().any(|bound| {
                                                matches!(
                                                    bound,
                                                    TypeParamBound::Trait(TraitBound {
                                                        modifier: TraitBoundModifier::Maybe(..),
                                                        ..
                                                    })
                                                )
                                            }) {
                                                found = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if found {
                    let new_stmt: Stmt = parse_quote! {
                        let _: dyn* SomeTrait = item;
                    };
                    item_fn.block.stmts.insert(0, new_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with parameters that have trait bounds, especially those marked with `?Sized`. It introduces a `dyn*` pointer within the function body to stress-test the `dyn_star` feature, aiming to uncover compiler bugs related to trait object handling and dynamic dispatch."
    }
}