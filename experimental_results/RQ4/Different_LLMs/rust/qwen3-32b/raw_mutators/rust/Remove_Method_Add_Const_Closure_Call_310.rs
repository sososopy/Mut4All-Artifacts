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
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    let mut methods: Vec<_> = impl_item.items.iter_mut().filter_map(|i| {
                        if let syn::ImplItem::Fn(func) = i {
                            Some(func)
                        } else {
                            None
                        }
                    }).collect();
                    if let Some(method) = methods.into_iter().next() {
                        let method_ident = &method.sig.ident;
                        let self_ty = &impl_item.self_ty;
                        let index = impl_item.items.iter().position(|i| {
                            if let syn::ImplItem::Fn(func) = i {
                                &func.sig.ident == method_ident
                            } else {
                                false
                            }
                        }).unwrap();
                        impl_item.items.remove(index);
                        let const_item = parse_quote! {
                            const _: () = {
                                const || {
                                    <#self_ty as #trait_path>::#method_ident();
                                }();
                            };
                        };
                        file.items.push(Item::Const(const_item));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}