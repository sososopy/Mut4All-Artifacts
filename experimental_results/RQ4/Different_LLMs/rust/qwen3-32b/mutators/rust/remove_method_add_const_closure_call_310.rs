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

pub struct Remove_Method_Add_Const_Closure_Call_310;

impl Mutator for Remove_Method_Add_Const_Closure_Call_310 {
    fn name(&self) -> &str {
        "Remove_Method_Add_Const_Closure_Call_310"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    let index = impl_item.items.iter().enumerate().find_map(|(i, item)| {
                        if let syn::ImplItem::Fn(_) = item {
                            Some(i)
                        } else {
                            None
                        }
                    });
                    if let Some(index) = index {
                        let removed_item = impl_item.items.remove(index);
                        if let syn::ImplItem::Fn(method) = removed_item {
                            let method_ident = &method.sig.ident;
                            let self_ty = &impl_item.self_ty;
                            let const_item = parse_quote! {
                                const _: () = {
                                    const || {
                                        <#self_ty as #trait_path>::#method_ident();
                                    }();
                                };
                            };
                            new_items.push(Item::Const(const_item));
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}