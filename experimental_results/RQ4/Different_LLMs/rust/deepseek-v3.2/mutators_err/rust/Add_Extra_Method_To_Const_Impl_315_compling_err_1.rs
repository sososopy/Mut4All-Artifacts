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

pub struct Add_Extra_Method_To_Const_Impl_315;

impl Mutator for Add_Extra_Method_To_Const_Impl_315 {
    fn name(&self) -> &str {
        "Add_Extra_Method_To_Const_Impl_315"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut counter = 0;
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                if !item_impl.constness.is_some() {
                    continue;
                }
                counter += 1;
                let method_name = Ident::new(&format!("missing_method_{}", counter), Span::call_site());
                let new_method: ImplItem = parse_quote! {
                    fn #method_name() -> () {}
                };
                item_impl.items.push(new_method);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const trait implementations and adds an extra method not declared in the trait. This creates a mismatch between trait definition and implementation, potentially triggering query cycles or deadlocks during const checking. The added method has a simple signature (no parameters, returns unit) and a unique name to avoid conflicts."
    }
}