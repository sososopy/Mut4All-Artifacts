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

pub struct Closure_Impl_Bikeshed_Enum_Check_464;

impl Mutator for Closure_Impl_Bikeshed_Enum_Check_464 {
    fn name(&self) -> &str {
        "Closure_Impl_Bikeshed_Enum_Check_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() && func.sig.ident != "main" {
                    let new_return_type = parse_quote!(impl std::mem::BikeshedIntrinsicFrom());
                    func.sig.output = ReturnType::Type(token::RArrow::default(), Box::new(new_return_type));
                    func.attrs.push(parse_quote!(# [feature(transmutability)]));
                    func.attrs.push(parse_quote!(# [feature(unboxed_closures)]));
                    
                    let closure = parse_quote!(|| {
                        match x {
                            0 => 1,
                            _ => 0,
                        }
                    });
                    func.block = Box::new(syn::Block {
                        stmts: vec![
                            Stmt::Local(Local {
                                let_token: parse_quote!(let),
                                pat: Pat::Ident(PatIdent {
                                    ident: Ident::new("x", Span::call_site()),
                                    subpat: None,
                                    mutability: None,
                                    attrs: vec![],
                                    by_ref: None,
                                }),
                                init: Some(LocalInit {
                                    expr: Box::new(Expr::Assign(parse_quote! { 42 })),
                                    eq_token: token::Eq::default(),
                                    diverge: None,
                                }),
                                semi_token: Some(parse_quote!(; )),
                                attrs: vec![],
                            }),
                            Stmt::Expr(closure, None),
                        ],
                        brace_token: Default::default(),
                    });
                    
                    let new_return_type = parse_quote!(impl ~const std::mem::BikeshedIntrinsicFrom());
                    func.sig.output = ReturnType::Type(token::RArrow::default(), Box::new(new_return_type));
                    func.attrs.push(parse_quote!(# [feature(const_trait_impl)]));
                }
            } else if let Item::Enum(enum_item) = item {
                enum_item.attrs.push(parse_quote!(# [repr(packed, align(0x100))]));
                enum_item.attrs.push(parse_quote!(# [repr(u8)]));
                
                let enum_name = &enum_item.ident;
                let new_fn = parse_quote! {
                    # [feature(transmutability)]
                    # [feature(unboxed_closures)]
                    const fn check_transmutable_ #enum_name() -> impl std::mem::BikeshedIntrinsicFrom() {
                        is_maybe_transmutable::<#enum_name>();
                        || {}
                    }
                };
                file.items.push(Item::Fn(new_fn));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}