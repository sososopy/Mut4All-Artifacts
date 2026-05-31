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

pub struct Alter_Trait_Impls_With_Const_262;

impl Mutator for Alter_Trait_Impls_With_Const_262 {
    fn name(&self) -> &str {
        "Alter_Trait_Impls_With_Const_262"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() && item_impl.trait_.as_ref().unwrap().0.is_none() {
                    item_impl.trait_.as_mut().unwrap().0 = Some(token::Not {
                        spans: [Span::call_site()],
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets impl blocks that implement a trait without using const. It modifies these impl blocks to attempt const trait implementation by adding the const keyword before the trait name. This transformation is designed to expose compiler weaknesses related to const trait impls, such as errors involving trait constness, experimental feature usage, and potential ICEs during trait resolution and code generation."
    }
}