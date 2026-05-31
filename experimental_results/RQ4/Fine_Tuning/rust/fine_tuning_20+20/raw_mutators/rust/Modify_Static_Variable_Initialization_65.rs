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

pub struct Modify_Static_Variable_Initialization_65;

impl Mutator for Modify_Static_Variable_Initialization_65 {
    fn name(&self) -> &str {
        "Modify_Static_Variable_Initialization_65"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Mod(item_mod) = item {
                if let Some((_, items)) = &mut item_mod.content {
                    for item in items {
                        if let Item::Static(item_static) = item {
                            if let Type::Path(type_path) = &*item_static.ty {
                                if let Some(last_segment) = type_path.path.segments.last() {
                                    let type_name = last_segment.ident.to_string();
                                    if type_name == "AtomicUsize" {
                                        item_static.expr = Box::new(parse_quote! {
                                            AtomicUsize::new(0.0)
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
    fn chain_of_thought(&self) -> &str {
        "This mutator targets static variables of type `AtomicUsize` within modules and modifies their initialization expression to introduce a type mismatch. By replacing the integer initializer with a floating-point number, it aims to trigger constant evaluation errors and test the compiler's handling of type mismatches in static contexts."
    }
}