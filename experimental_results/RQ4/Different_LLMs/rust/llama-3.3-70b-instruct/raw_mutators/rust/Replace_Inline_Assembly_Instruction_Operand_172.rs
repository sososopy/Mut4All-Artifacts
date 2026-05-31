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

pub struct Replace_Inline_Assembly_Instruction_Operand_172;

impl Mutator for Replace_Inline_Assembly_Instruction_Operand_172 {
    fn name(&self) -> &str {
        "Replace_Inline_Assembly_Instruction_Operand_172"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Macro(mac) = expr {
                            if mac.mac.path.is_ident("asm") {
                                let mut new_tokens = mac.mac.tokens.clone();
                                let mut operand_found = false;
                                let mut new_operand = 0;
                                for token in &mut new_tokens {
                                    if let TokenTree::Literal(lit) = token {
                                        if let Literal::Integer(int) = lit {
                                            operand_found = true;
                                            new_operand = int.base10_parse::<i32>().unwrap() + 1;
                                            *lit = Literal::integer(new_operand);
                                            break;
                                        }
                                    }
                                }
                                if operand_found {
                                    mac.mac.tokens = new_tokens;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly instructions in Rust code and replaces the operand of the instruction with a different one. This transformation aims to test the robustness of the Rust compiler's handling of inline assembly instructions and their operands."
    }
}