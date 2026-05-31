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

pub struct Replace_ExternC_Return_With_ImplTrait_201;

impl Mutator for Replace_ExternC_Return_With_ImplTrait_201 {
    fn name(&self) -> &str {
        "Replace_ExternC_Return_With_ImplTrait_201"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = Vec::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_names.push(item_trait.ident.clone());
            }
        }
        let mut has_dummy_trait = false;
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "DummyTrait" {
                    has_dummy_trait = true;
                }
            }
        }
        if !has_dummy_trait {
            let dummy_trait: Item = parse_quote! {
                trait DummyTrait {
                    fn dummy(&self) -> i32;
                }
            };
            file.items.insert(0, dummy_trait);
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        if item_fn.sig.abi.is_some() {
                            let return_type = &mut item_fn.sig.output;
                            let mut new_return_type: ReturnType = parse_quote! { -> impl DummyTrait };
                            if !trait_names.is_empty() {
                                let trait_name = trait_names[0].clone();
                                new_return_type = parse_quote! { -> impl #trait_name };
                            }
                            *return_type = new_return_type;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait methods with `extern \"C\"` ABI by replacing their return type with `impl Trait`. It leverages existing traits in the module or introduces a `DummyTrait` if none are available. This transformation challenges the compiler's handling of FFI function signatures and opaque return types, potentially revealing issues in type-checking and ABI compliance."
    }
}