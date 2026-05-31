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

pub struct Replace_Intrinsic_With_Constant_425;

impl Mutator for Replace_Intrinsic_With_Constant_425 {
    fn name(&self) -> &str {
        "Replace_Intrinsic_With_Constant_425"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let Expr::Call(ExprCall {
                    expr: Expr::Path(ExprPath {
                        path: SynPath {
                            segments: punctuated::Punctuated::from_iter(vec![syn::PathSegment {
                                ident: Ident::new("transmute", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: punctuated::Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("u8", Span::call_site())),
                                    }))]),
                                    gt_token: token::Gt::default(),
                                }),
                            }]),
                            ..
                        }),
                        ..
                    }),
                    ..
                }) = &const_item.expr {
                    const_item.expr = parse_quote! { 0u8 };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the use of intrinsic functions within constant expressions with constant values. This transformation tests the compiler's handling of constant expressions and intrinsic functions, and may expose potential bugs in the compiler's constant evaluation and code generation."
    }
}