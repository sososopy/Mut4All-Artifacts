use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprGroup, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus, Minus, Star, Slash},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Macro_GAT_Const_Expr_138;

impl Mutator for Modify_Macro_GAT_Const_Expr_138 {
    fn name(&self) -> &str {
        "Modify_Macro_GAT_Const_Expr_138"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let const_params: Vec<_> = item_impl.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(param) = param {
                            Some(param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();

                if !const_params.is_empty() {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Type(assoc_type) = impl_item {
                            if let Type::Path(type_path) = &mut assoc_type.ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Const(expr) = arg {
                                                modify_expr(expr, &const_params);
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
        "The mutation operator targets macro-generated GAT trait implementations with const arithmetic in associated type definitions. It modifies the const expressions by adding a new arithmetic operation using the same const parameter, increasing the complexity of the expression. This stresses the compiler's handling of complex const expressions in GATs while preserving all existing types and variables."
    }
}

fn modify_expr(expr: &mut Expr, const_params: &[Ident]) {
    if let Expr::Group(group) = expr {
        let inner_expr = *group.expr.clone();
        let mut rng = thread_rng();
        let op = match rng.gen_range(0..4) {
            0 => syn::BinOp::Add(Plus::default()),
            1 => syn::BinOp::Sub(Minus::default()),
            2 => syn::BinOp::Mul(Star::default()),
            3 => syn::BinOp::Div(Slash::default()),
            _ => unreachable!(),
        };
        let param = const_params.choose(&mut rng).unwrap();

        let param_expr = Expr::Path(ExprPath {
            attrs: vec![],
            qself: None,
            path: SynPath::from(param.clone()),
        });

        let literal_two = Expr::Lit(ExprLit {
            attrs: vec![],
            lit: Lit::Int(syn::LitInt::new("2", group.span())),
        });

        let new_part = Expr::Binary(ExprBinary {
            attrs: vec![],
            left: Box::new(param_expr),
            op,
            right: Box::new(literal_two),
        });

        let bin_op = Expr::Binary(ExprBinary {
            attrs: vec![],
            left: Box::new(inner_expr),
            op,
            right: Box::new(new_part),
        });

        let new_group = Expr::Group(ExprGroup {
            attrs: vec![],
            expr: Box::new(bin_op),
        });

        *expr = new_group;
    }
}