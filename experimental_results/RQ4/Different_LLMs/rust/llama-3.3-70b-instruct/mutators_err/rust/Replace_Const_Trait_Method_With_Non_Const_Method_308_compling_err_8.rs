use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Trait_Method_With_Non_Const_Method_308;

impl Mutator for Replace_Const_Trait_Method_With_Non_Const_Method_308 {
    fn name(&self) -> &str {
        "Replace_Const_Trait_Method_With_Non_Const_Method_308"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        if let Some(_) = method.sig.constness {
                            let mut new_block = method.block.clone();
                            new_block.stmts.clear();
                            let mut_x = parse_quote! { let mut x = 0; };
                            let x = parse_quote! { x };
                            new_block.stmts.push(Stmt::Expr(mut_x, None));
                            new_block.stmts.push(Stmt::Expr(x, None));
                            method.block = new_block;
                            method.sig.constness = None;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const trait methods and replaces them with non-const versions. This transformation aims to test how the code behaves when const methods are called in non-const contexts, potentially exposing bugs related to const correctness."
    }
}

impl syn::visit_mut::VisitMut for Replace_Const_Trait_Method_With_Non_Const_Method_308 {
    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        for impl_item in &mut i.items {
            if let syn::ImplItem::Method(method) = impl_item {
                if let Some(_) = method.sig.constness {
                    let mut new_block = method.block.clone();
                    new_block.stmts.clear();
                    let mut_x = parse_quote! { let mut x = 0; };
                    let x = parse_quote! { x };
                    new_block.stmts.push(Stmt::Expr(mut_x, None));
                    new_block.stmts.push(Stmt::Expr(x, None));
                    method.block = new_block;
                    method.sig.constness = None;
                }
            }
        }
    }
}