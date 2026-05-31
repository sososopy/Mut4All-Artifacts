use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated, spanned::Spanned, token, token::Comma, token::{Paren, Plus},
    visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Convert_Closures_To_Functions_27;

impl Mutator for Convert_Closures_To_Functions_27 {
    fn name(&self) -> &str {
        "Convert_Closures_To_Functions_27"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut functions_to_add = vec![];
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut index = 0;
                while index < func.block.stmts.len() {
                    if let Stmt::Expr(Expr::Closure(expr_closure)) = &func.block.stmts[index] {
                        let closure_ident = Ident::new(
                            &format!("closure_function_{}", rng.gen::<u32>()),
                            Span::call_site(),
                        );

                        let fn_decl = ItemFn {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            sig: syn::Signature {
                                constness: None,
                                asyncness: None,
                                unsafety: None,
                                abi: None,
                                fn_token: token::Fn { span: Span::call_site() },
                                ident: closure_ident.clone(),
                                generics: Default::default(),
                                paren_token: token::Paren { span: Span::call_site() },
                                inputs: expr_closure.inputs.clone(),
                                variadic: None,
                                output: expr_closure.output.clone(),
                            },
                            block: Box::new(syn::Block {
                                brace_token: expr_closure.brace_token,
                                stmts: vec![Stmt::Expr(*expr_closure.body.clone())],
                            }),
                        };

                        functions_to_add.push(Item::Fn(fn_decl));

                        func.block.stmts[index] = Stmt::Expr(Expr::Path(ExprPath {
                            attrs: vec![],
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![
                                    syn::PathSegment {
                                        ident: closure_ident,
                                        arguments: PathArguments::None,
                                    },
                                ]),
                            },
                        }));
                    }
                    index += 1;
                }
            }
        }

        file.items.extend(functions_to_add);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closure expressions and replaces them with equivalent function definitions. It defines new functions within the same scope and replaces closure calls with function calls. This checks the compiler's handling of type inference, trait implementation, and specialization when translating closures to regular functions."
    }
}