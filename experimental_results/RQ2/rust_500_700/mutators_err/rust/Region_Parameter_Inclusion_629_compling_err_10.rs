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

pub struct Region_Parameter_Inclusion_629;

impl Mutator for Region_Parameter_Inclusion_629 {
    fn name(&self) -> &str {
        "Region_Parameter_Inclusion_629"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if let Some(lifetime) = impl_type.generics.lifetimes().next() {
                            let new_lifetime_ident = Ident::new("b", Span::call_site());
                            let new_lifetime = Lifetime::new("'b", Span::call_site());

                            if !impl_type.generics.lifetimes().any(|lt| lt.lifetime == new_lifetime) {
                                impl_type.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                                    attrs: Vec::new(),
                                    lifetime: new_lifetime,
                                    colon_token: None,
                                    bounds: Punctuated::new(),
                                }));

                                if let Type::Reference(type_ref) = impl_type.ty.as_ref() {
                                    let new_type = Type::Tuple(TypeTuple {
                                        paren_token: Paren::default(),
                                        elems: {
                                            let mut elems = Punctuated::new();
                                            elems.push(Type::Reference(type_ref.clone()));
                                            elems.push(Type::Reference(TypeReference {
                                                and_token: token::And::default(),
                                                lifetime: Some(new_lifetime),
                                                mutability: None,
                                                elem: Box::new(Type::Tuple(TypeTuple {
                                                    paren_token: Paren::default(),
                                                    elems: Punctuated::new(),
                                                })),
                                            }));
                                            elems
                                        },
                                    });
                                    impl_type.ty = Box::new(new_type);
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