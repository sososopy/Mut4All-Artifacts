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

pub struct Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362;

impl Mutator for Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362 {
    fn name(&self) -> &str {
        "Replace_Generic_Constant_Expression_With_Non_Generic_Constant_Expression_362"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(item_type) = impl_item {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }) = *item_type.ty
                        {
                            for segment in segments {
                                if let PathSegment {
                                    ident,
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: _,
                                        args,
                                        gt_token: _,
                                    }),
                                } = segment
                                {
                                    for arg in args {
                                        if let GenericArgument::Const(Expr::Binary(ExprBinary {
                                            op: BinOp::Add(_),
                                            left: Expr::Const(ExprConst {
                                                attrs: _,
                                                const_token: _,
                                                expr: _,
                                            }),
                                            right: Expr::Const(ExprConst {
                                                attrs: _,
                                                const_token: _,
                                                expr: _,
                                            }),
                                        })) = arg
                                        {
                                            *arg = GenericArgument::Const(Expr::Lit(ExprLit {
                                                attrs: Vec::new(),
                                                lit: Lit::Int(LitInt {
                                                    value: 10,
                                                }),
                                            }));
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
        "The mutation operator replaces generic constant expressions with non-generic constant expressions in type aliases within impl blocks. It aims to test the compiler's handling of constant expressions and type aliases, potentially triggering bugs related to type checking or code generation."
    }
}