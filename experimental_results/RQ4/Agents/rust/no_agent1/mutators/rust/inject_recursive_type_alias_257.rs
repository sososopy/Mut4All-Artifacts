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

pub struct Inject_Recursive_Type_Alias_257;

impl Mutator for Inject_Recursive_Type_Alias_257 {
    fn name(&self) -> &str {
        "Inject_Recursive_Type_Alias_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_alias: Item = parse_quote! {
            type RecursiveType = Box<RecursiveType>;
        };

        file.items.push(recursive_alias);

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_type: Type = parse_quote!(RecursiveType);
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        pat_type.ty = Box::new(recursive_type.clone());
                    }
                }
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(recursive_type.clone());
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let recursive_type: Type = parse_quote!(RecursiveType);
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                pat_type.ty = Box::new(recursive_type.clone());
                            }
                        }
                        if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                            *ty = Box::new(recursive_type.clone());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `RecursiveType` defined as `Box<RecursiveType>`. It modifies function and method signatures to use this recursive type for parameters and return types. This transformation aims to test the compiler's handling of recursive type definitions, potentially leading to infinite type resolution loops, stack overflows, or ICEs."
    }
}