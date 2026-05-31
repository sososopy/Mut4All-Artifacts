use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprField, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct ReplaceFieldAccessWithMethodCall_64;

impl Mutator for ReplaceFieldAccessWithMethodCall_64 {
    fn name(&self) -> &str {
        "ReplaceFieldAccessWithMethodCall_64"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        let method_name = method.sig.ident.clone();
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Field(expr_field) = expr {
                                    let field_name = expr_field.member.clone();
                                    let method_name = Ident::new(
                                        &format!("get_{}", field_name.to_token_stream().to_string()),
                                        expr_field.member.span(),
                                    );
                                    let method_call = Expr::Call(ExprCall {
                                        attrs: vec![],
                                        func: Box::new(Expr::Path(ExprPath {
                                            attrs: vec![],
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: method_name,
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                            qself: None,
                                        })),
                                        paren_token: Paren {
                                            span: expr_field.member.span(),
                                        },
                                        args: punctuated::Punctuated::new(),
                                    });
                                    *expr = method_call;
                                }
                            } else if let Stmt::Expr(expr) = stmt {
                                if let Expr::Field(expr_field) = expr {
                                    let field_name = expr_field.member.clone();
                                    let method_name = Ident::new(
                                        &format!("get_{}", field_name.to_token_stream().to_string()),
                                        expr_field.member.span(),
                                    );
                                    let method_call = Expr::Call(ExprCall {
                                        attrs: vec![],
                                        func: Box::new(Expr::Path(ExprPath {
                                            attrs: vec![],
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: method_name,
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
                                            qself: None,
                                        })),
                                        paren_token: Paren {
                                            span: expr_field.member.span(),
                                        },
                                        args: punctuated::Punctuated::new(),
                                    });
                                    *expr = method_call;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces field access on non-ADT types with method calls. It generates a new method name based on the field name and replaces the field access with a method call. This transformation aims to test the compiler's handling of field access and method calls on non-ADT types."
    }
}