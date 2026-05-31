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

pub struct Replace_Const_Trait_Implementation_With_Non_Const_307;

impl Mutator for Replace_Const_Trait_Implementation_With_Non_Const_307 {
    fn name(&self) -> &str {
        "Replace_Const_Trait_Implementation_With_Non_Const_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if impl_item.attrs.iter().any(|attr| attr.path().is_ident("const")) {
                    impl_item.attrs.retain(|attr| !attr.path().is_ident("const"));
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Fn(func) = impl_item {
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
        "The mutation operator replaces const trait implementations with non-const implementations. It removes the `const` keyword from trait implementations and their methods, potentially affecting the compiler's analysis and optimization of the code. This transformation aims to test the compiler's handling of const trait implementations and may lead to ICEs or inference failures in the trait resolution system."
    }
}