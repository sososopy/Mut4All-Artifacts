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

pub struct ReplaceMacroInvocationWithFunctionCall_207;

impl Mutator for ReplaceMacroInvocationWithFunctionCall_207 {
    fn name(&self) -> &str {
        "ReplaceMacroInvocationWithFunctionCall_207"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Macro(mac) = expr {
                            let macro_name = mac.mac.path.segments.first().unwrap().ident.to_string();
                            let func_name = Ident::new(&macro_name, mac.mac.path.span());
                            let func_call = Expr::Call(ExprCall {
                                attrs: vec![],
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: func_name,
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                    qself: None,
                                })),
                                paren_token: Paren {
                                    span: mac.mac.path.span(),
                                },
                                args: Punctuated::from_iter(mac.mac.tokens.clone().into_iter().map(|token| {
                                    Expr::Verbatim(quote!(#token))
                                })),
                            });
                            *expr = func_call;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces macro invocations with function calls having the same name as the macro. This transformation aims to test the compiler's ability to handle the transition between macro invocations and function calls, potentially exposing bugs in the resolution and expansion of macros within the compiler."
    }
}