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

pub struct Replace_Const_Generic_Expr_With_Different_Operator_85;

impl Mutator for Replace_Const_Generic_Expr_With_Different_Operator_85 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Expr_With_Different_Operator_85"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                        },
                    }) = **return_type {
                        for segment in &mut segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for (index, arg) in args.args.iter().enumerate() {
                                    if let GenericArgument::Const(Expr::Binary(ExprBinary {
                                        left,
                                        op,
                                        right,
                                        ..
                                    })) = arg {
                                        let new_op = match op {
                                            BinOp::Add(_) => BinOp::Mul(token::Star::default()),
                                            BinOp::Mul(_) => BinOp::Add(token::Plus::default()),
                                            _ => continue,
                                        };
                                        let new_expr = Expr::Binary(ExprBinary {
                                            left: left.clone(),
                                            op: new_op,
                                            right: right.clone(),
                                            attrs: vec![],
                                        });
                                        let new_arg = GenericArgument::Const(new_expr);
                                        args.args[index] = new_arg;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = **return_type {
                                for segment in &mut segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for (index, arg) in args.args.iter().enumerate() {
                                            if let GenericArgument::Const(Expr::Binary(ExprBinary {
                                                left,
                                                op,
                                                right,
                                                ..
                                            })) = arg {
                                                let new_op = match op {
                                                    BinOp::Add(_) => BinOp::Mul(token::Star::default()),
                                                    BinOp::Mul(_) => BinOp::Add(token::Plus::default()),
                                                    _ => continue,
                                                };
                                                let new_expr = Expr::Binary(ExprBinary {
                                                    left: left.clone(),
                                                    op: new_op,
                                                    right: right.clone(),
                                                    attrs: vec![],
                                                });
                                                let new_arg = GenericArgument::Const(new_expr);
                                                args.args[index] = new_arg;
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
        "The mutation operator replaces constant generic expressions with different operators, aiming to test the compiler's handling of constant evaluation and generic types."
    }
}