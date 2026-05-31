use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, ExprBlock, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Add},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Const_Generic_Default_With_Enum_Arithmetic_80;

impl Mutator for Replace_Const_Generic_Default_With_Enum_Arithmetic_80 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Default_With_Enum_Arithmetic_80"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut eligible_enums = Vec::new();
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants_with_discriminant = Vec::new();
                for variant in &enum_item.variants {
                    if let Some((_, expr)) = &variant.discriminant {
                        variants_with_discriminant.push((variant.ident.clone(), expr.clone()));
                    }
                }
                if variants_with_discriminant.len() >= 2 {
                    eligible_enums.push((enum_item.ident.clone(), variants_with_discriminant));
                }
            }
        }

        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if eligible_enums.is_empty() {
                                continue;
                            }
                            let mut rng = thread_rng();
                            if let Some((enum_ident, variants)) = eligible_enums.choose(&mut rng) {
                                let variant1 = variants.choose(&mut rng).unwrap();
                                let variant2 = variants.choose(&mut rng).unwrap();
                                let expr = {
                                    let variant1_path = parse_quote! { #enum_ident :: #variant1.0 };
                                    let variant2_path = parse_quote! { #enum_ident :: #variant2.0 };

                                    let expr1 = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: variant1_path,
                                    });

                                    let expr2 = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: variant2_path,
                                    });

                                    let cast1 = ExprCast {
                                        attrs: vec![],
                                        expr: Box::new(expr1),
                                        as_token: token::As::default(),
                                        ty: Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: parse_quote! { u8 },
                                        })),
                                    };

                                    let cast2 = ExprCast {
                                        attrs: vec![],
                                        expr: Box::new(expr2),
                                        as_token: token::As::default(),
                                        ty: Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: parse_quote! { u8 },
                                        })),
                                    };

                                    let op = BinOp::Add(Add::default());
                                    let expr_binary = Expr::Binary(ExprBinary {
                                        attrs: vec![],
                                        left: Box::new(cast1),
                                        op,
                                        right: Box::new(cast2),
                                    });

                                    Expr::Block(ExprBlock {
                                        attrs: vec![],
                                        label: None,
                                        block: parse_quote! { { #expr_binary } },
                                    })
                                };
                                *default = Box::new(expr);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces default values of const generic parameters with arithmetic expressions involving enum variant values. This creates recursive evaluation contexts that stress the compiler's const evaluation and dependency resolution systems, potentially exposing bugs in complex type and const interactions."
    }
}