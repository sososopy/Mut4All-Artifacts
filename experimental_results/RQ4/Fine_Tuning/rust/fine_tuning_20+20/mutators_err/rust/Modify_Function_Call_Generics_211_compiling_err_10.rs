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

pub struct Modify_Function_Call_Generics_211;

impl Mutator for Modify_Function_Call_Generics_211 {
    fn name(&self) -> &str {
        "Modify_Function_Call_Generics_211"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct FunctionInfo {
            name: String,
            generics_count: usize,
            where_clause: Option<WhereClause>,
        }

        struct FunctionCollector {
            functions: Vec<FunctionInfo>,
        }

        impl<'ast> Visit<'ast> for FunctionCollector {
            fn visit_item_fn(&mut self, node: &'ast ItemFn) {
                self.functions.push(FunctionInfo {
                    name: node.sig.ident.to_string(),
                    generics_count: node.sig.generics.params.len(),
                    where_clause: node.sig.generics.where_clause.clone(),
                });
            }

            fn visit_impl_item_fn(&mut self, node: &'ast ImplItemFn) {
                self.functions.push(FunctionInfo {
                    name: node.sig.ident.to_string(),
                    generics_count: node.sig.generics.params.len(),
                    where_clause: node.sig.generics.where_clause.clone(),
                });
            }
        }

        struct FunctionCallMutator<'a> {
            functions: &'a [FunctionInfo],
        }

        impl VisitMut for FunctionCallMutator<'_> {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    let function_name = path.segments.last().unwrap().ident.to_string();
                    if let Some(function_info) = self
                        .functions
                        .iter()
                        .find(|info| info.name == function_name)
                    {
                        let required_generics_count = function_info.generics_count;
                        if let PathArguments::AngleBracketed(ref mut args) =
                            node.func.as_mut().as_path_mut().unwrap().segments.last_mut().unwrap().arguments
                        {
                            let current_generics_count = args.args.len();
                            if current_generics_count < required_generics_count {
                                let missing_count =
                                    required_generics_count - current_generics_count;
                                let mut added_types = HashSet::new();
                                for arg in &args.args {
                                    if let GenericArgument::Type(ty) = arg {
                                        added_types.insert(ty.to_token_stream().to_string());
                                    }
                                }
                                if let Some(where_clause) = &function_info.where_clause {
                                    for predicate in &where_clause.predicates {
                                        if let WherePredicate::Type(predicate_type) = predicate {
                                            for bound in &predicate_type.bounds {
                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                    let trait_name = trait_bound
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string();
                                                    if trait_name == "Fn" {
                                                        args.args.push(parse_quote! {
                                                            fn()
                                                        });
                                                        added_types.insert("fn()".to_string());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                for _ in 0..missing_count {
                                    if !added_types.contains("usize") {
                                        args.args.push(parse_quote! { usize });
                                        added_types.insert("usize".to_string());
                                    } else if !added_types.contains("i32") {
                                        args.args.push(parse_quote! { i32 });
                                        added_types.insert("i32".to_string());
                                    } else if !added_types.contains("bool") {
                                        args.args.push(parse_quote! { bool });
                                        added_types.insert("bool".to_string());
                                    } else if !added_types.contains("char") {
                                        args.args.push(parse_quote! { char });
                                        added_types.insert("char".to_string());
                                    } else {
                                        args.args.push(parse_quote! { () });
                                    }
                                }
                            }
                        } else if required_generics_count > 0 {
                            let mut args: Punctuated<GenericArgument, Comma> = Punctuated::new();
                            for _ in 0..required_generics_count {
                                args.push(parse_quote! { () });
                            }
                            node.func.as_mut().as_path_mut().unwrap().segments.last_mut().unwrap().arguments =
                                PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: Default::default(),
                                    args,
                                    gt_token: Default::default(),
                                });
                        }
                    }
                }
            }
        }

        let mut collector = FunctionCollector {
            functions: Vec::new(),
        };
        collector.visit_file(file);

        let mut mutator = FunctionCallMutator {
            functions: &collector.functions,
        };
        mutator.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function calls with generic parameters. It ensures that the number of generic arguments in the call matches the function definition by adding missing type arguments. If the function has a where clause with a `Fn` bound, it prioritizes adding a `fn()` type. Otherwise, it fills in common types like `usize`, `i32`, `bool`, and `char`, or defaults to `()` if needed. This transformation stresses the compiler's generic argument resolution and type inference mechanisms."
    }
}