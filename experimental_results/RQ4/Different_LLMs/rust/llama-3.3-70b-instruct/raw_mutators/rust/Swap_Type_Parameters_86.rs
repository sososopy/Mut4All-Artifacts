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

pub struct Swap_Type_Parameters_86;

impl Mutator for Swap_Type_Parameters_86 {
    fn name(&self) -> &str {
        "Swap_Type_Parameters_86"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.params.len() >= 2 {
                    let mut params: Vec<_> = struct_item.generics.params.iter().collect();
                    let idx1 = thread_rng().gen_range(0..params.len());
                    let idx2 = thread_rng().gen_range(0..params.len());
                    params.swap(idx1, idx2);
                    struct_item.generics.params = params.into_iter().collect();
                }
            } else if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.len() >= 2 {
                    let mut params: Vec<_> = func.sig.generics.params.iter().collect();
                    let idx1 = thread_rng().gen_range(0..params.len());
                    let idx2 = thread_rng().gen_range(0..params.len());
                    params.swap(idx1, idx2);
                    func.sig.generics.params = params.into_iter().collect();
                }
            } else if let syn::Item::Impl(impl_item) = item {
                if impl_item.generics.params.len() >= 2 {
                    let mut params: Vec<_> = impl_item.generics.params.iter().collect();
                    let idx1 = thread_rng().gen_range(0..params.len());
                    let idx2 = thread_rng().gen_range(0..params.len());
                    params.swap(idx1, idx2);
                    impl_item.generics.params = params.into_iter().collect();
                }
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.generics.params.len() >= 2 {
                            let mut params: Vec<_> = func.sig.generics.params.iter().collect();
                            let idx1 = thread_rng().gen_range(0..params.len());
                            let idx2 = thread_rng().gen_range(0..params.len());
                            params.swap(idx1, idx2);
                            func.sig.generics.params = params.into_iter().collect();
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct or function definitions with generic type parameters and swaps the order of two type parameters in the definition. This transformation tests the compiler's handling of generic type parameters and their ordering, potentially leading to issues with type inference, trait resolution, or generic instantiation."
    }
}