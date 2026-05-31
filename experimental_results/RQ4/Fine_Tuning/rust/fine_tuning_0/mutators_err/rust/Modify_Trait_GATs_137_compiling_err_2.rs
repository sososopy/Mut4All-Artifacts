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

pub struct Modify_Trait_GATs_137;

impl Mutator for Modify_Trait_GATs_137 {
    fn name(&self) -> &str {
        "Modify_Trait_GATs_137"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut found_gat = false;
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if type_item.ident == "Base" {
                            found_gat = true;
                            type_item.generics.params.push(parse_quote!(Extra));
                        }
                    }
                }
                if found_gat {
                    for item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = item {
                            method.sig.generics.params.push(parse_quote!(Extra));
                            if let ReturnType::Type(_, ref mut return_type) = method.sig.output {
                                if let Type::Path(type_path) = &**return_type {
                                    if let Some(segment) = type_path.path.segments.last_mut() {
                                        if segment.ident == "Base" {
                                            segment.arguments = PathArguments::AngleBracketed(
                                                syn::AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: {
                                                        let mut args = Punctuated::new();
                                                        args.push(GenericArgument::Type(parse_quote!(B)));
                                                        args.push(GenericArgument::Type(parse_quote!(Extra)));
                                                        args
                                                    },
                                                    gt_token: token::Gt::default(),
                                                },
                                            );
                                        }
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
        "This mutator targets traits with generic associated types (GATs), specifically modifying the associated type by adding an extra generic parameter. It also updates methods within the trait to use this new type signature, aiming to explore the compiler's handling of additional complexity in generic type resolution."
    }
}