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
    Fields, Field, *,
};

use crate::mutator::Mutator;

pub struct Introduce_Redundant_Lifetime_Annotations_19;

impl Mutator for Introduce_Redundant_Lifetime_Annotations_19 {
    fn name(&self) -> &str {
        "Introduce_Redundant_Lifetime_Annotations_19"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if !item_fn.sig.generics.params.is_empty() {
                        let new_lifetime: LifetimeParam = parse_quote!('b);
                        item_fn.sig.generics.params.push(GenericParam::Lifetime(new_lifetime));
                        if let Some(FnArg::Typed(pat_type)) = item_fn.sig.inputs.first_mut() {
                            let redundant_param: PatType = parse_quote!(_redundant: &'b str);
                            item_fn.sig.inputs.push(FnArg::Typed(redundant_param));
                        }
                    }
                }
                Item::Struct(item_struct) => {
                    if !item_struct.generics.params.is_empty() {
                        let new_lifetime: LifetimeParam = parse_quote!('b);
                        item_struct.generics.params.push(GenericParam::Lifetime(new_lifetime));
                        let redundant_field: Field = parse_quote!(_redundant: &'b str);
                        if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                            fields_named.named.push(redundant_field);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator introduces redundant lifetime annotations to function signatures and struct definitions that already have lifetime parameters. By adding an extra, unnecessary lifetime and corresponding parameter or field, it tests the compiler's handling of lifetime resolution and aims to expose potential issues related to internal compiler errors involving lifetimes."
    }
}