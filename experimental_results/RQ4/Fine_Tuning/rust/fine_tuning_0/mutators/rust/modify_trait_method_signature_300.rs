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
    ItemTrait, TraitItem, ImplItem,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Method_Signature_300;

impl Mutator for Modify_Trait_Method_Signature_300 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut new_generic_type = Ident::new("T", Span::call_site());

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() && !method.sig.generics.params.is_empty() {
                            method.sig.generics.params.push(parse_quote!(#new_generic_type));
                            
                            // Update the trait method's return type or where clause to use the new generic parameter
                            if let ReturnType::Default = method.sig.output {
                                method.sig.output = parse_quote!(-> Option<#new_generic_type>);
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.asyncness.is_some() && !method.sig.generics.params.is_empty() {
                            method.sig.generics.params.push(parse_quote!(#new_generic_type));
                            
                            // Update the impl method's return type or where clause to use the new generic parameter
                            if let ReturnType::Default = method.sig.output {
                                method.sig.output = parse_quote!(-> Option<#new_generic_type>);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async trait methods with generic parameters, adding a new generic parameter `T` to their signatures. It updates the return type to `Option<T>` to ensure the new parameter is utilized, testing the compiler's handling of expanded generic contexts and async method resolution."
    }
}