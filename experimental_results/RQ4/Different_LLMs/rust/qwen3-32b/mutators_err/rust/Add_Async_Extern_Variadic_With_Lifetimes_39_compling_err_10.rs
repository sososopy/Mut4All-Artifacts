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

pub struct Add_Async_Extern_Variadic_With_Lifetimes_39;

impl Mutator for Add_Async_Extern_Variadic_With_Lifetimes_39 {
    fn name(&self) -> &str {
        "Add_Async_Extern_Variadic_With_Lifetimes_39"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.inputs.len() > 0 {
                    func.sig.unsafety = Some(syn::token::Unsafe::default());
                    func.sig.abi = Some(syn::Abi {
                        extern_token: syn::token::Extern::default(),
                        name: Some(syn::LitStr::new("C", proc_macro2::Span::call_site())),
                    });
                    func.sig.asyncness = Some(syn::token::Async::default());
                    func.sig.generics.params.push(syn::parse_quote!('a));
                    func.sig.generics.params.push(syn::parse_quote!('b));
                    func.sig.inputs.push(syn::parse_quote! { ... });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms functions with at least one parameter into async unsafe extern \"C\" functions with two named lifetimes and a C-variadic parameter. This creates complex combinations of async, extern, lifetimes, and variadic parameters, testing the compiler's handling of these features in overlapping contexts and potentially exposing edge cases in function signature validation and code generation."
    }
}