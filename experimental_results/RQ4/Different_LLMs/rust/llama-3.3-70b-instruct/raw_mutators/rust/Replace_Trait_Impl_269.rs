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

pub struct Replace_Trait_Impl_269;

impl Mutator for Replace_Trait_Impl_269 {
    fn name(&self) -> &str {
        "Replace_Trait_Impl_269"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = get_trait_path(&impl_item) {
                    let new_impl = create_new_impl(&impl_item, &path);
                    *item = Item::Impl(new_impl);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations and replaces them with new implementations. It aims to test the trait system by introducing different method bodies or making the trait implementation incompatible with the trait definition."
    }
}

fn get_trait_path(impl_item: &ItemImpl) -> Option<(&Ident, &SynPath, &Type)> {
    if let Some(trait_path) = &impl_item.trait_ {
        let path = &trait_path.1;
        let for_type = &impl_item.self_ty;
        let ident = &impl_item.trait_.as_ref().unwrap().1.segments.first().unwrap().ident;
        Some((ident, path, for_type))
    } else {
        None
    }
}

fn create_new_impl(impl_item: &ItemImpl, path: &SynPath) -> ItemImpl {
    let mut new_impl = impl_item.clone();
    for impl_item in &mut new_impl.items {
        if let ImplItem::Fn(func) = impl_item {
            let new_func = create_new_func(func, path);
            *impl_item = ImplItem::Fn(new_func);
        }
    }
    new_impl
}

fn create_new_func(func: &ImplItemFn, path: &SynPath) -> ImplItemFn {
    let mut new_func = func.clone();
    let new_block = create_new_block(&func.block, path);
    new_func.block = new_block;
    new_func
}

fn create_new_block(block: &Block, path: &SynPath) -> Block {
    let mut new_block = block.clone();
    new_block.stmts.clear();
    new_block.stmts.push(Stmt::Expr(parse_quote! { println!("New implementation for {}", #path) }, None));
    new_block
}