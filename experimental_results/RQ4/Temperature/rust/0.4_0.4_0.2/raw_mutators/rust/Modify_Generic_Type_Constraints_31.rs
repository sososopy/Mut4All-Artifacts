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

pub struct Modify_Generic_Type_Constraints_31;

impl Mutator for Modify_Generic_Type_Constraints_31 {
    fn name(&self) -> &str {
        "Modify_Generic_Type_Constraints_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let action = rng.gen_range(0..3);
                        match action {
                            0 => {
                                // Add a conflicting trait bound
                                type_param.bounds.push(parse_quote!(Clone));
                            }
                            1 => {
                                // Remove an existing trait bound
                                type_param.bounds.clear();
                            }
                            2 => {
                                // Alter the existing trait bound
                                type_param.bounds.clear();
                                type_param.bounds.push(parse_quote!(IntoIterator));
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the constraints on generic type parameters in function signatures. By adding, removing, or altering trait bounds, it introduces potential inconsistencies or conflicts that can expose issues related to trait resolution and type inference in the Rust type system."
    }
}