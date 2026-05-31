use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, File, GenericParam, Item, ItemFn, ItemImpl,
    ImplItem, Ident,
};

use crate::mutator::Mutator;

pub struct AddUnusedGenericTypeParams252;

impl Mutator for AddUnusedGenericTypeParams252 {
    fn name(&self) -> &str {
        "AddUnusedGenericTypeParams252"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    for i in 0..5 {
                        let generic_param: GenericParam = parse_quote!(T#i);
                        generics.params.push(generic_param);
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            for i in 0..5 {
                                let generic_param: GenericParam = parse_quote!(U#i);
                                generics.params.push(generic_param);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds multiple unused generic type parameters to non-main function and impl signatures. This stresses the compiler's handling of generic parameters, potentially triggering issues with type resolution, unused parameter warnings, or internal compiler errors related to generic parameter management."
    }
}