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

pub struct Modify_GAT_Const_Expression_138;

impl Mutator for Modify_GAT_Const_Expression_138 {
    fn name(&self) -> &str {
        "Modify_GAT_Const_Expression_138"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = impl_item {
                        if let Type::Path(type_path) = &type_item.ty {
                            let segments = &type_path.path.segments;
                            if segments.len() >= 2 {
                                let last_segment = segments.last().unwrap();
                                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                    let mut new_args = args.clone();
                                    for arg in &mut new_args.args {
                                        if let GenericArgument::Const(const_expr) = arg {
                                            if let Expr::Binary(binary_expr) = const_expr {
                                                let new_op = match binary_expr.op {
                                                    BinOp::Add(_) => {
                                                        if rng.gen_bool(0.5) {
                                                            BinOp::Mul(token::Star::default())
                                                        } else {
                                                            BinOp::Sub(token::Minus::default())
                                                        }
                                                    }
                                                    BinOp::Mul(_) => {
                                                        if rng.gen_bool(0.5) {
                                                            BinOp::Add(token::Plus::default())
                                                        } else {
                                                            BinOp::Sub(token::Minus::default())
                                                        }
                                                    }
                                                    BinOp::Sub(_) => {
                                                        if rng.gen_bool(0.5) {
                                                            BinOp::Add(token::Plus::default())
                                                        } else {
                                                            BinOp::Mul(token::Star::default())
                                                        }
                                                    }
                                                    _ => continue,
                                                };
                                                let new_left = binary_expr.left.clone();
                                                let new_right = binary_expr.right.clone();
                                                let new_binary = ExprBinary {
                                                    attrs: binary_expr.attrs.clone(),
                                                    left: new_left,
                                                    op: new_op,
                                                    right: new_right,
                                                };
                                                *const_expr = Expr::Binary(new_binary);
                                            } else if let Expr::Lit(lit_expr) = const_expr {
                                                if let Lit::Int(int_lit) = &lit_expr.lit {
                                                    let new_val = int_lit.base10_digits().parse::<i64>().unwrap() + 1;
                                                    let new_lit = Lit::Int(LitInt::new(
                                                        &new_val.to_string(),
                                                        int_lit.span(),
                                                    ));
                                                    *const_expr = Expr::Lit(ExprLit {
                                                        attrs: lit_expr.attrs.clone(),
                                                        lit: new_lit,
                                                    });
                                                }
                                            }
                                        }
                                    }
                                    let new_segment = PathSegment {
                                        ident: last_segment.ident.clone(),
                                        arguments: PathArguments::AngleBracketed(new_args),
                                    };
                                    let mut new_segments = segments.clone();
                                    new_segments.pop();
                                    new_segments.push(new_segment);
                                    let new_path = SynPath {
                                        leading_colon: type_path.path.leading_colon,
                                        segments: new_segments,
                                    };
                                    type_item.ty = Type::Path(TypePath {
                                        qself: type_path.qself.clone(),
                                        path: new_path,
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with Generic Associated Types (GATs) that use const generic parameters in complex expressions. It modifies the const expression within the associated type definition, changing arithmetic operations (e.g., multiplication to addition or subtraction) or incrementing integer literals. This transformation aims to expose compiler bugs in handling late-bound regions, normalization, and const evaluation for GATs with const generics, particularly when generated via macro expansion."
    }
}