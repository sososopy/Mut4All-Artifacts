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

pub struct Inject_Recursive_Type_Alias_409;

impl Mutator for Inject_Recursive_Type_Alias_409 {
    fn name(&self) -> &str {
        "Inject_Recursive_Type_Alias_409"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias: Item = parse_quote! {
            type RecursiveAlias = RecursiveAlias;
        };

        file.items.push(type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let recursive_type: Type = parse_quote!(RecursiveAlias);
                    func.sig.output = syn::ReturnType::Type(
                        syn::token::RArrow { spans: [Span::call_site(), Span::call_site()] },
                        Box::new(recursive_type),
                    );
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if func.sig.ident != "main" {
                            let recursive_type: Type = parse_quote!(RecursiveAlias);
                            func.sig.output = syn::ReturnType::Type(
                                syn::token::RArrow { spans: [Span::call_site(), Span::call_site()] },
                                Box::new(recursive_type),
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = RecursiveAlias;` into the program. It then modifies the return type of non-main functions and methods to use this recursive alias. The transformation aims to stress the compiler's type resolution and recursion handling mechanisms, potentially triggering infinite loops, stack overflows, or ICEs due to unresolved recursive types."
    }
}