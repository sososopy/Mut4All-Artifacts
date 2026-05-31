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

pub struct Mismatch_Inline_Assembly_Output_Operands_176;

impl Mutator for Mismatch_Inline_Assembly_Output_Operands_176 {
    fn name(&self) -> &str {
        "Mismatch_Inline_Assembly_Output_Operands_176"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let is_naked = func.attrs.iter().any(|attr| {
                    attr.path().segments.last().map(|seg| seg.ident.to_string()) == Some("naked".to_string())
                });
                if is_naked {
                    let mut has_asm = false;
                    for stmt in &func.block.stmts {
                        if let Stmt::Expr(expr, _) = stmt {
                            if let Expr::Macro(macro_expr) = expr {
                                if macro_expr.mac.path.segments.last().map(|seg| seg.ident.to_string()) == Some("asm".to_string()) {
                                    has_asm = true;
                                    break;
                                }
                            }
                        }
                    }
                    if has_asm {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Macro(macro_expr) = expr {
                                    if macro_expr.mac.path.segments.last().map(|seg| seg.ident.to_string()) == Some("asm".to_string()) {
                                        let mut new_macro = macro_expr.mac.clone();
                                        let mut tokens = new_macro.tokens.clone();
                                        let mut new_tokens = proc_macro2::TokenStream::new();
                                        let mut found_noreturn = false;
                                        let mut output_count = 0;
                                        let mut iter = tokens.clone().into_iter();
                                        while let Some(token) = iter.next() {
                                            if let proc_macro2::TokenTree::Ident(ident) = &token {
                                                if ident.to_string() == "options" {
                                                    let next_token = iter.next();
                                                    if let Some(proc_macro2::TokenTree::Group(group)) = next_token {
                                                        let group_stream = group.stream();
                                                        let mut group_iter = group_stream.into_iter();
                                                        let mut new_group_stream = proc_macro2::TokenStream::new();
                                                        while let Some(group_token) = group_iter.next() {
                                                            if let proc_macro2::TokenTree::Ident(group_ident) = &group_token {
                                                                if group_ident.to_string() == "noreturn" {
                                                                    found_noreturn = true;
                                                                    new_group_stream.extend(proc_macro2::TokenStream::from(group_token));
                                                                } else {
                                                                    new_group_stream.extend(proc_macro2::TokenStream::from(group_token));
                                                                }
                                                            } else {
                                                                new_group_stream.extend(proc_macro2::TokenStream::from(group_token));
                                                            }
                                                        }
                                                        if found_noreturn {
                                                            let new_group = proc_macro2::Group::new(group.delimiter(), new_group_stream);
                                                            new_tokens.extend(proc_macro2::TokenStream::from(token));
                                                            new_tokens.extend(proc_macro2::TokenStream::from(proc_macro2::TokenTree::Group(new_group)));
                                                        } else {
                                                            new_tokens.extend(proc_macro2::TokenStream::from(token));
                                                            new_tokens.extend(proc_macro2::TokenStream::from(proc_macro2::TokenTree::Group(group)));
                                                        }
                                                    } else {
                                                        new_tokens.extend(proc_macro2::TokenStream::from(token));
                                                        if let Some(next_token) = next_token {
                                                            new_tokens.extend(proc_macro2::TokenStream::from(next_token));
                                                        }
                                                    }
                                                } else if ident.to_string() == "out" {
                                                    output_count += 1;
                                                    new_tokens.extend(proc_macro2::TokenStream::from(token));
                                                } else {
                                                    new_tokens.extend(proc_macro2::TokenStream::from(token));
                                                }
                                            } else {
                                                new_tokens.extend(proc_macro2::TokenStream::from(token));
                                            }
                                        }
                                        if found_noreturn {
                                            let return_type_count = match &func.sig.output {
                                                ReturnType::Default => 0,
                                                ReturnType::Type(_, ty) => {
                                                    if let Type::Tuple(tuple) = &**ty {
                                                        tuple.elems.len()
                                                    } else {
                                                        1
                                                    }
                                                }
                                            };
                                            let mismatch_count = if output_count == return_type_count {
                                                if return_type_count == 0 {
                                                    2
                                                } else {
                                                    return_type_count + 1
                                                }
                                            } else {
                                                if rng.gen_bool(0.5) {
                                                    if return_type_count == 0 {
                                                        2
                                                    } else {
                                                        return_type_count + 1
                                                    }
                                                } else {
                                                    if return_type_count > 1 {
                                                        return_type_count - 1
                                                    } else {
                                                        0
                                                    }
                                                }
                                            };
                                            if mismatch_count != output_count {
                                                let mut new_output_tokens = proc_macro2::TokenStream::new();
                                                for i in —mismatch_count {
                                                    let reg = if i == 0 { "rax" } else if i == 1 { "rdx" } else if i == 2 { "rcx" } else { "r8" };
                                                    new_output_tokens.extend(quote! { out(#reg) _, });
                                                }
                                                let mut final_tokens = proc_macro2::TokenStream::new();
                                                let mut iter = new_tokens.clone().into_iter();
                                                let mut in_output_section = false;
                                                let mut output_added = false;
                                                while let Some(token) = iter.next() {
                                                    if let proc_macro2::TokenTree::Ident(ident) = &token {
                                                        if ident.to_string() == "out" {
                                                            in_output_section = true;
                                                            if !output_added {
                                                                final_tokens.extend(new_output_tokens.clone());
                                                                output_added = true;
                                                            }
                                                            continue;
                                                        }
                                                    }
                                                    if in_output_section {
                                                        if let proc_macro2::TokenTree::Punct(punct) = &token {
                                                            if punct.as_char() == ',' {
                                                                in_output_section = false;
                                                            }
                                                        }
                                                        continue;
                                                    }
                                                    final_tokens.extend(proc_macro2::TokenStream::from(token));
                                                }
                                                if !output_added {
                                                    let template_tokens = quote! { "mov rax, 0", "mov rdx, 0", "ret", };
                                                    let mut new_final_tokens = proc_macro2::TokenStream::new();
                                                    new_final_tokens.extend(template_tokens);
                                                    new_final_tokens.extend(new_output_tokens);
                                                    new_final_tokens.extend(quote! { options(noreturn) });
                                                    final_tokens = new_final_tokens;
                                                }
                                                new_macro.tokens = final_tokens;
                                                macro_expr.mac = new_macro;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if func.sig.ident != "main" && rng.gen_bool(0.3) {
                        let mut new_func = func.clone();
                        new_func.attrs.push(parse_quote!(#[naked]));
                        let return_type_count = match &func.sig.output {
                            ReturnType::Default => 0,
                            ReturnType::Type(_, ty) => {
                                if let Type::Tuple(tuple) = &**ty {
                                    tuple.elems.len()
                                } else {
                                    1
                                }
                            }
                        };
                        let mismatch_count = if return_type_count == 0 {
                            2
                        } else {
                            return_type_count + 1
                        };
                        let mut output_tokens = proc_macro2::TokenStream::new();
                        for i in —mismatch_count {
                            let reg = if i == 0 { "rax" } else if i == 1 { "rdx" } else if i == 2 { "rcx" } else { "r8" };
                            output_tokens.extend(quote! { out(#reg) _, });
                        }
                        let asm_tokens = quote! {
                            asm!("mov rax, 0", "mov rdx, 0", "ret", #output_tokens options(noreturn))
                        };
                        let asm_expr: Expr = parse_quote! { #asm_tokens };
                        let mut new_block = func.block.clone();
                        new_block.stmts.clear();
                        new_block.stmts.push(Stmt::Expr(asm_expr, None));
                        new_func.block = new_block;
                        *item = Item::Fn(new_func);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets naked functions with inline assembly blocks using the noreturn option. It modifies the asm! macro to have output operands that mismatch the function's return type arity, creating a discrepancy that can trigger index out-of-bounds errors during code generation. If no naked function exists, it introduces one by copying an existing function signature and adding #[naked] and an asm! block with mismatched outputs."
    }
}