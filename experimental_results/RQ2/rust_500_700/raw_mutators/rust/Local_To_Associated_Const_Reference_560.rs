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

pub struct Local_To_Associated_Const_Reference_560;

impl Mutator for Local_To_Associated_Const_Reference_560 {
    fn name(&self) -> &str {
        "Local_To_Associated_Const_Reference_560"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut global_constants = HashSet::new();

        for item in &file.items {
            if let syn::Item::Const(item_const) = item {
                global_constants.insert(item_const.ident.clone());
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Const(impl_const) = impl_item {
                        if let syn::Expr::Path(expr_path) = &*impl_const.expr {
                            if let Some(ident) = expr_path.path.get_ident() {
                                if !global_constants.contains(ident) {
                                    let new_const_ident = Ident::new(
                                        &format!("GLOBAL_{}", ident),
                                        Span::call_site(),
                                    );
                                    let new_const: syn::ItemConst = parse_quote! {
                                        const #new_const_ident: usize = 10;
                                    };
                                    file.items.push(syn::Item::Const(new_const));
                                    impl_const.expr = Box::new(parse_quote!(#new_const_ident));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies associated constants within trait implementations that are incorrectly assigned from local variables. It replaces these assignments with references to newly created global constants, ensuring the associated constants are assigned from a non-local source, thereby reducing the likelihood of internal compiler errors."
    }
}