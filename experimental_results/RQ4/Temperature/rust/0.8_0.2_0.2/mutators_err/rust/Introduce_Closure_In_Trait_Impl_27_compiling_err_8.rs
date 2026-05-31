use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, GenericParam, Item, ItemImpl, ImplItem, Stmt, parse_quote, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct IntroduceClosureInTraitImpl27;

impl Mutator for IntroduceClosureInTraitImpl27 {
    fn name(&self) -> &str {
        "IntroduceClosureInTraitImpl27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if item_impl.trait_.is_some() {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            let has_generic = item_impl.generics.params.iter().any(|param| {
                                matches!(param, syn::GenericParam::Type(_))
                            });
                            if has_generic {
                                let new_stmt: Stmt = parse_quote! {
                                    let capture_value = 20;
                                    let closure = || capture_value * 2;
                                    closure();
                                };
                                method.block.stmts.insert(0, new_stmt);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a closure within trait implementations that involve generic types. It captures a local variable and performs a simple operation, ensuring interaction with Rust's specialization and type alias systems. This transformation aims to explore potential ICEs by leveraging the complexity of closures in generic contexts."
    }
}