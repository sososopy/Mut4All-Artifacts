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

pub struct Toggle_Const_Trait_Impl_322;

impl Mutator for Toggle_Const_Trait_Impl_322 {
    fn name(&self) -> &str {
        "Toggle_Const_Trait_Impl_322"
    }
    fn mutate(&self, file: &mut syn::File) {
        let builtin_traits: HashSet<&str> = ["Drop", "Copy", "Clone", "Sized", "Destruct"].iter().cloned().collect();
        let mut impls_to_mutate = Vec::new();
        
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.to_string();
                    impls_to_mutate.push((item_impl.span(), trait_name, item_impl.defaultness.is_some()));
                } else {
                    impls_to_mutate.push((item_impl.span(), String::new(), item_impl.defaultness.is_some()));
                }
            }
        }
        
        if impls_to_mutate.is_empty() {
            return;
        }
        
        let mut rng = thread_rng();
        let (target_span, trait_name, has_const) = impls_to_mutate.choose(&mut rng).unwrap().clone();
        
        let mut visitor = ConstTraitImplVisitor {
            target_span,
            trait_name,
            has_const,
            builtin_traits,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator toggles the const qualifier on trait implementations, prioritizing built-in traits (Drop, Copy, Clone, Sized, Destruct) that are more likely to trigger compiler edge cases. It randomly selects one impl block per mutation and either adds const if absent or removes it if present. This directly targets const trait implementation handling, potentially exposing bugs in trait resolution, const evaluation, or compiler-internal trait processing."
    }
}

struct ConstTraitImplVisitor {
    target_span: Span,
    trait_name: String,
    has_const: bool,
    builtin_traits: HashSet<&'static str>,
    mutated: bool,
}

impl VisitMut for ConstTraitImplVisitor {
    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        if i.span().into() == self.target_span && !self.mutated {
            if self.has_const {
                i.defaultness = None;
            } else {
                i.defaultness = Some(token::Default::default());
            }
            self.mutated =  true;
        }
        syn::visit_mut::visit_item_impl_mut(self, i);
    }
}