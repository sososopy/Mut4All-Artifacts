use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParamConst, GenericParamType, Ident,
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

pub struct Swap_Defaulted_Generic_Parameters_86;

impl Mutator for Swap_Defaulted_Generic_Parameters_86 {
    fn name(&self) -> &str {
        "Swap_Defaulted_Generic_Parameters_86"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(s) => self.swap_generics(&mut s.generics),
                Item::Enum(e) => self.swap_generics(&mut e.generics),
                Item::Union(u) => self.swap_generics(&mut u.generics),
                Item::Trait(t) => self.swap_generics(&mut t.generics),
                Item::Impl(i) => self.swap_generics(&mut i.generics),
                Item::Fn(f) => self.swap_generics(&mut f.sig.generics),
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the positions of generic parameters with defaults and those without defaults in a generic parameter list. It applies to struct, enum, union, trait, impl, and function definitions where there is at least one generic parameter with a default value and at least one generic parameter without a default value. The operator selects a generic parameter with a default value that is not in the last position among generic parameters without defaults and swaps it with the immediately following generic parameter that does not have a default value. This transformation violates the constraint that generic parameters with defaults must be trailing, potentially triggering compiler bugs related to generic parameter ordering, especially in contexts involving const generics and default values."
    }
}

impl Swap_Defaulted_Generic_Parameters_86 {
    fn swap_generics(&self, generics: &mut Generics) {
        let params = &mut generics.params;
        let mut indices_with_default = Vec::new();
        let mut indices_without_default = Vec::new();

        for (i, param) in params.iter().enumerate() {
            match param {
                GenericParam::Type(t) => {
                    if t.default.is_some() {
                        indices_with_default.push(i);
                    } else {
                        indices_without_default.push(i);
                    }
                }
                GenericParam::Const(c) => {
                    if c.default.is_some() {
                        indices_with_default.push(i);
                    } else {
                        indices_without_default.push(i);
                    }
                }
                GenericParam::Lifetime(_) => {}
            }
        }

        if indices_with_default.is_empty() || indices_without_default.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let candidate_pairs = indices_with_default
            .iter()
            .filter(|&d_idx| {
                let next_idx = d_idx + 1;
                indices_without_default.contains(&next_idx)
            })
            .collect::<Vec<_>>();

        if candidate_pairs.is_empty() {
            return;
        }

        let selected_idx = *candidate_pairs.choose(&mut rng).unwrap();
        let next_idx = selected_idx + 1;

        let mut param_vec: Vec<GenericParam> = params.iter().cloned().collect();
        param_vec.swap(*selected_idx, next_idx);
        *params = param_vec.into_iter().collect();
    }
}