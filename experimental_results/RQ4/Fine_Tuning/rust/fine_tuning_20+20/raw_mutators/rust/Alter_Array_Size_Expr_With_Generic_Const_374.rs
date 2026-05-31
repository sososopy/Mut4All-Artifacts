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

pub struct Alter_Array_Size_Expr_With_Generic_Const_374;

impl Mutator for Alter_Array_Size_Expr_With_Generic_Const_374 {
    fn name(&self) -> &str {
        "Alter_Array_Size_Expr_With_Generic_Const_374"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let generic_consts: HashSet<_> = item_fn
                    .sig
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.ident.to_string())
                        } else {
                            None
                        }
                    })
                    .collect();

                let mut target_indices = Vec::new();

                for (index, input) in item_fn.sig.inputs.iter().enumerate() {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &*pat_type.ty {
                            if let Expr::Block(expr_block) = &type_array.len {
                                let mut has_generic_const = false;
                                for stmt in &expr_block.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Path(expr_path) = expr {
                                            if expr_path
                                                .path
                                                .segments
                                                .first()
                                                .map(|seg| seg.ident.to_string())
                                                .filter(|ident| generic_consts.contains(ident))
                                                .is_some()
                                            {
                                                has_generic_const = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if has_generic_const {
                                    target_indices.push(index);
                                }
                            }
                        }
                    }
                }

                if target_indices.is_empty() {
                    return;
                }

                let target_index = {
                    let mut rng = thread_rng();
                    *target_indices.choose(&mut rng).unwrap()
                };

                if let FnArg::Typed(pat_type) = &mut item_fn.sig.inputs[target_index] {
                    if let Type::Array(type_array) = &mut *pat_type.ty {
                        if let Expr::Block(expr_block) = &type_array.len {
                            let mut rng = thread_rng();
                            let new_expr = if rng.gen_bool(0.5) {
                                parse_quote! { N + 1 }
                            } else {
                                parse_quote! { N * 2 }
                            };
                            type_array.len = Expr::Block(ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block: Block {
                                    brace_token: token::Brace::default(),
                                    stmts: vec![Stmt::Expr(new_expr, None)],
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets array parameters in functions where the array size is specified using a const expression involving a generic const parameter. It modifies the const expression to include additional arithmetic operations on the generic const parameter, such as adding 1 or multiplying by 2. This transformation aims to stress-test the compiler's const evaluation and generic const handling by altering array size expressions in a way that remains syntactically valid but changes the runtime semantics."
    }
}