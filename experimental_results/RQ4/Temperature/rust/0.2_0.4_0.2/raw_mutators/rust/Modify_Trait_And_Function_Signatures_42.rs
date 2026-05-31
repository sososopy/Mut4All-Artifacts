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

pub struct Modify_Trait_And_Function_Signatures_42;

impl Mutator for Modify_Trait_And_Function_Signatures_42 {
    fn name(&self) -> &str {
        "Modify_Trait_And_Function_Signatures_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Trait(trait_item) => {
                    trait_item.generics.params.push(parse_quote!('a));
                    trait_item.generics.params.push(parse_quote!(T));
                }
                Item::Fn(func) => {
                    if func.sig.ident != "main" {
                        func.sig.generics.params.push(parse_quote!('a));
                        func.sig.generics.params.push(parse_quote!(T));
                        if let Some(first_param) = func.sig.inputs.first_mut() {
                            if let FnArg::Typed(pat_type) = first_param {
                                pat_type.ty = Box::new(parse_quote!(T));
                            }
                        }
                        func.sig.output = ReturnType::Type(
                            Default::default(),
                            Box::new(parse_quote!(&'a T)),
                        );
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}