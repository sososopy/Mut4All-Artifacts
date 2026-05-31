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

pub struct Modify_Trait_Function_Signatures_299;

impl Mutator for Modify_Trait_Function_Signatures_299 {
    fn name(&self) -> &str {
        "Modify_Trait_Function_Signatures_299"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        let mut new_sig = method.sig.clone();

                        // Modify parameters
                        for input in &mut new_sig.inputs {
                            if let FnArg::Typed(PatType { ty, .. }) = input {
                                if let Type::Path(TypePath { path, .. }) = &**ty {
                                    if let Some(segment) = path.segments.last() {
                                        if segment.arguments.is_empty() {
                                            // Replace with `impl Trait`
                                            *ty = parse_quote! { impl #path };
                                        }
                                    }
                                }
                            }
                        }

                        // Modify return type
                        if let ReturnType::Type(_, ty) = &mut new_sig.output {
                            if let Type::Path(TypePath { path, .. }) = &**ty {
                                if let Some(segment) = path.segments.last() {
                                    if segment.arguments.is_empty() {
                                        // Replace with `impl Trait`
                                        *ty = parse_quote! { impl #path };
                                    }
                                }
                            }
                        }

                        method.sig = new_sig;
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions within traits, specifically those using trait types as parameters or return types. It modifies these signatures by replacing trait types with generic types constrained by the trait or using `impl Trait` where applicable. This encourages exploring object safety and the impact of such changes in complex trait hierarchies."
    }
}