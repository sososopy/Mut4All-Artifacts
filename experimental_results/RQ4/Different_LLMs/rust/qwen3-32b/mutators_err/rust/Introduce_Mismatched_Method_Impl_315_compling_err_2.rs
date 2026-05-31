use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Introduce_Mismatched_Method_Impl_315;

impl Mutator for Introduce_Mismatched_Method_Impl_315 {
    fn name(&self) -> &str {
        "Introduce_Mismatched_Method_Impl_315"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_def) = item {
                let has_attr = trait_def.attrs.iter().any(|attr| {
                    if let Ok(meta) = attr.parse_meta() {
                        if let syn::Meta::Path(path) = meta {
                            path.segments.iter().any(|seg| seg.ident == "const_trait")
                        } else {
                            false
                        }
                    } else {
                        false
                    }
                });
                if has_attr {
                    let mut methods = HashSet::new();
                    for item_trait in &trait::items {
                        if let TraitItem::Fn(func) = item_trait {
                            methods.insert(func.sig.ident.clone());
                        }
                    }
                    const_traits.push((trait_def.ident.clone(), methods));
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_block) = item {
                if impl_block.constness.is_some() {
                    if let Some((_, trait_path, _)) = &mut impl_block.trait_ {
                        let trait_ident = match trait_path.segments.last() {
                            Some(seg) => seg.ident.clone(),
                            None => continue,
                        };
                        if let Some((_, methods)) = const_traits.iter().find(|(ident, _)| *ident == trait_ident) {
                            for impl_item in &mut impl_block.items {
                                if let ImplItem::Fn(func) = impl_item {
                                    if methods.contains(&func.sig.ident) {
                                        func.sig.ident = Ident::new("bar", func.sig.ident.span());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}