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

pub struct Alter_Const_Generic_Params_In_Methods_535;

impl Mutator for Alter_Const_Generic_Params_In_Methods_535 {
    fn name(&self) -> &str {
        "Alter_Const_Generic_Params_In_Methods_535"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let generics = &mut method.sig.generics;
                        if !generics.params.is_empty() {
                            generics.params.push(parse_quote!(const B: usize));
                            method.sig.inputs.push(parse_quote!(values: [u8; B]));
                            if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                                let new_stmt: Stmt = parse_quote! {
                                    &values[0]
                                };
                                method.block.stmts.clear();
                                method.block.stmts.push(new_stmt);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}