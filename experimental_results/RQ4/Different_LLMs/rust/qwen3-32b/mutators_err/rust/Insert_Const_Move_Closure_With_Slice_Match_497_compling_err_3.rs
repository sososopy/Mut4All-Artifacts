use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Insert_Const_Move_Closure_With_Slice_Match_497;

impl Mutator for Insert_Const_Move_Closure_With_Slice_Match_497 {
    fn name(&self) -> &str {
        "Insert_Const_Move_Closure_With_Slice_Match_497"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &**return_type {
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let TraitBoundModifier::Maybe(_) = trait_bound.modifier {
                                        if let Some(first_segment) = trait_bound.path.segments.first() {
                                            if first_segment.ident == "Fn" {
                                                let block = &mut func.block;
                                                for stmt in &mut block.stmts {
                                                    if let Stmt::Expr(expr, _) = stmt {
                                                        if let Expr::Closure(closure) = expr.as_mut() {
                                                            let new_body = parse_quote! {
                                                                {
                                                                    let sl: &[u8] = b"bar";
                                                                    match sl {
                                                                        [a, b @ ..] => assert_eq!(*a, 98)
                                                                    }
                                                                }
                                                            };
                                                            *expr = Box::new(ExprClosure {
                                                                attrs: closure.attrs.clone(),
                                                                capture: syn::Capture::FnOnce(syn::FnOnce {
                                                                    fn_once_token: Default::default(),
                                                                }),
                                                                constness: Some(Default::default()),
                                                                movability: None,
                                                                lifetimes: Punctuated::new(),
                                                                or1_token: Some(Default::default()),
                                                                or2_token: Some(Default::default()),
                                                                inputs: Punctuated::new(),
                                                                output: closure.output.clone(),
                                                                body: Box::new(new_body),
                                                            });
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
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