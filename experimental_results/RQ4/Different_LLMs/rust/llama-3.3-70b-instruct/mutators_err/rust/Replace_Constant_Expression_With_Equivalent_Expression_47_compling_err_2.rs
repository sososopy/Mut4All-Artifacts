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

pub struct Replace_Constant_Expression_With_Equivalent_Expression_47;

impl Mutator for Replace_Constant_Expression_With_Equivalent_Expression_47 {
    fn name(&self) -> &str {
        "Replace_Constant_Expression_With_Equivalent_Expression_47"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Lit(lit) = expr {
                            let value = match lit.lit {
                                Lit::Int(int) => int.base10_parse::<i32>().unwrap(),
                                Lit::Float(float) => float.base10_parse::<f64>().unwrap() as i32,
                                _ => continue,
                            };

                            let equivalent_expr = self.generate_equivalent_expression(value);
                            *expr = equivalent_expr;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant expressions with equivalent but more complex expressions. This transformation tests the compiler's ability to handle constant evaluation in various contexts and may expose bugs in the constant evaluation mechanism."
    }
}

impl Replace_Constant_Expression_With_Equivalent_Expression_47 {
    fn generate_equivalent_expression(&self, value: i32) -> Expr {
        let mut rng = thread_rng();
        let op = ["+", "-", "*", "/"][rng.gen_range(0..4)];
        let num1 = rng.gen_range(1..=value);
        let num2 = match op {
            "+" => value - num1,
            "-" => value + num1,
            "*" => value / num1,
            "/" => value * num1,
            _ => unreachable!(),
        };

        let num1_lit = Lit::Int(IntLit {
            span: Span::call_site(),
            suffix: None,
            base: 10,
            digits: num1.to_string(),
        });

        let num2_lit = Lit::Int(IntLit {
            span: Span::call_site(),
            suffix: None,
            base: 10,
            digits: num2.to_string(),
        });

        let num1_expr = Expr::Lit(num1_lit);

        let num2_expr = Expr::Lit(num2_lit);

        let op_token = match op {
            "+" => token::Plus(Span::call_site()),
            "-" => token::Minus(Span::call_site()),
            "*" => token::Star(Span::call_site()),
            "/" => token::Slash(Span::call_site()),
            _ => unreachable!(),
        };

        Expr::Binary(ExprBinary {
            attrs: vec![],
            left: Box::new(num1_expr),
            op: op_token,
            right: Box::new(num2_expr),
        })
    }
}