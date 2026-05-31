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

pub struct Pointer_Arithmetic_With_Type_Casting_348;

impl Mutator for Pointer_Arithmetic_With_Type_Casting_348 {
    fn name(&self) -> &str {
        "Pointer_Arithmetic_With_Type_Casting_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct UnsafeBlockVisitor;

        impl VisitMut for UnsafeBlockVisitor {
            fn visit_block_mut(&mut self, block: &mut syn::Block) {
                let mut new_stmts = Vec::new();
                for stmt in &block.stmts {
                    if let Stmt::Expr(Expr::MethodCall(method_call), _) = stmt {
                        if method_call.method == "add" {
                            if let Expr::Cast(cast_expr) = &*method_call.receiver {
                                let ptr_type = &cast_expr.ty;
                                let ptr_expr = &cast_expr.expr;
                                let offset = &method_call.args[0];

                                let ptr_as_usize: Stmt = parse_quote! {
                                    let ptr_as_usize = #ptr_expr as usize;
                                };
                                let new_address: Stmt = parse_quote! {
                                    let new_address = ptr_as_usize + #offset * std::mem::size_of::<#ptr_type>();
                                };
                                let new_ptr: Stmt = parse_quote! {
                                    let new_ptr = new_address as #ptr_type;
                                };

                                new_stmts.push(ptr_as_usize);
                                new_stmts.push(new_address);
                                new_stmts.push(new_ptr);
                                continue;
                            }
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                block.stmts = new_stmts;
                visit_mut::visit_block_mut(self, block);
            }
        }

        UnsafeBlockVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets pointer arithmetic operations in unsafe blocks. It replaces direct pointer arithmetic with a combination of arithmetic and type casting to a different pointer type. The process involves casting a pointer to a usize, performing arithmetic on the usize, and then casting it back to the original pointer type. This approach increases the likelihood of encountering edge cases related to pointer arithmetic and type casting, which are common triggers for compiler bugs, particularly those related to memory and type safety."
    }
}