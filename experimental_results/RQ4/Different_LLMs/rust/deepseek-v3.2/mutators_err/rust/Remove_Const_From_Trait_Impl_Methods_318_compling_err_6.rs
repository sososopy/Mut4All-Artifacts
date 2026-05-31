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

pub struct Remove_Const_From_Trait_Impl_Methods_318;

impl Mutator for Remove_Const_From_Trait_Impl_Methods_318 {
    fn name(&self) -> &str {
        "Remove_Const_From_Trait_Impl_Methods_318"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            if func.sig.constness.is_some() {
                                func.sig.constness = None;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations where methods are declared as const. It removes the const keyword from method signatures within impl blocks while the trait definition still requires const methods. This creates a mismatch between trait declaration and implementation, potentially triggering compiler errors or ICEs related to const trait validation and effect checking. The operator exploits the compiler's handling of const trait methods and their implementations."
    }
}