use crate::mutator::Mutator;
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
pub struct Add_Constant_Generics_1;
impl Mutator for Add_Constant_Generics_1 {
    fn name(&self) -> &str {
        "Add_Constant_Generics_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                if func.sig.ident != "main" {
                    generics.params.push(parse_quote!(const N: usize));
                    generics.params.push(parse_quote!(const M: isize));
                    generics.params.push(parse_quote!(const B: bool));
                    generics.params.push(parse_quote!(const C: char));
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let generics = &mut func.sig.generics;
                        if func.sig.ident != "main" {
                            generics.params.push(parse_quote!(const N: usize));
                            generics.params.push(parse_quote!(const M: isize));
                            generics.params.push(parse_quote!(const B: bool));
                            generics.params.push(parse_quote!(const C: char));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds multiple generic const parameters (usize, isize, bool, char) to non-main function and impl signatures. It aims to stress the compiler's const generics system by introducing diverse constant types, potentially triggering type resolution issues, const evaluation bugs, or unexpected behavior in generic function contexts."
    }
}
