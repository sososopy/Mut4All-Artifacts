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

pub struct Pointer_Arithmetic_With_Type_Casting_226;

impl Mutator for Pointer_Arithmetic_With_Type_Casting_226 {
    fn name(&self) -> &str {
        "Pointer_Arithmetic_With_Type_Casting_226"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct UnsafeBlockFinder {
            unsafe_blocks: Vec<usize>,
        }

        impl<'ast> Visit<'ast> for UnsafeBlockFinder {
            fn visit_block(&mut self, block: &'ast Block) {
                if let Some(Stmt::Expr(expr, _)) = block.stmts.first() {
                    if let Expr::Unsafe(_) = expr {
                        self.unsafe_blocks.push(block.span().start().line);
                    }
                }
                syn::visit::visit_block(self, block);
            }
        }

        struct PointerArithmeticReplacer {
            target_line: usize,
        }

        impl VisitMut for PointerArithmeticReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if expr.span().start().line == self.target_line {
                    if let Expr::MethodCall(method_call) = expr {
                        if method_call.method == "add" || method_call.method == "sub" {
                            if method_call.args.len() == 1 {
                                let ptr = &method_call.receiver;
                                let offset = &method_call.args[0];
                                let is_add = method_call.method == "add";
                                let new_expr = if is_add {
                                    parse_quote! {
                                        {
                                            let ptr_as_usize = #ptr as usize;
                                            let new_address = ptr_as_usize + #offset * std::mem::size_of_val(&*#ptr);
                                            new_address as *const _
                                        }
                                    }
                                } else {
                                    parse_quote! {
                                        {
                                            let ptr_as_usize = #ptr as usize;
                                            let new_address = ptr_as_usize - #offset * std::mem::size_of_val(&*#ptr);
                                            new_address as *const _
                                        }
                                    }
                                };
                                *expr = new_expr;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut unsafe_block_finder = UnsafeBlockFinder {
            unsafe_blocks: Vec::new(),
        };
        unsafe_block_finder.visit_file(file);

        let mut rng = thread_rng();
        if let Some(&target_line) = unsafe_block_finder.unsafe_blocks.choose(&mut rng) {
            let mut replacer = PointerArithmeticReplacer { target_line };
            replacer.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets pointer arithmetic operations within unsafe blocks. It identifies unsafe blocks and replaces pointer addition or subtraction with an equivalent operation that involves casting the pointer to a usize, performing the arithmetic on the usize, and then casting back to a pointer. This transformation stresses the compiler's handling of pointer arithmetic, type casting, and memory safety checks, which are critical areas for detecting subtle bugs and edge cases."
    }
}