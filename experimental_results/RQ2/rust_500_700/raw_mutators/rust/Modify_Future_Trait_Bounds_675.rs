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

pub struct Modify_Future_Trait_Bounds_675;

impl Mutator for Modify_Future_Trait_Bounds_675 {
    fn name(&self) -> &str {
        "Modify_Future_Trait_Bounds_675"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        if let Type::ImplTrait(type_impl_trait) = &type_item.ty {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Future") {
                                        let mut new_trait_bound = trait_bound.clone();
                                        new_trait_bound.path.segments.last_mut().unwrap().arguments = PathArguments::AngleBracketed(parse_quote!(<Output = u32>));
                                        new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        new_bounds.push(parse_quote!(Send));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            type_item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: type_impl_trait.impl_token,
                                bounds: new_bounds,
                            }));
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