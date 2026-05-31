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

pub struct Swap_Const_Generic_Parameter_Positions_367;

impl Mutator for Swap_Const_Generic_Parameter_Positions_367 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Parameter_Positions_367"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((self_ty, args)) = extract_self_type_and_args(&item_impl.self_ty) {
                    let const_params = extract_const_params(&item_impl.generics);
                    let assoc_const_refs = find_assoc_const_refs(&args, &const_params);
                    
                    if !assoc_const_refs.is_empty() {
                        let mut new_args = args.clone();
                        if new_args.len() >= 2 {
                            let indices: Vec<usize> = (0..new_args.len()).collect();
                            let mut shuffled_indices = indices.clone();
                            shuffled_indices.shuffle(&mut rng);
                            
                            for (i, &new_idx) in shuffled_indices.iter().enumerate() {
                                if i != new_idx && (assoc_const_refs.contains(&i) || assoc_const_refs.contains(&new_idx)) {
                                    new_args.swap(i, new_idx);
                                    break;
                                }
                            }
                        } else if new_args.len() == 1 && !assoc_const_refs.is_empty() {
                            let duplicate_arg = new_args[0].clone();
                            new_args.push(duplicate_arg);
                        }
                        
                        if new_args != args {
                            let new_path = create_new_path(self_ty, new_args);
                            item_impl.self_ty = Box::new(new_path);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations that reference associated constants from the implementing type within const generic arguments. It swaps the order of const generic arguments if multiple are present, or duplicates the associated constant reference if only one is present. This creates potential cyclic dependencies in const generic evaluation, stressing the compiler's query system and potentially triggering deadlocks or ICEs under parallel compilation."
    }
}

fn extract_self_type_and_args(self_ty: &Box<Type>) -> Option<(SynPath, Vec<GenericArgument>)> {
    if let Type::Path(type_path) = &**self_ty {
        if let Some(last_segment) = type_path.path.segments.last() {
            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                let mut path_without_args = type_path.path.clone();
                if let Some(last) = path_without_args.segments.last_mut() {
                    last.arguments = PathArguments::None;
                }
                return Some((path_without_args, args.args.iter().cloned().collect()));
            }
        }
    }
    None
}

fn extract_const_params(generics: &Generics) -> HashSet<String> {
    let mut const_params = HashSet::new();
    for param in &generics.params {
        if let GenericParam::Const(const_param) = param {
            const_params.insert(const_param.ident.to_string());
        }
    }
    const_params
}

fn find_assoc_const_refs(args: &[GenericArgument], const_params: &HashSet<String>) -> HashSet<usize> {
    let mut assoc_refs = HashSet::new();
    for (i, arg) in args.iter().enumerate() {
        if let GenericArgument::Const(expr) = arg {
            if let Expr::Path(expr_path) = expr {
                if expr_path.path.segments.len() >= 2 {
                    let last_segment = expr_path.path.segments.last().unwrap();
                    if last_segment.ident.to_string().chars().next().map_or(false, |c| c.is_uppercase()) {
                        let first_segment = expr_path.path.segments.first().unwrap();
                        if const_params.contains(&first_segment.ident.to_string()) {
                            assoc_refs.insert(i);
                        }
                    }
                }
            }
        }
    }
    assoc_refs
}

fn create_new_path(base_path: SynPath, args: Vec<GenericArgument>) -> Type {
    let mut new_segments = base_path.segments;
    if let Some(last_segment) = new_segments.last_mut() {
        let punctuated_args: Punctuated<GenericArgument, Comma> = args.into_iter().collect();
        last_segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
            colon2_token: None,
            lt_token: token::Lt::default(),
            args: punctuated_args,
            gt_token: token::Gt::default(),
        });
    }
    Type::Path(TypePath {
        qself: None,
        path: SynPath {
            leading_colon: base_path.leading_colon,
            segments: new_segments,
        },
    })
}