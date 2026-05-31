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

pub struct Inline_Assembly_Operand_Type_Mutation_With_Erroneous_Type_Context_170;

impl Mutator for Inline_Assembly_Operand_Type_Mutation_With_Erroneous_Type_Context_170 {
    fn name(&self) -> &str {
        "Inline_Assembly_Operand_Type_Mutation_With_Erroneous_Type_Context_170"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_asm = false;
                let mut asm_operands = Vec::new();
                let mut stmts_to_insert = Vec::new();
                let mut error_var_name = None;

                for stmt in &item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Macro(macro_expr) = expr {
                            if macro_expr.mac.path.is_ident("asm") || macro_expr.mac.path.is_ident("llvm_asm") {
                                has_asm = true;
                                let tokens = macro_expr.mac.tokens.clone();
                                let parsed = syn::parse2::<syn::Expr>(tokens).ok();
                                if let Some(Expr::Call(call)) = parsed {
                                    for arg in &call.args {
                                        if let Expr::Assign(assign) = arg {
                                            if let Expr::Path(path) = &*assign.left {
                                                if path.path.segments.len() == 1 {
                                                    let ident = &path.path.segments[0].ident;
                                                    if ident == "in" || ident == "out" || ident == "inout" {
                                                        asm_operands.push((stmt, assign.right.clone()));
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

                if has_asm && !asm_operands.is_empty() {
                    let error_type_name = Ident::new("UndefinedType", Span::call_site());
                    let error_var = Ident::new("error_val", Span::call_site());
                    error_var_name = Some(error_var.clone());

                    let error_decl: Stmt = parse_quote! {
                        let #error_var: #error_type_name = unsafe { std::mem::zeroed() };
                    };
                    stmts_to_insert.push(error_decl);

                    for (stmt, operand_expr) in asm_operands {
                        if let Stmt::Expr(Expr::Macro(macro_expr), _) = stmt {
                            let tokens = macro_expr.mac.tokens.clone();
                            let parsed = syn::parse2::<syn::Expr>(tokens).ok();
                            if let Some(Expr::Call(call)) = parsed {
                                let mut new_args: Punctuated<Expr, Comma> = Punctuated::new();
                                for arg in &call.args {
                                    if let Expr::Assign(assign) = arg {
                                        if let Expr::Path(path) = &*assign.left {
                                            if path.path.segments.len() == 1 {
                                                let ident = &path.path.segments[0].ident;
                                                if ident == "in" || ident == "out" || ident == "inout" {
                                                    let new_assign: Expr = parse_quote! {
                                                        #ident = #error_var
                                                    };
                                                    new_args.push(new_assign);
                                                } else {
                                                    new_args.push(arg.clone());
                                                }
                                            } else {
                                                new_args.push(arg.clone());
                                            }
                                        } else {
                                            new_args.push(arg.clone());
                                        }
                                    } else {
                                        new_args.push(arg.clone());
                                    }
                                }
                                let new_call: Expr = parse_quote! {
                                    (#new_args)
                                };
                                let new_macro = ExprMacro {
                                    attrs: macro_expr.attrs.clone(),
                                    mac: syn::Macro {
                                        path: macro_expr.mac.path.clone(),
                                        bang_token: macro_expr.mac.bang_token,
                                        delimiter: macro_expr.mac.delimiter,
                                        tokens: new_call.to_token_stream(),
                                    },
                                };
                                *stmt = Stmt::Expr(Expr::Macro(new_macro), None);
                            }
                        }
                    }

                    item_fn.block.stmts.splice(0..0, stmts_to_insert);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions containing inline assembly blocks (asm! or llvm_asm!). It introduces a type error by declaring a variable of an undefined type (UndefinedType) using unsafe zeroed initialization. Then, it mutates all input/output operands in the assembly to reference this erroneous variable. This creates a scenario where the compiler type-checks inline assembly operands within a context tainted by earlier type errors, testing error recovery and type resolution edge cases."
    }
}