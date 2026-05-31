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

pub struct Trait_Const_Generic_63;

impl Mutator for Trait_Const_Generic_63 {
    fn name(&self) -> &str {
        "Trait_Const_Generic_63"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.items.iter().any(|i| matches!(i, syn::TraitItem::Const(_))) {
                    let trait_name = &trait_item.ident;
                    let mut rng = thread_rng();
                    let const_generic_ident = Ident::new("N", Span::call_site());
                    let const_generic_type: u64 = rng.gen_range(1..100);

                    let impl_block: Item = parse_quote! {
                        impl<const #const_generic_ident: u64> #trait_name for [u8; #const_generic_ident] {
                            const ASSOC: usize = #const_generic_ident as usize;
                        }
                    };
                    file.items.push(impl_block);

                    let use_trait_fn: Item = parse_quote! {
                        fn use_trait() -> [u8; <[u8; 13] as #trait_name>::ASSOC] {
                            [0; 13]
                        }
                    };
                    file.items.push(use_trait_fn);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits with associated constants and creates implementations using constant generics. It introduces potential inconsistencies by using the constant generic in type-level operations, aiming to provoke issues in const evaluation and type inference."
    }
}