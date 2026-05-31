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

pub struct RpititAssociatedTypeAnonymity_191;

impl Mutator for RpititAssociatedTypeAnonymity_191 {
    fn name(&self) -> &str {
        "RpititAssociatedTypeAnonymity_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if let syn::ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let syn::Type::ImplTrait(impl_trait) = return_type.as_mut() {
                                // Add two lifetimes to the method's generics: 'b followed by 'a: 'b
                                method.sig.generics.params.push(parse_quote!(lifetime 'b));
                                method.sig.generics.params.push(parse_quote!(lifetime 'a: 'b));
                                
                                // Replace the return type with impl ToString + 'b
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path::from(Ident::new("ToString", Span::call_site())),
                                }));
                                bounds.push(syn::TypeParamBound::Lifetime(syn::Lifetime::new(
                                    "'b",
                                    Span::call_site(),
                                )));
                                impl_trait.bounds = bounds;
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