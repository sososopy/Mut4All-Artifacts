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

pub struct Modify_Trait_Bounds_With_Impl_Syntax_48;

impl Mutator for Modify_Trait_Bounds_With_Impl_Syntax_48 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_With_Impl_Syntax_48"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    let mut new_bounds = Punctuated::new();

                    for bound in &type_impl_trait.bounds {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            if trait_bound.path.is_ident("Sized") {
                                continue;
                            }
                        }
                        new_bounds.push(bound.clone());
                    }

                    if new_bounds.is_empty() || thread_rng().gen_bool(0.5) {
                        new_bounds.push(parse_quote!(Clone));
                    }

                    item_type.ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds: new_bounds,
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}