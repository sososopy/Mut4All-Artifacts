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

pub struct Unused_Generic_Type_Parameter_80;

impl Mutator for Unused_Generic_Type_Parameter_80 {
    fn name(&self) -> &str {
        "Unused_Generic_Type_Parameter_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.generics.params.is_empty() {
                    let new_generic_param: GenericParam = parse_quote! { U };
                    item_fn.sig.generics.params.push(new_generic_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function declarations with generic parameters. It introduces an unused generic type parameter to the function signature, ensuring that the new parameter does not appear in the function body, parameters, or return type. This can expose potential issues in the compiler's handling of unused generic parameters."
    }
}