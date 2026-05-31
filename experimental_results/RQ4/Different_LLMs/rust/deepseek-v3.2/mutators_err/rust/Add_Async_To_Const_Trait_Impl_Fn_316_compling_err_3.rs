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

pub struct Add_Async_To_Const_Trait_Impl_Fn_316;

impl Mutator for Add_Async_To_Const_Trait_Impl_Fn_316 {
    fn name(&self) -> &str {
        "Add_Async_To_Const_Trait_Impl_Fn_316"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if !func.sig.constness.is_some() {
                            continue;
                        }
                        if func.sig.asyncness.is_some() {
                            continue;
                        }
                        let mut new_sig = func.sig.clone();
                        let async_token = token::Async {
                            span: Span::call_site(),
                        };
                        new_sig.asyncness = Some(async_token);
                        let mut new_attrs = Vec::new();
                        for attr in &func.attrs {
                            new_attrs.push(attr.clone());
                        }
                        let mut new_vis = func.vis.clone();
                        let mut new_block = func.block.clone();
                        let mut new_unsafety = func.sig.unsafety.clone();
                        let mut new_abi = func.sig.abi.clone();
                        let mut new_generics = func.sig.generics.clone();
                        let mut new_inputs = func.sig.inputs.clone();
                        let mut new_output = func.sig.output.clone();
                        let mut new_variadic = func.sig.variadic.clone();
                        let mut new_ident = func.sig.ident.clone();
                        let mut new_constness = func.sig.constness.clone();
                        let mut new_fn_token = func.sig.fn_token.clone();
                        let mut new_paren_token = func.sig.paren_token.clone();
                        let mut new_sig_tokens = new_sig.to_token_stream();
                        let mut new_func = syn::ImplItemFn {
                            attrs: new_attrs,
                            vis: new_vis,
                            defaultness: func.defaultness.clone(),
                            sig: new_sig,
                            block: new_block,
                        };
                        *func = new_func;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an async modifier to const function declarations within trait impl blocks. It specifically targets const fn in trait implementations, inserting async before const. This transformation creates the async const fn combination which is known to trigger internal compiler errors in the mir_const_qualif query. The mutation applies even if the trait definition does not declare the mutated function, potentially causing type checking cycles or ICEs related to const and async interaction."
    }
}