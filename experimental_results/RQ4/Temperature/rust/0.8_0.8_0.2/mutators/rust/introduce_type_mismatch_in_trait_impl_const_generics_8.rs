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

pub struct Introduce_Type_Mismatch_In_Trait_Impl_Const_Generics_8;

impl Mutator for Introduce_Type_Mismatch_In_Trait_Impl_Const_Generics_8 {
    fn name(&self) -> &str {
        "Introduce_Type_Mismatch_In_Trait_Impl_Const_Generics_8"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if trait_path.segments.last().unwrap().ident == "ExampleTrait" {
                        if let Some(syn::GenericParam::Const(const_param)) = 
                            item_impl.generics.params.first() {
                            if const_param.ident == "N" {
                                item_impl.generics.params.clear();
                                item_impl.generics.params.push(parse_quote!(const N: u64));

                                for impl_item in &mut item_impl.items {
                                    if let syn::ImplItem::Const(impl_const) = impl_item {
                                        if impl_const.ident == "CONST_ASSOC" {
                                            impl_const.expr = parse_quote!(N as usize);
                                        }
                                    } else if let syn::ImplItem::Fn(method) = impl_item {
                                        method.sig.inputs = method.sig.inputs.iter().map(|arg| {
                                            if let syn::FnArg::Typed(pat_type) = arg {
                                                syn::FnArg::Typed(syn::PatType {
                                                    ty: Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath::from(Ident::new("usize", Span::call_site())),
                                                    })),
                                                    ..pat_type.clone()
                                                })
                                            } else {
                                                arg.clone()
                                            }
                                        }).collect();
                                    }
                                }
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