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

pub struct Introduce_Additional_Trait_Layer_586;

impl Mutator for Introduce_Additional_Trait_Layer_586 {
    fn name(&self) -> &str {
        "Introduce_Additional_Trait_Layer_586"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_impls = Vec::new();

        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        let trait_name = &last_segment.ident;
                        trait_impls.push((trait_name.clone(), item_impl.self_ty.clone()));
                    }
                }
            }
        }

        for (trait_name, self_ty) in trait_impls {
            let super_trait_name = Ident::new(&format!("Super{}", trait_name), Span::call_site());
            let super_trait: Item = parse_quote! {
                trait #super_trait_name<'a>: #trait_name<'a> {}
            };
            file.items.push(super_trait);

            let super_trait_impl: Item = parse_quote! {
                impl<'a> #super_trait_name<'a> for #self_ty {}
            };
            file.items.push(super_trait_impl);

            for item in &mut file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &mut item_impl.trait_ {
                        if let Some(last_segment) = path.segments.last_mut() {
                            if last_segment.ident == trait_name {
                                last_segment.ident = super_trait_name.clone();
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