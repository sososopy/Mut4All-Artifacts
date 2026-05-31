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

pub struct Modify_Const_Generic_Default_Expression_387;

impl Mutator for Modify_Const_Generic_Default_Expression_387 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_Expression_387"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let operations = vec!["-", "*", "&", "|", "^"];
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    self.mutate_generics(&mut item_fn.sig.generics, &operations, &mut rng);
                }
                Item::Impl(item_impl) => {
                    self.mutate_generics(&mut item_impl.generics, &operations, &mut rng);
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            self.mutate_generics(&mut method.sig.generics, &operations, &mut rng);
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    self.mutate_generics(&mut item_trait.generics, &operations, &mut rng);
                }
                Item::Struct(item_struct) => {
                    self.mutate_generics(&mut item_struct.generics, &operations, &mut rng);
                }
                Item::Enum(item_enum) => {
                    self.mutate_generics(&mut item_enum.generics, &operations, &mut rng);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const parameters with default values that are simple arithmetic expressions depending on another const generic parameter bound in the same for binder. It modifies the default expression by replacing the arithmetic operation with a different one (subtraction, multiplication, bitwise AND, OR, XOR) or introducing a more complex expression while maintaining dependency on the earlier parameter. This aims to trigger internal compiler errors related to constant evaluation and map lookups in the THIR or MIR construction phases by altering the const generic default expressions in where-clauses."
    }
}

impl Modify_Const_Generic_Default_Expression_387 {
    fn mutate_generics<R: Rng>(&self, generics: &mut Generics, operations: &Vec<&str>, rng: &mut R) {
        if let Some(where_clause) = &mut generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let WherePredicate::Type(predicate_type) = predicate {
                    if let Type::TraitObject(trait_object) = &mut predicate_type.bounded_ty {
                        if let Some(BoundLifetimes { lifetimes, .. }) = &mut trait_object.bounds.first_mut().and_then(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.lifetimes.as_mut()
                            } else {
                                None
                            }
                        }) {
                            self.mutate_for_lifetimes(lifetimes, operations, rng);
                        }
                    }
                }
            }
        }
        for param in &mut generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default) = &mut const_param.default {
                    self.mutate_const_default(default, operations, rng);
                }
            }
        }
    }

    fn mutate_for_lifetimes<R: Rng>(&self, lifetimes: &mut Punctuated<LifetimeParam, Comma>, operations: &Vec<&str>, rng: &mut R) {
        for lifetime_param in lifetimes.iter_mut() {
            if let Some(bounds) = &mut lifetime_param.bounds {
                for bound in bounds.iter_mut() {
                    if let GenericParamBound::Lifetime(lifetime) = bound {
                        // This is a lifetime bound, not relevant for const generics
                    }
                }
            }
        }
    }

    fn mutate_const_default<R: Rng>(&self, default: &mut Expr, operations: &Vec<&str>, rng: &mut R) {
        if let Expr::Block(expr_block) = default {
            if let Some(stmt) = expr_block.block.stmts.last_mut() {
                if let Stmt::Expr(expr, _) = stmt {
                    self.mutate_arithmetic_expression(expr, operations, rng);
                }
            }
        } else {
            self.mutate_arithmetic_expression(default, operations, rng);
        }
    }

    fn mutate_arithmetic_expression<R: Rng>(&self, expr: &mut Expr, operations: &Vec<&str>, rng: &mut R) {
        match expr {
            Expr::Binary(expr_binary) => {
                if matches!(expr_binary.op, BinOp::Add(_) | BinOp::Sub(_) | BinOp::Mul(_) | BinOp::BitAnd(_) | BinOp::BitOr(_) | BinOp::BitXor(_)) {
                    let new_op = operations.choose(rng).unwrap();
                    match *new_op {
                        "-" => expr_binary.op = BinOp::Sub(Default::default()),
                        "*" => expr_binary.op = BinOp::Mul(Default::default()),
                        "&" => expr_binary.op = BinOp::BitAnd(Default::default()),
                        "|" => expr_binary.op = BinOp::BitOr(Default::default()),
                        "^" => expr_binary.op = BinOp::BitXor(Default::default()),
                        _ => {}
                    }
                    if *new_op == "*" {
                        if let Expr::Lit(expr_lit) = &mut *expr_binary.right {
                            if let Lit::Int(lit_int) = &mut expr_lit.lit {
                                let current_val = lit_int.base10_parse::<usize>().unwrap_or(1);
                                lit_int.token = proc_macro2::Literal::usize_suffixed(current_val * 2);
                            }
                        }
                    }
                }
                self.mutate_arithmetic_expression(&mut expr_binary.left, operations, rng);
                self.mutate_arithmetic_expression(&mut expr_binary.right, operations, rng);
            }
            Expr::Paren(expr_paren) => {
                self.mutate_arithmetic_expression(&mut expr_paren.expr, operations, rng);
            }
            Expr::Unary(expr_unary) => {
                self.mutate_arithmetic_expression(&mut expr_unary.expr, operations, rng);
            }
            _ => {}
        }
    }
}