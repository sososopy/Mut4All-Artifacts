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

pub struct Modify_Const_Generic_Parameter_Type_537;

impl Mutator for Modify_Const_Generic_Parameter_Type_537 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameter_Type_537"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for generic_param in &mut trait_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = generic_param {
                        if let syn::Type::Path(type_path) = &*const_param.ty {
                            if type_path.path.is_ident("str") {
                                const_param.ty = Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Const(const_item) = impl_item {
                        if let syn::Expr::Lit(expr_lit) = &const_item.expr {
                            if let syn::Lit::Str(_) = expr_lit.lit {
                                const_item.expr = syn::Expr::Lit(syn::ExprLit {
                                    attrs: vec![],
                                    lit: syn::Lit::Int(syn::LitInt::new("42", Span::call_site())),
                                });
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