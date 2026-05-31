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

pub struct Insert_Invalid_Const_In_Global_Asm_426;

impl Mutator for Insert_Invalid_Const_In_Global_Asm_426 {
    fn name(&self) -> &str {
        "Insert_Invalid_Const_In_Global_Asm_426"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = GlobalAsmVisitor {
            const_items: Vec::new(),
        };
        visitor.visit_file(file);
        let const_names: Vec<Ident> = visitor.const_items.iter().map(|(ident, _)| ident.clone()).collect();
        let mut visitor_mut = GlobalAsmMutator {
            const_names,
            rng: thread_rng(),
        };
        visitor_mut.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets global_asm! macro invocations. It scans the program for const items to use in constructing erroneous constant expressions. For each global_asm! macro, if a const argument exists, its expression is replaced with a division-by-zero error using either a found const item or a literal. If no const argument exists, a new const argument with an erroneous expression is inserted. This transformation aims to trigger constant evaluation errors during asm constant resolution, potentially exposing internal compiler errors."
    }
}

struct GlobalAsmVisitor {
    const_items: Vec<(Ident, Type)>,
}

impl Visit<'_> for GlobalAsmVisitor {
    fn visit_item_const(&mut self, node: &syn::ItemConst) {
        self.const_items.push((node.ident.clone(), *node.ty.clone()));
        syn::visit::visit_item_const(self, node);
    }
}

struct GlobalAsmMutator {
    const_names: Vec<Ident>,
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for GlobalAsmMutator {
    fn visit_expr_macro_mut(&mut self, node: &mut syn::ExprMacro) {
        if node.mac.path.is_ident("global_asm") {
            let tokens = &mut node.mac.tokens;
            let token_stream = tokens.clone();
            let mut parser = syn::parse::Parser::parse2(
                |input: syn::parse::ParseStream| {
                    let mut args = Punctuated::<syn::Expr, Token![,]>::new();
                    while !input.is_empty() {
                        let expr: syn::Expr = input.parse()?;
                        args.push(expr);
                        if input.is_empty() {
                            break;
                        }
                        let _: Token![,] = input.parse()?;
                    }
                    Ok(args)
                },
                token_stream,
            );
            if let Ok(mut args) = parser {
                let mut has_const_arg = false;
                let mut const_index = None;
                for (i, arg) in args.iter().enumerate() {
                    if let syn::Expr::Assign(assign) = arg {
                        if let syn::Expr::Path(path) = &*assign.left {
                            if path.path.is_ident("const") {
                                has_const_arg = true;
                                const_index = Some(i);
                                break;
                            }
                        }
                    }
                }
                let erroneous_expr = if self.const_names.is_empty() {
                    parse_quote!(1 / 0)
                } else {
                    let const_name = self.const_names.choose(&mut self.rng).unwrap();
                    parse_quote!(#const_name / 0)
                };
                if has_const_arg {
                    if let Some(idx) = const_index {
                        if let syn::Expr::Assign(assign) = &mut args[idx] {
                            assign.right = Box::new(erroneous_expr);
                        }
                    }
                } else {
                    let new_const_arg: syn::Expr = parse_quote!(const = #erroneous_expr);
                    args.push(new_const_arg);
                }
                let new_tokens = quote! { #args };
                node.mac.tokens = new_tokens;
            }
        }
        syn::visit_mut::visit_expr_macro_mut(self, node);
    }
}