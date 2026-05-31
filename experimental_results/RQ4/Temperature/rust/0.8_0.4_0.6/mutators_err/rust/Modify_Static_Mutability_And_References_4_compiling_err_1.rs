use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Static_Mutability_And_References_4;

impl Mutator for Modify_Static_Mutability_And_References_4 {
    fn name(&self) -> &str {
        "Modify_Static_Mutability_And_References_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                // Toggle mutability
                if item_static.mutability.is_some() {
                    item_static.mutability = None;
                } else {
                    item_static.mutability = Some(token::Mut {
                        span: Span::call_site(),
                    });
                }

                // Modify reference types if applicable
                if let Type::Path(type_path) = &mut *item_static.ty {
                    if let Some(last_segment) = type_path.path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(angle_args) = &mut last_segment.arguments {
                            for arg in &mut angle_args.args {
                                if let GenericArgument::Type(Type::Reference(type_ref)) = arg {
                                    // Toggle reference mutability
                                    if type_ref.mutability.is_some() {
                                        type_ref.mutability = None;
                                    } else {
                                        type_ref.mutability = Some(token::Mut {
                                            span: Span::call_site(),
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
        "The mutation operator targets static variables, toggling their mutability and modifying reference types. By altering static mutability and reference mutability, it aims to uncover issues in memory handling and reference stability, especially during constant evaluation in the Rust compiler."
    }
}