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

pub struct Inline_Assembly_Excess_Register_Access_176;

impl Mutator for Inline_Assembly_Excess_Register_Access_176 {
    fn name(&self) -> &str {
        "Inline_Assembly_Excess_Register_Access_176"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let is_naked = func.attrs.iter().any(|attr| attr.path().is_ident("naked"));
                if is_naked {
                    let has_asm = func.block.stmts.iter().any(|stmt| {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Macro(macro_expr) = expr {
                                macro_expr.path.is_ident("asm")
                            } else {
                                false
                            }
                        } else {
                            false
                        }
                    });
                    if has_asm {
                        let num_params = func.sig.inputs.len();
                        if num_params > 0 {
                            let registers = ["rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"];
                            let extra_register = if num_params < registers.len() {
                                registers[num_params]
                            } else {
                                "r15"
                            };
                            let asm_code = format!("add rax, {}", extra_register);
                            let new_asm_expr = parse_quote! {
                                asm!(#asm_code);
                            };
                            func.block.stmts.push(Stmt::Expr(new_asm_expr, None));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets naked functions with inline assembly, inserting an additional inline assembly instruction that accesses a register corresponding to an undeclared parameter. This forces the compiler to reference a non-existent operand during code generation, potentially exposing index out-of-bounds errors in operand resolution. The transformation leverages x86_64 register ordering (rdi, rsi, rdx, ...) to simulate excess parameter access, challenging the compiler's handling of register allocation and operand indexing in inline assembly contexts."
    }
}