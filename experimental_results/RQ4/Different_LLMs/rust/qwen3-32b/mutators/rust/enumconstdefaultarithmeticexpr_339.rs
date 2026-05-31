use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprCast, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct EnumConstDefaultArithmeticExpr_339;

impl Mutator for EnumConstDefaultArithmeticExpr_339 {
    fn name(&self) -> &str {
        "EnumConstDefaultArithmeticExpr_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                for param in &mut enum_item.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let Some(default_expr) = &const_param.default {
                            let variants = &enum_item.variants;
                            let explicit_discriminants: Vec<_> = variants
                                .iter()
                                .filter(|v| v.discriminant.is_some())
                                .collect();
                            if explicit_discriminants.len() >= 2 {
                                let variant1 = &explicit_discriminants[0];
                                let variant2 = &explicit_discriminants[1];
                                let enum_ident = &enum_item.ident;
                                let path1 = create_variant_path(enum_ident, default_expr, &variant1.ident);
                                let path2 = create_variant_path(enum_ident, default_expr, &variant2.ident);
                                let cast1 = Expr::Cast(ExprCast {
                                    attrs: Vec::new(),
                                    expr: Box::new(Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: path1.path,
                                    })),
                                    as_token: token::As::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("u8", Span::call_site())),
                                    })),
                                });
                                let cast2 = Expr::Cast(ExprCast {
                                    attrs: Vec::new(),
                                    expr: Box::new(Expr::Path(syn::ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: path2.path,
                                    })),
                                    as_token: token::As::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("u8", Span::call_site())),
                                    })),
                                });
                                let expr = Expr::Binary(ExprBinary {
                                    attrs: Vec::new(),
                                    left: Box::new(cast1),
                                    op: syn::BinOp::Add(token::Plus::default()),
                                    right: Box::new(cast2),
                                });
                                const_param.default = Some(expr);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions with const generic parameters and default values. It replaces the default value with an arithmetic expression combining the discriminant values of two variants. This forces evaluation of const expressions during substitution and may trigger ICEs in generic const expression handling."
    }
}

fn create_variant_path(enum_ident: &Ident, default_expr: &Expr, variant_ident: &Ident) -> ExprPath {
    let path = SynPath {
        leading_colon: None,
        segments: {
            let mut segments = Punctuated::new();
            segments.push(PathSegment {
                ident: enum_ident.clone(),
                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                    colon2_token: None,
                    lt_token: token::Lt::default(),
                    args: {
                        let mut args = Punctuated::new();
                        args.push(GenericArgument::Const(default_expr.clone()));
                        args
                    },
                    gt_token: token::Gt::default(),
                }),
            });
            segments.push(PathSegment {
                ident: variant_ident.clone(),
                arguments: PathArguments::None,
            });
            segments
        },
    };
    ExprPath {
        attrs: Vec::new(),
        qself: None,
        path,
    }
}