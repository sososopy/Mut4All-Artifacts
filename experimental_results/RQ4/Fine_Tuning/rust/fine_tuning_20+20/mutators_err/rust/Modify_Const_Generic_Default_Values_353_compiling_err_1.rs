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

pub struct Modify_Const_Generic_Default_Values_353;

impl Mutator for Modify_Const_Generic_Default_Values_353 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_Values_353"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_names = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                const_names.insert(item_const.ident.to_string());
            }
        }

        let mut found_target = false;
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if found_target {
                    break;
                }

                if let Some(where_clause) = &item_struct.generics.where_clause {
                    let mut has_const_generic = false;
                    for param in &item_struct.generics.params {
                        if let GenericParam::Const(_) = param {
                            has_const_generic = true;
                            break;
                        }
                    }

                    if !has_const_generic {
                        continue;
                    }

                    let mut const_generic_indices = Vec::new();
                    for (i, param) in item_struct.generics.params.iter().enumerate() {
                        if let GenericParam::Const(_) = param {
                            const_generic_indices.push(i);
                        }
                    }

                    let mut target_index = None;
                    for (i, param) in item_struct.generics.params.iter().enumerate() {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.default.is_some() {
                                target_index = Some(i);
                                break;
                            }
                        }
                    }

                    if let Some(target_index) = target_index {
                        let mut new_default_expr = None;

                        let mut candidate_indices = const_generic_indices.clone();
                        candidate_indices.retain(|&i| i != target_index);

                        if !candidate_indices.is_empty() {
                            let chosen_index = candidate_indices
                                [rng.gen_range(0..candidate_indices.len())];
                            if let GenericParam::Const(chosen_param) =
                                &item_struct.generics.params[chosen_index]
                            {
                                new_default_expr =
                                    Some(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(chosen_param.ident.clone()),
                                    }));
                            }
                        }

                        if new_default_expr.is_none() {
                            let mut const_candidates: Vec<String> =
                                const_names.iter().cloned().collect();
                            const_candidates.shuffle(&mut rng);

                            for candidate in const_candidates {
                                let candidate_ident = Ident::new(&candidate, Span::call_site());
                                let candidate_expr = Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: SynPath::from(candidate_ident.clone()),
                                });

                                let mut dummy_struct = item_struct.clone();
                                if let GenericParam::Const(dummy_param) =
                                    &mut dummy_struct.generics.params[target_index]
                                {
                                    dummy_param.default = Some(candidate_expr.clone());
                                }

                                let dummy_file = File {
                                    shebang: None,
                                    attrs: file.attrs.clone(),
                                    items: {
                                        let mut items = file.items.clone();
                                        items.push(Item::Struct(dummy_struct));
                                        items
                                    },
                                };

                                let dummy_code = dummy_file.to_token_stream().to_string();
                                if syn::parse_file(&dummy_code).is_ok() {
                                    new_default_expr = Some(candidate_expr);
                                    break;
                                }
                            }
                        }

                        if new_default_expr.is_none() {
                            let mut const_candidates: Vec<String> =
                                const_names.iter().cloned().collect();
                            const_candidates.shuffle(&mut rng);

                            for candidate in const_candidates {
                                let candidate_ident = Ident::new(&candidate, Span::call_site());
                                let candidate_expr = Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: SynPath::from(candidate_ident.clone()),
                                });

                                let mut dummy_struct = item_struct.clone();
                                if let GenericParam::Const(dummy_param) =
                                    &mut dummy_struct.generics.params[target_index]
                                {
                                    dummy_param.default = Some(candidate_expr.clone());
                                }

                                let dummy_file = File {
                                    shebang: None,
                                    attrs: file.attrs.clone(),
                                    items: {
                                        let mut items = file.items.clone();
                                        items.push(Item::Struct(dummy_struct));
                                        items
                                    },
                                };

                                let dummy_code = dummy_file.to_token_stream().to_string();
                                if syn::parse_file(&dummy_code).is_ok() {
                                    new_default_expr = Some(candidate_expr);
                                    break;
                                }
                            }
                        }

                        if let Some(new_default_expr) = new_default_expr {
                            if let GenericParam::Const(target_param) =
                                &mut item_struct.generics.params[target_index]
                            {
                                target_param.default = Some(new_default_expr);
                                found_target = true;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets struct definitions with const generic parameters that have default values. It modifies the default value to reference another const generic parameter or a more complex expression involving existing constants or parameters. This transformation aims to increase the complexity of const generic defaults, potentially exposing issues in the compiler's const evaluation and type checking mechanisms."
    }
}