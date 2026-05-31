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

pub struct Modify_Struct_Trait_Bound_53;

impl Mutator for Modify_Struct_Trait_Bound_53 {
    fn name(&self) -> &str {
        "Modify_Struct_Trait_Bound_53"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_trait_bound = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            has_trait_bound = true;
                            break;
                        }
                    }
                }
                if !has_trait_bound {
                    continue;
                }
                let mut new_generics = item_struct.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            type_param.bounds.clear();
                            type_param.bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("Clone", Span::call_site())),
                            }));
                        }
                    }
                }
                item_struct.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with type parameters that have trait bounds. It replaces existing trait bounds with the `Clone` trait. This transformation exploits the reliance on specific trait bounds in generic structs, potentially leading to compilation errors if the types used with the struct do not satisfy the new `Clone` bound. It tests the compiler's ability to enforce trait constraints and handle type mismatches in generic contexts."
    }
}