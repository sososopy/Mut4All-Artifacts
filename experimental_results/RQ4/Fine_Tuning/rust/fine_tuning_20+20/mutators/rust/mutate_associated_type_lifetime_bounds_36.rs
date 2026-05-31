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

pub struct Mutate_Associated_Type_Lifetime_Bounds_36;

impl Mutator for Mutate_Associated_Type_Lifetime_Bounds_36 {
    fn name(&self) -> &str {
        "Mutate_Associated_Type_Lifetime_Bounds_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Type(ty) = item {
                        if ty.generics.params.iter().any(|param| {
                            matches!(param, GenericParam::Lifetime(_))
                        }) {
                            ty.generics
                                .make_where_clause()
                                .predicates
                                .push(parse_quote!(Self: 'static));
                        }
                    }
                }
            } else if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(ident) = path.segments.last().map(|s| &s.ident) {
                        if ident == "Trait" {
                            for impl_item in &mut item_impl.items {
                                if let ImplItem::Type(ty) = impl_item {
                                    if ty.generics.params.iter().any(|param| {
                                        matches!(param, GenericParam::Lifetime(_))
                                    }) {
                                        ty.generics
                                            .make_where_clause()
                                            .predicates
                                            .push(parse_quote!(Self: 'static));
                                        ty.eq_token = Default::default();
                                        ty.ty = Type::Tuple(TypeTuple {
                                            paren_token: Default::default(),
                                            elems: Punctuated::new(),
                                        });
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
        "This mutator targets traits with associated types that have lifetime parameters. It modifies the trait definition to include a `where Self: 'static` bound on the associated type. In the corresponding trait implementation, it changes the associated type to a unit type `()` and adds the same lifetime bound. This transformation stresses the compiler's handling of lifetime constraints in trait-associated types, potentially exposing issues in lifetime inference and trait resolution."
    }
}