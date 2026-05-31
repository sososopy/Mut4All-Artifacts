use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Modify_Trait_Method_Return_Type_250;

impl Mutator for Modify_Trait_Method_Return_Type_250 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Return_Type_250"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                            if let syn::Type::ImplTrait(type_impl_trait) = &**ty {
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                for bound in &mut new_bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.segments.last().unwrap().ident == "FnOnce" {
                                            trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("FnMut", Span::call_site());
                                        } else if trait_bound.path.segments.last().unwrap().ident == "FnMut" {
                                            trait_bound.path.segments.last_mut().unwrap().ident = Ident::new("Fn", Span::call_site());
                                        }
                                    }
                                }
                                *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                                    impl_token: type_impl_trait.impl_token,
                                    bounds: new_bounds,
                                }));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method return types involving closures or higher-order functions. It modifies the return type to introduce potential errors related to trait method resolution, type inference, or closure handling."
    }
}